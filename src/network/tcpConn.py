import socket
import typing
import threading
import json
import socket

from .abstract import packet_encoding, device_to_connect_at_one_time, \
                    listener_buffer, EOF, ClassObject
from .mcast import LanConnection

# udp multicast (mcast) is to get refreshed list of devices by
# sending ping every $packet_resend_after (sec) on mcast server, while TCP server is for
# communicating between two devices for data exchange

# data send /recv in mcast is public to all present devices in the server
# data send via TCP is P2P transmission.


class NetworkManager(LanConnection):
    def __init__(self, ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
        super().__init__(ip=ip, debug=debug)
        self.debug = debug
        self.setup_socket(ip)

    def setup_socket(self, ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4'):
        super().setup_socket(ip)
        self.tcp.tcp_listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.tcp.tcp_sender = socket.create_connection

        self.tcp.tcp_listener.bind(('', 0))
        self.tcp.tcp_listener.listen(device_to_connect_at_one_time)
        self.tcp.tcp_listener.getsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        if self.debug:
            print('tcp ip: ', self.tcp.tcp_listener.getsockname())

        self.myself['tcpIP_listener'], self.myself['tcpPort_listener'] = self.tcp.tcp_listener.getsockname()
        self.generate_myself()

        self.tcp.tcp_listener_process = threading.Thread(target=self.tcp_listen)

    def tcp_listen(self):
        while True:
            # new connection obtained
            conn, addr = self.tcp.tcp_listener.accept()
            if self.debug:
                print(f'\n[INFO]: {addr} connected.\n')
            classcontainer = ClassObject()
            # process func handles the process to exec after getting headers 
            classcontainer.process_function = None
            client_thread = threading.Thread(target=self.handle_client, args=(conn, addr, classcontainer))
            client_thread.start()

    def start_threads(self):
        super().start_threads()
        self.tcp.tcp_listener_process.start()

    def run(self):
        self.reset_ip()
        self.start_threads()

    def handle_client(self, conn, addr, container):
        data_send_prev = b''
        while True:
            try:
                data = conn.recv(listener_buffer)
                if container.process_function:
                    if container.process_function(data, addr, conn, container):
                        break

                elif data.rstrip().endswith(EOF):
                    data = data[:data.rfind(EOF)].rstrip()
                    if data_send_prev:
                        decoded_data = json.loads((data_send_prev+data).decode(packet_encoding))
                    else:
                        decoded_data = json.loads(data.decode(packet_encoding))
                    self.process_data(decoded_data, addr, conn, container)

                else:
                    data_send_prev += data

            except Exception as e:
                if self.debug:
                    self.except_hook(e)
                data_send_prev = b''

    def process_data(self, data: dict, addr, conn: socket.socket, container: ClassObject):
        # 1v1 connection
        assert "event-type" in data
        if data["event-type"] == 'data-transfer':
            assert "data-type" in data
            assert "metadata" in data
            assert "filename" in data["metadata"]

            if data["data-type"] == "bytes":
                container.process_filename = data["metadata"]["filename"]
                container.process_function = self.process_bytes

            elif data["data-type"] == "ping":
                container.process_function = self.recv_ping
                container.pingData = b''
    
    def process_bytes(self, data: bytes, addr, conn: socket.socket, container: ClassObject) -> None:
        rt = 0
        if data.rstrip().endswith(EOF):
            container.process_function = None
            data = data[:data.rfind(EOF)].rstrip()
            del container.process_filename
            rt = 1
            
        with open(container.process_filename, 'a') as file:
            file.write(data.decode(packet_encoding))
        
        if rt:
            return 1

    def recv_ping(self, data: bytes, addr, conn: socket.socket, container: ClassObject) -> None:
        if data.rstrip().endswith(EOF):
            container.process_function = None
            data = data[:data.rfind(EOF)].rstrip()
            self.ui_connector.pipe({"type":"ping", "data-type":str, "data": (container.pingData+data).decode(packet_encoding)})
            del container.pingData
            return 1

        container.pingData += data
