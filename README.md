# crossword editor

Had the crazy idea of creating a crosswords editor in curses (terminal).  
Had not coded C in ages! Was very rusty. Was fun.  
Tested on Mac and Linux. Should work in Msys under Windows too.


## Sample screen follows:

```
+-+-+-+-+-+-+-+-+-+-+-+
|b|a|t|a|t|a|s|▮|c|i|a|
+-+-+-+-+-+-+-+-+-+-+-+
|a| | | | | |a| | | |c|
+-+-+-+-+-+-+-+-+-+-+-+
|c| | | | | |l| | | |o|
+-+-+-+-+-+-+-+-+-+-+-+
|a| | | | | |m| | | |r|
+-+-+-+-+-+-+-+-+-+-+-+
|l| | | | | |a| | | |d|
+-+-+-+-+-+-+-+-+-+-+-+
|h| | | | | |o| | | |a|
+-+-+-+-+-+-+-+-+-+-+-+
|a| | | | | |▮|r|i|a|▮|
+-+-+-+-+-+-+-+-+-+-+-+
|u|m|v|▮|@| | | | | | |
+-+-+-+-+-+-+-+-+-+-+-+
|▮|v|i|s|e|u| | | | | |
+-+-+-+-+-+-+-+-+-+-+-+
|a|▮|▮|u| | | | | | | |
+-+-+-+-+-+-+-+-+-+-+-+
|i|l|h|a| | | | | | | |
+-+-+-+-+-+-+-+-+-+-+-+


pos: 4,7 | mode: HOR | char: 67
```

## generate project

    premake4 gmake

You can generate xcode or vscode versions too,  
do `premake4 --help` to know more.

## compiling

to build the debug version:

    make

for the release version:

    make config=release

to skip curses altogether and just test load/save:

    make config=debugnocurses

## running

    ./editor

or, if you want to override the default 11 x 11 dimensions:

    ./editor 16 14

Previous board is loaded if `data.bin`, if present.

## keys

- arrow keys to move
- write down characters to set cells
- BCKSPC deletes
- ENTER to fill/unfill cell (filled cells can't be edited, only unset)
- TAB to toggle between horizontal and vertical fill modes
- Q to save and exit

## TODO

- nothing for now, codewise :D
- would love to setup remote gdb (was not successful so far)

## Running in docker

build the image:

    docker build . -t gcc

use the image:

    docker run -it gcc bash

## Linux tools

basic memory test:

    valgrind ./editor

memory test:

    valgrind --trace-children=yes --track-fds=yes --log-fd=2 --error-limit=no \
         --leak-check=full --show-possibly-lost=yes --track-origins=yes \
         --show-reachable=yes --show-leak-kinds=all --xtree-leak=yes ./editor

print binary files:

    xxd data.bin (hex)
    xxd -b data.bin (binary)

use gdb:

    gdb editor
    break main
    run
    next
    next
    info locals
    print *state
    print *state->cells

## Yet to work

tested without success (uncomment Dockerfile lines):

    docker run -p 9091:9091 --security-opt seccomp:unconfined -v "$(pwd)":/source -it gcc bash

in mac, to open valgrind generated xtree files:

    brew install qcachegrind
    qcachegrind <file>

unsuccessful gdbserver visited by mac host vscode debugging:
    
    gdbserver localhost:9091 editor


## References

- [ncurses cheat sheet](https://github.com/thenamankumar/ncurses-cheatsheet/blob/master/cheatsheet.md#initialization-functions)
- [ncurses tutorial](https://www.youtube.com/watch?v=lV-OPQhPvSM&list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v)
- [vscode c dev with containers](https://lemariva.com/blog/2020/10/vscode-c-development-and-debugging-containers)