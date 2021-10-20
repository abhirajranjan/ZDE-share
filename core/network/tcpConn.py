import socket
import mcast
import typing
import threading
import abstract
import json


# udp multicast (mcast) is to get refreshed list of devices by
# sending ping every $packet_resend_after (sec) on mcast server, while TCP server is for
# communicating between two devices for data exchange

# data send /recv in mcast is public to all present devices in the server
# data send via TCP is P2P transmission.


class NetworkManager(mcast.LanConnection):
    def __init__(self, ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4'):
        super().__init__(ip)
        self.setup_socket(ip)

    def setup_socket(self, ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4'):
        super().setup_socket(ip)
        self.tcp.tcp_listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.tcp.tcp_sender = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.tcp.tcp_listener.bind(('', 0))
        self.tcp.tcp_listener.listen(abstract.device_to_connect_at_one_time)
        self.tcp.tcp_listener.getsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        print(self.tcp.tcp_listener.getsockname())

        self.myself['tcpIP_listener'], self.myself['tcpPort_listener'] = self.tcp.tcp_listener.getsockname()
        self.generate_myself()

        self.tcp.tcp_listener_process = threading.Thread(target=self.tcp_listen)

    def tcp_listen(self):
        while True:
            # new connection obtained
            conn, addr = self.tcp.tcp_listener.accept()
            print(addr, 'yes its connecting')
            client_thread = threading.Thread(target=self.handle_client, args=(conn, addr))
            client_thread.start()

    def start_threads(self):
        super().start_threads()
        self.tcp.tcp_listener_process.start()

    def run(self):
        self.reset_ip()
        self.start_threads()

    def handle_client(self, conn, addr):
        data_send_prev = b''
        while True:
            data = conn.recv(abstract.listener_buffer)

            if (a := data.find(abstract.EOL)) != -1:
                data = data[:a]
                data_send_prev += data

            elif (a := data.find(abstract.EOF)) != -1:
                data = data[:a]
                if data_send_prev:
                    decoded_data = json.loads(data_send_prev.decode(abstract.packet_encoding))
                else:
                    decoded_data = json.loads(data.decode(abstract.packet_encoding))
                self.process_data(decoded_data, addr, conn)

    def process_data(self, data: dict, addr, conn: socket.socket):
        print(data, addr)


if __name__ == '__main__':
    manager = NetworkManager()
    manager.run()