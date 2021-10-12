import struct
import socket
import threading
import time
import typing
import platform
import json
from abstract import *


class LanConnection(AbstractConnections):
    def __init__(self, ip: typing.Union['ipv4', 'ipv6'] = 'ipv4'):
        super().__init__()
        self.ipType = ip
        self.myself = {
            'type': 'MySelf',
            'deviceName': platform.node(),
            'tcpIP': None,
            'tcpPort': None,
            'nickname': None,
        }
        self.udp.serialized_myself = json.dumps(self.myself, allow_nan=True).encode(packet_encoding)
        self.udp.active_connection = {}
        self.udp.listener_thread = threading.Thread(target=self.listen)

    def run(self):
        self.setup_socket(self.ipType)
        self.udp.listener_thread.start()
        self.ping()

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
        reset = 0
        while True:
            data, sender = self.udp.listener_conn.recvfrom(listener_buffer)

            while data[-1:] == EOL:
                reset = 3
                data = data[:-1]  # Strip trailing \0's

            while data[-1:] == EOF:
                reset = 1
                data = data[:-1]

            if reset == 3:
                data_per_ip[sender] = data_per_ip[sender] + data if data_per_ip.get(sender, 0) else data
                reset = 0

            if reset == 1:
                if sender in data_per_ip:
                    decoded_data = json.loads(data_per_ip[sender].decode(packet_encoding))
                    del data_per_ip[sender]
                else:
                    decoded_data = json.loads(data.decode(packet_encoding))
                # if decoded_data['type', None] == 'MySelf':
                #     self.handle_active_connections(decoded_data)
                print(decoded_data)
                reset = 0

    def handle_active_connections(self, data: dict['tcpIP', 'tcpPort', 'device_name', 'nickname']):
        if (not data.get('tcpIP', None)) or \
                (not data.get('tcpPort', None)) or \
                data == self.myself:
            return
        self.udp.active_connection[Connection(device_name=data['device_name'],
                                              nickname=data['nickname'],
                                              tcpPort=data['tcpPort'],
                                              tcpIP=data['tcpIP'])] = True

    @staticmethod
    def except_hook(e: Exception):
        print(e)

    def ping(self):
        while True:
            self.udp.conn.sendto(self.udp.serialized_myself + EOF, (self.udp.addr_info[4][0], port))
            time.sleep(packet_resend_after)


if __name__ == '__main__':
    connection = LanConnection()
    connection.run()
