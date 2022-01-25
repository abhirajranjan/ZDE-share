# Alora Share

Alora Share tends to replicate "Apple Environment" on mobile and desktop OS

__Alora Share for mobile has active development under__ <https://github.com/abhirajranjan/alora-share-mobile>

## Requirements

Alora Share cli is built under standard builtin python module - sockets, so in order to run program in cli enviroment there is no need for additional packages.

For Gui, it uses [PyQt6](https://pypi.org/project/PyQt6/) as base

```bash
pip install PyQt6
```

Full requirements are listed under requirements.txt

## File Structure

* All the source code is under src/.

* All guides and developer docs which are needed to understand the flow is under docs/.

* src/network/ handle all the network creation and all backend stuffs.

* src/gui/ controls all graphical user stuffs.

## Getting started

1. clone this repo and change directory

```bash
git clone https://github.com/abhirajranjan/alora-share.git && cd alora-share
```

2. project can run in cli as well as in gui enviroment

* For CLI

```bash
python3 main.py --cli
```

* For GUI

```bash
python3 main.py

or

python3 main.py --gui
```

## how to

* how to use cli can be found under docs/ .

## Inspiration & Reference

* [KDE Connect](https://github.com/KDE/kdeconnect-kde)