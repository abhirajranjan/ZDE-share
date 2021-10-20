import struct
import socket
import threading
import time
import typing
import platform
import json
from abstract import *
import os


class LanConnection(AbstractConnections):
    def __init__(self, ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4'):
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
        self.udp.active_connection = {}

        self.udp.listener_thread = threading.Thread(target=self.listen)
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

    def listen(self):
        """ listen for messages """
        data_per_ip = {}
        while True:
            data, sender = self.udp.listener_conn.recvfrom(listener_buffer)

            if (a := data.find(EOL)) != -1:
                data = data[:a]
                data_per_ip[sender] = data_per_ip[sender] + data if data_per_ip.get(sender, 0) else data

            if (a := data.find(EOF)) != -1:
                data = data[:a]
                if sender in data_per_ip:
                    decoded_data = json.loads(data_per_ip[sender].decode(packet_encoding))
                    del data_per_ip[sender]
                else:
                    decoded_data = json.loads(data.decode(packet_encoding))

                if decoded_data.get('type', None) == 'MySelf':
                    self.handle_active_connections(decoded_data)

    def handle_active_connections(self, data: dict[str('tcpIP'), str('tcpPort'),
                                                    str('device_name'), str('nickname')]):

        # checks if there is a fault in tcpIP server:port (as None or invalid) and also if we receive our own message
        if (not data.get('tcpIP_listener', None)) or \
                (not data.get('tcpPort_listener', None)) or \
                (data == self.myself):
            return
        self.udp.active_connection[Connection(device_name=data['deviceName'],
                                              nickname=data['nickname'],
                                              tcpPort=data['tcpPort_listener'],
                                              tcpIP=data['tcpIP_listener'])] = True

        print(data)

    @staticmethod
    def except_hook(e: Exception):
        print(e)

    def ping(self):
        while True:
            self.udp.conn.sendto(self.udp.serialized_myself + EOF, (self.udp.addr_info[4][0], port))
            time.sleep(packet_resend_after)
