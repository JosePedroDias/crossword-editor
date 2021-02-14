# crossword editor

## generate project

    premake4 gmake

## compiling

    make

or

    make config=release


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


## References

- [ncurses cheat sheet](https://github.com/thenamankumar/ncurses-cheatsheet/blob/master/cheatsheet.md#initialization-functions)
- [ncurses tutorial](https://www.youtube.com/watch?v=lV-OPQhPvSM&list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v)
