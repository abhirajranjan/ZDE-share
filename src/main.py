import sys
import typing

from cli_interface import run


def CLI(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    run(ip=ip, debug=debug)


def GUI(debug=False):
    from PyQt6.QtWidgets import QApplication
    app = QApplication()
    # GUI app widget comes here
    sys.exit(app.exec())


if len(sys.argv) >= 2:
    if sys.argv[1] in {'cli', '--cli', '-cli', '-c'}:
        if len(sys.argv) == 3 and sys.argv[2]  in {"-d", '--debug'}:
            print('hh')
            CLI(debug=True)
        else:
            CLI()

    elif sys.argv[1] in {'gui', '--gui', '-gui', '-g'}:
        if len(sys.argv) == 3 and sys.argv[2]  in {"-d", '--debug'}:
            GUI(debug=True)
        else:
            GUI()