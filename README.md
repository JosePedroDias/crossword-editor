# crossword editor

## generate project

    premake4 gmake

## compile and run

    make
    ./editor

you can pass initial dimensions for the board

previous board is loaded if `data.bin` is present

## keys

- arrow keys to move
- write down characters to set cell
- ENTER to fill/unfill cell
- TAB to toggle between horizontal and vertical fill modes
- space to exit
- Q to save and exit

## TODO

- binary format with version, width and height
