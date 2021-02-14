#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

// cheat sheet:
// https://github.com/thenamankumar/ncurses-cheatsheet/blob/master/cheatsheet.md#initialization-functions
//
// tutorial:
// https://www.youtube.com/watch?v=lV-OPQhPvSM&list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v

#define CHAR_ENTER  10
#define CHAR_ESCAPE 27 // problematic?
#define CHAR_SPACE  32
#define CHAR_LEFT  68
#define CHAR_RIGHT 67
#define CHAR_UP    65
#define CHAR_DOWN  66

#define CLR_GRID    1
#define CLR_FILLED  2
#define CLR_CURSOR  3

#define DEFAULT_FILE "data.bin"

#define STATUS_Y 25

#define S_CELL struct Cell
struct Cell {
  bool filled;
  char value;
};

#define FILE_FORMAT_VERSION 1

struct State {
  int dims[2];
  S_CELL* cells;
};

S_CELL * getCell(S_CELL *cells, int x, int y, int width) {
  return &cells[y*width + x];
}

void clearCells(S_CELL *cells, int width, int height) {
  int x, y;
  for (y = 0; y < height; ++y) {
    for (x = 0; x < width; ++x) {
      S_CELL* cell = getCell(cells, x, y, width);
      cell->value = ' ';
      cell->filled = FALSE;
    }
  }
}

bool load(S_CELL *cells, int width, int height) {
  FILE *file = fopen(DEFAULT_FILE, "rb");
  if (file == NULL) {
    return false;
  }
  fread(cells, sizeof(S_CELL), width*height, file);
  fclose(file);
  return true;
}

void save(S_CELL *cells, int width, int height) {
  FILE *file = fopen(DEFAULT_FILE, "wb");
  fwrite(cells, sizeof(S_CELL), width*height, file);
  fclose(file);
}

void drawGrid(int width, int height) {
  int x, y;
  attron(COLOR_PAIR(CLR_GRID));
  for (y = 0; y <= height; ++y) {
    for (x = 0; x <= width; ++x) {
      if (x == width) {
        mvprintw(y*2,width*2, "+");
      } else {
        mvprintw(y*2,   x*2, "+-");
      }
      if (y != height) {
        mvprintw(y*2+1, x*2, "|");
      }
    }
  }
  attroff(COLOR_PAIR(CLR_GRID));
}

void drawCell(S_CELL *cell, int x, int y) {
  char v = cell->value | ' ';
  if (cell->filled) {
    v = ' ';
  }

  if (cell->filled) {
    attron(COLOR_PAIR(CLR_FILLED));
  }

  mvaddch(y*2+1, x*2+1, v);

  if (cell->filled) {
    attroff(COLOR_PAIR(CLR_FILLED));
  }
}

void drawCells(S_CELL *cells, int width, int height) {
  int x, y;
  for (y = 0; y < height; ++y) {
    for (x = 0; x < width; ++x) {
      S_CELL* cell = getCell(cells, x, y, width);
      drawCell(cell, x, y);
    }
  }
}

int main(int args, char **argv) {
  int width  = 11;
  int height = 11;

  if (args == 3) {
    width  = atoi(argv[1]);
    height = atoi(argv[2]);
  }

  S_CELL *cells = malloc(width * height * sizeof(S_CELL));

  int x = 0;
  int y = 0;
  int c = 0;

  if (!load(cells, width, height)) {
    clearCells(cells, width, height);
  }

  initscr();

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  noecho();
  start_color();

  //        pairNumber   foreground    background
  init_pair(CLR_GRID,    COLOR_YELLOW, COLOR_BLACK);
  init_pair(CLR_FILLED,  COLOR_BLACK,  COLOR_WHITE);
  init_pair(CLR_CURSOR,  COLOR_RED,    COLOR_BLACK);

  drawGrid(width, height);
  drawCells(cells, width, height);

  for (;;) {

    // draw cursor
    attron(COLOR_PAIR(CLR_CURSOR));
    mvaddch(y*2+1, x*2+1, '@');
    attroff(COLOR_PAIR(CLR_CURSOR));

    mvprintw(STATUS_Y, 0, "y: %d, y: %d, (c: %d)    ", y, x, c);
    refresh();

    c = getch();

    drawCell(getCell(cells, x, y, width), x, y);

    if (c == CHAR_SPACE) { // leave
       break;
    } else if (c == CHAR_LEFT && x > 0) {
      --x;
    } else if (c == CHAR_RIGHT && x < width-1) {
      ++x;
    } else if (c == CHAR_UP && y > 0) {
      --y;
    } else if (c == CHAR_DOWN && y < height-1) {
      ++y;
    } else if (c == CHAR_ENTER) { // toggle filled
      S_CELL* cell = getCell(cells, x, y, width);
      cell->filled = !cell->filled;
    } else if (c >= 97 && c <= 122) { // a to z
      S_CELL* cell = getCell(cells, x, y, width);
      cell->value = (char)c;
    }
  }

  endwin();

  save(cells, width, height);

  free(cells);

  return 0;
}

