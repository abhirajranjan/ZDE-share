import sys
import typing

from network.main import NetworkManager


CLI_ARGUMENTS = {'list', 'connect', 'sendfile', 'ping'}


def cliparser(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    nm_pt = NetworkManager(ip=ip, debug=debug)
    nm_pt.run()
    
    while True:
        cmd = input('>>> ')
        if cmd in {'exit', '\4'}:
            sys.exit()

        if cmd.strip() in CLI_ARGUMENTS:
            if cmd == 'list':
                print(nm_pt.get_connections())
                continue

            if cmd == "connect":
                print("connecting...")
                continue

            print(f'incorrect use of {cmd}.')
        else:
            print(f'Command {cmd} not found. Type "help" to find possible commands.')


if __name__ == "__main__":
    cliparser()