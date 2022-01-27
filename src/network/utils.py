import threading
import time

from .constants import *


class Connection:
    def __init__(self, parent, device_name: str, tcpIP: str, tcpPort: int, nickname: str):
        self.parent = parent
        self.deviceName = device_name
        self.tcpIP = tcpIP
        self.tcpPort = tcpPort
        self.nickname = nickname
        self.time = time.time()
        self.__timer_thread__ = threading.Thread(target=self.__timer_func__)
        self.__timer_thread__.start()

    def remove_timer(self):
        if time.time() - self.time > connection_remove_time:
            self.parent.remove_connection(self)

    def update_time(self):
        self.time = time.time()

    def __timer_func__(self):
        while True:
            time.sleep(connection_remove_time)
            if self.time == None:
                self.destroy()
                break
            self.remove_timer()

    def destroy(self):
        del self.__timer_thread__

    @property
    def name(self):
        pass

    @name.getter
    def name(self):
        return self.nickname if self.nickname else self.deviceName


class ConnectionContainer:
    def __init__(self):
        self.connection_list = dict()

    def create_connection(self, device_name: str, tcpIP: str, tcpPort: int, nickname: str):
        conn = Connection(self, device_name, tcpIP, tcpPort, nickname)
        self[conn] = True
    
    def remove_connection(self, conn):
        try:
            conn.time = None
            del self.connection_list[self[conn]]
        except KeyError:
            pass

    def __getitem__(self, conn):
        for i in self.connection_list:
            if (i.deviceName == conn.deviceName and \
               i.tcpIP == conn.tcpIP and \
               i.tcpPort == conn.tcpPort and \
               i.nickname == conn.nickname):
                return i

    def __setitem__(self, key, value):
        if conn := self[key]:
            conn.update_time()

        else:
            self.connection_list[key] = value