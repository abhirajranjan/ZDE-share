from .constants import *
from .utils import UI_connector
from abc import ABC, abstractmethod

class UDP(ABC):
    def __init__(self):
        pass

class TCP(ABC):
    def __init__(self) -> None:
        pass

class Abstract(ABC):
    ...

class AbstractConnections(ABC):
    def __init__(self):
        self.udp = UDP()
        self.tcp = TCP()
        self.ui_connector = UI_connector()

        # define whether to use ipv4 | ipv6
        self.ipType = None

        # sets the data which is send every PACKET_RESEND_AFTER seconds
        # contains details about the addr:port in which tcp is listening
        # and details like name and device info
        # 
        #  - type             ::  defines the type of data [MySelf]
        #  - deviceName       ::  name of device
        #  - OS               ::  name of os
        #  - tcpIP_listener   ::  ip addr of tcp listener
        #  - tcpPort_listener ::  ip port of tcp listener
        self.myself = {}