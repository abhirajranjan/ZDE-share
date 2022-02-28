import sys
import typing

from network.main import NetworkManager
from network.generate_requests import File_transfer


CLI_ARGUMENTS = {'list', 'sendfile', 'ping'}


def maxlen(arg: str) -> int:
    _max = 0
    for i in arg:
        _max = max(_max, len(i.name))
    return _max

def ping(*args, **kwargs):
    print(*args, **kwargs)

def list_connections(nm_pt: NetworkManager, conns: dict) -> None:
        _max = maxlen(conns)
        
        a = ('|'+'-'*(_max+8)+'|')
        a += '\n' 
        for idx, conn in enumerate(conns):
            a += (('| {:>2} - {:<'+str(_max+1)+'} |').format(idx+1, conn.name))
            a += '\n'
        a += ('|'+'-'*(_max+8)+'|')
        
        return a

def cliparser(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    nm_pt = NetworkManager(ip=ip, debug=debug)
    nm_pt.ui_connector.register(ping)
    nm_pt.run()
    
    while True:
        cmd = input('>>> ')
        if cmd in {'exit', '\4'}:
            sys.exit()

        cmd = cmd.split()

        if not len(cmd):
            print(f'Command {cmd} not found. Type "help" to find possible commands.')


        if cmd[0] in CLI_ARGUMENTS:
            conns = nm_pt.get_connections()
            
            if not len(cmd): continue

            if cmd[0] == 'list':
                print(list_connections(nm_pt, conns))
                continue
            
            if cmd[0] == 'ping':
                if len(cmd) == 3:
                    conn = list(conns)[int(cmd[1])-1]
                    if not conn:
                        print("connection not active with specified user")
                        continue
                    nm_pt.tcp.tcp_sender.sendto(File_transfer("ping", 'ping'), (conn.tcpIP,conn.tcpPort))
                    nm_pt.tcp.tcp_sender.sendto()
                    continue

            print(f'incorrect use of {cmd}.')
        else:
            print(f'Command {cmd} not found. Type "help" to find possible commands.')

if __name__ == "__main__":
    cliparser()