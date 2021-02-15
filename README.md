# crossword editor

## generate project

    premake4 gmake

## compiling

    make

or

    make config=release

or

    make config=debugnocurses

## running

    ./editor

or, if you want to override the default 11 x 11 dimensions:

    ./editor 16 14

Previous board is loaded if `data.bin`, if present

## keys

- arrow keys to move
- write down characters to set cell
- BCKSPC deletes
- ENTER to fill/unfill cell
- TAB to toggle between horizontal and vertical fill modes
- Q to save and exit

## TODO

- binary format with version, width and height


## Running in docker

    docker build . -t gcc
    docker run -p 9091:9091 --security-opt seccomp:unconfined -v "$(pwd)":/source -it gcc bash

    docker run -p 2222:22 --security-opt seccomp:unconfined -it gcc bash
    
    docker run -d -p 2222:22 --security-opt seccomp:unconfined gcc

    docker run -p 9091:9091 --security-opt seccomp:unconfined -v (pwd):/source -it gcc bash

    make clean
    make

    ./editor

    valgrind ./editor

    gdb editor
    break main
    run
    next
    next
    info locals
    print *state
    print *state->cells

    https://gist.github.com/rkubik/b96c23bd8ed58333de37f2b8cd052c30
    
    gdbserver localhost:9091 editor


## References

- [ncurses cheat sheet](https://github.com/thenamankumar/ncurses-cheatsheet/blob/master/cheatsheet.md#initialization-functions)
- [ncurses tutorial](https://www.youtube.com/watch?v=lV-OPQhPvSM&list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v)
- [vscode c dev with containers](https://lemariva.com/blog/2020/10/vscode-c-development-and-debugging-containers)