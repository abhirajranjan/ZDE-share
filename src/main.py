import sys
import typing

from shell import shell
from cli import cliparser

def Shell(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    return shell(ip=ip, debug=debug)

def GUI(debug=False):
    from PyQt6.QtWidgets import QApplication
    app = QApplication()
    # GUI app widget comes here
    sys.exit(app.exec())

def parser():
    if len(sys.argv) >= 2:
        if sys.argv[1] in {'shell', '--shell', '-shell', '-s'}:
            if len(sys.argv) == 3 and sys.argv[2]  in {"-d", '--debug'}:
                Shell(debug=True)
            else:
                Shell()
            return

        elif sys.argv[1] in {'gui', '--gui', '-gui', '-g'}:
            if len(sys.argv) == 3 and sys.argv[2]  in {"-d", '--debug'}:
                GUI(debug=True)
            else:
                GUI()
            return

        elif sys.argv[1] in {'cli', '--cli', '-cli', '-c'}:
            if len(sys.argv) == 3 and sys.argv[2]  in {"-d", '--debug'}:
                cliparser(debug=True)
            else:
                cliparser()
    
    # default
    cliparser()

if __name__ == "__main__":
    parser()