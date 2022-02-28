import struct
import socket
import threading
import time
import typing
import platform
import json
import os

from .abstract import packet_encoding, EOF, AbstractConnections, \
                    ipv6_grp, ipv4_grp, ttl, port, listener_buffer, packet_resend_after
from .utils import ConnectionContainer, Connection

class LanConnection(AbstractConnections):
    def __init__(self, ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
        super().__init__()
        self.ipType = ip
        self.myself = {
            'type': 'MySelf',
            'deviceName': platform.node(),
            'OS': os.name,
            'tcpIP_listener': None,
            'tcpPort_listener': None,
            'tcpIP_sender': None,
            'tcpPort_sender': None,
            'nickname': None,
        }
        self.debug = debug
        self.udp.active_connection = ConnectionContainer()

        self.udp.listener_thread = threading.Thread(target=self.listen, args=(self.debug,))
        self.udp.sender_thread = threading.Thread(target=self.ping)

    def generate_myself(self):
        self.udp.serialized_myself = json.dumps(self.myself, allow_nan=True).encode(packet_encoding)

    def run(self):
        self.setup_socket(self.ipType)
        self.reset_ip()
        self.start_threads()

    def reset_ip(self):
        self.myself['tcpIP_listener'] = self.udp.listener_conn.getsockname()[0]
        self.myself['tcpIP_sender'] = self.udp.listener_conn.getsockname()[0]
        self.generate_myself()

    def start_threads(self):
        self.udp.listener_thread.start()
        self.udp.sender_thread.start()

    def setup_socket(self, ip):
        self.udp.addr_info = socket.getaddrinfo(ipv6_grp if ip == 'ipv6' else ipv4_grp, None)[0]

        self.udp.conn = socket.socket(self.udp.addr_info[0], socket.SOCK_DGRAM)
        self.udp.listener_conn = socket.socket(self.udp.addr_info[0], socket.SOCK_DGRAM)

        # Set Time-to-live (optional)
        ttl_bin = struct.pack('@i', ttl)

        # Allow multiple copies of this program on one machine
        # (not strictly needed)
        self.udp.listener_conn.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        self.udp.listener_conn.bind(('', port))  # accept all ping at local host
        group_bin = socket.inet_pton(self.udp.addr_info[0], self.udp.addr_info[4][0])

        # if self.addr_info[0] == socket.AF_INET:  # IPv4
        if ip == 'ipv4':
            mreq = group_bin + struct.pack('=I', socket.INADDR_ANY)
            self.udp.listener_conn.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

            self.udp.conn.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl_bin)
        else:
            mreq = group_bin + struct.pack('@I', 0)
            self.udp.listener_conn.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_JOIN_GROUP, mreq)

            self.udp.conn.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_MULTICAST_HOPS, ttl_bin)

        if self.debug:
            print("udp ip: ", self.udp.listener_conn.getsockname()[0])

    def listen(self, debug):
        """ listen for messages """
        data_per_ip = {}
        while True:
            try:
                data, sender = self.udp.listener_conn.recvfrom(listener_buffer)

                if (a := data.find(EOF)) != -1:
                    data = data[:a]
                    if sender in data_per_ip:
                        decoded_data = json.loads(data_per_ip[sender].decode(packet_encoding))
                        del data_per_ip[sender]
                    else:
                        decoded_data = json.loads(data.decode(packet_encoding))

                    if decoded_data.get('type', None) == 'MySelf':
                        self.handle_active_connections(decoded_data)
                
                else:
                    data_per_ip[sender] = data_per_ip[sender] + data if data_per_ip.get(sender, 0) else data

            except Exception as e:
                if self.debug:
                    self.except_hook(e)
                data_per_ip = {}

    def handle_active_connections(self, data: dict[str('tcpIP'), str('tcpPort'),
                                                    str('device_name'), str('nickname')]):

        # checks if there is a fault in tcpIP server:port (as None or invalid) and also if we receive our own message
        if (not data.get('tcpIP_listener', None)) or \
                (not data.get('tcpPort_listener', None)):
                return
        if (not self.debug) and data == self.myself:
            return
        
        self.udp.active_connection.create_connection(device_name=data['deviceName'],
                                              nickname=data['nickname'],
                                              tcpPort=data['tcpPort_listener'],
                                              tcpIP=data['tcpIP_listener'])

    def get_connections(self):
        return self.udp.active_connection.connection_list.copy()

    @staticmethod
    def except_hook(e: Exception):
        print(f'{type(e).__name__}: [Error {e.args[0]}] {e.args[1]}')

    def ping(self):
        while True:
            try:
                self.udp.conn.sendto(self.udp.serialized_myself + EOF, (self.udp.addr_info[4][0], port))
            except Exception as e:
                if self.debug:
                    self.except_hook(e)
            finally:
                time.sleep(packet_resend_after)