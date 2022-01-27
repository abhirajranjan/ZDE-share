import sys
from cli_interface import run


if len(sys.argv) == 2:
    if sys.argv[1] in  {'cli', '--cli', '-cli', '-c'}:
        run()
else:
    from PyQt6.QtWidgets import QApplication
    app = QApplication()
    # GUI app widget comes here
    sys.exit(app.exec())
