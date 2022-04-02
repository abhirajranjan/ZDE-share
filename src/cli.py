from fileinput import filename
import sys
import typing
import pathlib

from network.constants import EOF, EOF_STRING, LISTENER_BUFFER

from network.main import NetworkManager
from network.generate_requests import File_transfer, map_request


def maxlen(arg: typing.Iterable) -> int:
    _max = 0
    for i in arg:
        _max = max(_max, len(i.name))
    return _max

CLI_ARGUMENTS = {'list', 'sendfile', 'ping', 'help'}
HELP = {
    "list": [
        "list all the connection available",
        [
            ">>> list",
        ]
    ],
    "sendfile": [
        "send a file to specific device on network",
        [
            ">>> sendfile {location of file} {no on list}",
            "",
            "if 'list' returns ",
            "|-------------|",
            "| 1 - GenHost |",
            "|-------------|",
            "",
            "sendfile /downloads/test1.txt 1",
            "or",
            "sendfile C://test1.txt 1"
        ]
    ],
    "ping": [
        "ping a device in network",
        [
            ">>> ping {no on list} {Name of device}",
            "",
            "if list returns:",
            "|-------------|",
            "| 1 - GenHost |",
            "|-------------|",
            ">>> ping 1"
        ]
    ]
}

HELP['general'] = '\n'.join(f"{k}".ljust(10) + f": {v[0]}" for k,v in HELP.items())
HELP['general'] += "\n\ntype help cmd for cmd specific help"


def server_res(*args, **kwargs):
    # {"type":"ping", "data-type":str, "data": container.pingData}
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
    nm_pt.ui_connector.register(server_res)
    nm_pt.run()
    
    while True:
        cmd = input('\n>>> ')
        if cmd in {'exit', '\4'}:
            sys.exit()

        cmd = cmd.split()

        if len(cmd) == 0:
            print(f'Command {" ".join(cmd)} not found. Type "help" to find possible commands.')
            continue

        if cmd[0] in CLI_ARGUMENTS:
            conns = nm_pt.get_connections()
            
            if not len(cmd): continue

            if cmd[0].lower() == 'help':
                if len(cmd) == 1:
                    print(HELP['general'])
                    continue

                elif len(cmd) == 2 and cmd[1] in HELP:
                    print(f' {cmd[1]} ')
                    print('-'*(len(cmd[1])+2), end='\n')
                    print(f'{HELP[cmd[1]][0]}', end='\n\n')
                    print(f'USAGE:')
                    print('\t'+'\n\t'.join(HELP[cmd[1]][1]))
                    continue

            elif cmd[0] == 'list':
                print(list_connections(nm_pt, conns))
                continue
            
            elif cmd[0] == 'ping':
                if len(cmd) == 2:
                    conn = list(conns)[int(cmd[1])-1]
                    if not conn:
                        print("connection not active with specified user")
                        continue
                    with nm_pt.tcp.tcp_sender((conn.tcpIP,conn.tcpPort)) as sock:
                        for to_send_data in map_request(File_transfer(filename = "ping", data_type = 'ping')+EOF_STRING):
                            sock.sendall(to_send_data)
                        
                        for to_send_data in map_request("ping!"+EOF_STRING):
                            sock.sendall(to_send_data)
                            print(len(to_send_data))
                    continue
            
            elif cmd[0] == 'sendfile':
                if len(cmd) == 3:
                    # if path is under quotes, remove the quotes 
                    if (cmd[2].startswith("'") and cmd[2].endswith("'")) or (cmd[2].startswith('"') and cmd[2].endswith('"')):
                        cmd[2] = cmd[1:-1]
                    
                    path = pathlib.Path(cmd[1])
                    # check if path exists 
                    if not path.exists():
                        # check is path points to file
                        if path.is_file():
                            with nm_pt.tcp.tcp_sender((conn.tcpIP,conn.tcpPort)) as sock:
                                for to_send_data in map_request(File_transfer(filename = path.name, data_type = 'bytes')+EOF_STRING):
                                    sock.sendall(to_send_data)
                                
                                with open(path, 'wb') as file:
                                    while a := file.read(LISTENER_BUFFER):
                                        sock.sendall(a)

                                sock.sendall(EOF)
                            continue

            print(f'incorrect use of {" ".join(cmd)}.')
        else:
            print(f'Command {" ".join(cmd)} not found. Type "help" to find possible commands.')

if __name__ == "__main__":
    cliparser()