from .tcpConn import NetworkManager as nm
import typing


class NetworkManager(nm):
    def __init__(self, ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
        super().__init__(ip=ip, debug=debug)
        