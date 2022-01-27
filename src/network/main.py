from .tcpConn import NetworkManager as nm
import typing


class NetworkManager(nm):
    def __init__(self):
        super().__init__()
        