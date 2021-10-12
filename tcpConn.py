import socket
import mcast
import typing

# udp multicast (mcast) is to get refreshed list of devices by
# sending ping every second on mcast server. while TCP server is for
# communicating between two devices for data exchange

# data send /recv in mcast is public to all present devices in the server
# data send via TCP is P2P transmission.


class NetworkManager(mcast.LanConnection):
    def __init__(self,  ip: typing.Union['ipv4', 'ipv6'] = 'ipv4'):
        super().__init__(ip)
        self.setup_socket(ip)

    def setup_socket(self, ip: typing.Union['ipv4', 'ipv6'] = 'ipv4'):
        super().setup_socket(ip)
        self.tcpConn =
