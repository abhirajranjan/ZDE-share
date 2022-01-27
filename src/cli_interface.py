import typing
import subprocess
import sys
import code

from network.main import NetworkManager


intro = "Alora Python Shell\nuse nm to access NetworkManager class"

def start_server(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    nm = NetworkManager(ip=ip, debug=debug)
    nm.run()
    return nm

def run(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    try:
        nm = start_server(ip=ip, debug=debug)
        execute(nm)
    except Exception as e:
        print(f'{type(e).__name__}: [Error-{e.args[0]}] {e.args[1]}')

def execute(nm: NetworkManager):
    code.interact(banner = intro, local=locals(), exitmsg='exiting..')
    sys.exit()

if __name__ == "__main__":
    run()