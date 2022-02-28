from .constants import *
from .utils import UI_connector


class ClassObject:
    def __init__(self):
        pass


class AbstractConnections:
    def __init__(self):
        self.udp = ClassObject()
        self.tcp = ClassObject()
        self.ui_connector = UI_connector()