import pickle
import time
import struct
import socket
import threading
import typing
import pickle
# RULES :
#   For Sending: EOL (/0)  at end means that there is yet to come more data (>1024)
#                       (use this if cant send data in single stream)
#                EOF (/1) at end means that full data is send and start processing it (<=1024)
#                       (use this for single sendto command)
port = 61235
ipv4_grp = '225.0.0.250'
ipv6_grp = 'ff15:7079:7468:6f6e:6465:6d6f:6d63:6173'
ttl = 1  # Increase to reach other networks
listener_buffer = 1024
EOF = b'\1'
EOL = b'\0'


class LanConnection:
    def __init__(self, ip: typing.Union['ipv4', 'ipv6'] = 'ipv4'):
        self.setup_socket(ip)

        self.listener_thread = threading.Thread(target=self.listen)
        self.listener_thread.start()

        self.test_sender()

    def setup_socket(self, ip):
        self.addr_info = socket.getaddrinfo(ipv6_grp if ip == 'ipv6' else ipv4_grp, None)[0]

        self.conn = socket.socket(self.addr_info[0], socket.SOCK_DGRAM)
        self.listener_conn = socket.socket(self.addr_info[0], socket.SOCK_DGRAM)

        # Set Time-to-live (optional)
        ttl_bin = struct.pack('@i', ttl)

        # Allow multiple copies of this program on one machine
        # (not strictly needed)
        self.listener_conn.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        self.listener_conn.bind(('', port))  # accept all ping at local host
        group_bin = socket.inet_pton(self.addr_info[0], self.addr_info[4][0])

        # if self.addr_info[0] == socket.AF_INET:  # IPv4
        if ip == 'ipv4':
            mreq = group_bin + struct.pack('=I', socket.INADDR_ANY)
            self.listener_conn.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

            self.conn.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl_bin)
        else:
            mreq = group_bin + struct.pack('@I', 0)
            self.listener_conn.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_JOIN_GROUP, mreq)

            self.conn.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_MULTICAST_HOPS, ttl_bin)

    def listen(self):
        """ listen for messages """
        data_per_ip = {}
        reset = 0
        while True:
            data, sender = self.listener_conn.recvfrom(listener_buffer)

            while data[-1:] == EOL:
                reset = 3
                data = data[:-1]  # Strip trailing \0's

            while data[-1:] == EOF:
                reset = 1
                data = data[:-1]

            if reset == 3:
                data_per_ip[sender] = data_per_ip[sender]+data if data_per_ip.get(sender, 0) else data
                reset = 0

            if reset == 1:
                print(str(sender) + '  ' + pickle.loads(data_per_ip[sender]))
                del data_per_ip[sender]
                reset = 0

    @staticmethod
    def except_hook(e: Exception):
        print(e)

    def test_sender(self):
        for i in range(3):
            data = 'hello world !, my name is Abhiraj Ranjan\n'
            print('sending ' + data)
            self.conn.sendto(pickle.dumps(data)+EOL, (self.addr_info[4][0], port))
        self.conn.sendto(pickle.dumps(repr(time.time()))+EOF, (self.addr_info[4][0], port))


if __name__ == '__main__':
    connection = LanConnection()
