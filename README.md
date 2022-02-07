# Alora Share

Alora Share tends to replicate "Apple Environment" on mobile and desktop OS

__Alora Share for mobile will be in development under__ <https://github.com/abhirajranjan/alora-share-mobile>

## Requirements

Alora Share cli is built under standard builtin python module - sockets, so in order to run program in cli enviroment there is no need for additional packages.

For Gui, it uses [PyQt6](https://pypi.org/project/PyQt6/) as base

```bash
pip install PyQt6
```

Full requirements are listed under requirements.txt

## File Structure

* All the source code is under src/.

* src/network/ handle all the network creation and all backend stuffs.

## Getting started

1. clone this repo and change directory to src

```bash
git clone https://github.com/abhirajranjan/alora-share.git && cd alora-share/src
```

2. project can run in different enviroments

    for debug mode add -d or --debug at end of any enviroment command

<br>

* For command line usage

    view [#cli](#cli)  section for more information

    - --cli can be replaced by -c or -cli
```bash
python3 main.py --cli
```

  * For interactive  python like shell 
    
    - --shell can be replaced with -s or -shell
```shell
python3 main.py --shell 
```

## cli
command line supports few commands that can be directly accessed 

### help
list all the commands with usage structure

### list
lists all the active connection that can be found in the network

```
>>> list
|---------------|
|  1 - GenHost  |
|---------------|

>>>
```

### ping
use to ping a device

```
>>> list
|---------------|
|  1 - GenHost  |
|---------------|

>>> ping 1 GenHost
```

### sendfile
use to send a file to a device

[ TODO : able to send file to multiple use at a time ]

```
>>> list
|---------------|
|  1 - GenHost  |
|---------------|

>>> sendfile 1 GenHost /Downloads/test1.zip
```

## Inspiration & Reference

* [KDE Connect](https://github.com/KDE/kdeconnect-kde)