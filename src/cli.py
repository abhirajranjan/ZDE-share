import sys
import typing

from network.main import NetworkManager


CLI_ARGUMENTS = {'list', 'connect', 'sendfile', 'ping'}


def maxlen(arg: str) -> int:
    _max = 0
    for i in arg:
        _max = max(_max, len(i.name))
    return _max

def cliparser(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    nm_pt = NetworkManager(ip=ip, debug=debug)
    nm_pt.run()
    
    while True:
        cmd = input('>>> ')
        if cmd in {'exit', '\4'}:
            sys.exit()

        if cmd.strip() in CLI_ARGUMENTS:
            if cmd == 'list':
                conns = nm_pt.get_connections()
                _max = maxlen(conns)
                
                print('|'+'-'*(_max+8)+'|')
                for idx, conn in enumerate(conns):
                    print(('| {:>2} - {:<'+str(_max+1)+'} |').format(idx+1, conn.name))
                print('|'+'-'*(_max+8)+'|')

                print('\nTo connect type "connect" followed by serial number or name')
                continue

            if cmd == "connect":
                print("connecting...")
                continue

            print(f'incorrect use of {cmd}.')
        else:
            print(f'Command {cmd} not found. Type "help" to find possible commands.')


if __name__ == "__main__":
    cliparser()