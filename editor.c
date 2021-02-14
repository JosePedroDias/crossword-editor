#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


//// DEFINES

#define CHAR_TAB      9
#define CHAR_ENTER   10
#define CHAR_ESCAPE  27 // problematic?
#define CHAR_BCKSPC 127

#define CHAR_LEFT    68
#define CHAR_RIGHT   67
#define CHAR_UP      65
#define CHAR_DOWN    66

#define CHAR_SPACE   32
#define CHAR_a       97
#define CHAR_z      122
#define CHAR_Q       81

#define CLR_GRID   1
#define CLR_FILLED 2
#define CLR_CURSOR 3

#define MODE_HORIZONTAL 0
#define MODE_VERTICAL   1

#define STATUS_Y 25

#define DEFAULT_FILE "data.bin"


//// data structures

#define S_CELL struct Cell
struct Cell {
  bool filled;
  char value;
};

#define FILE_FORMAT_VERSION 1 // TODO STILL UNUSED

#define S_STATE struct State // TODO STILL UNUSED
S_STATE {
  int dims[2];
  S_CELL* cells;
};


//// cell lookup and clean

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


//// file I/O

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


//// draw functions

void drawCursor(int x, int y) {
  attron(COLOR_PAIR(CLR_CURSOR));
  mvaddch(y*2+1, x*2+1, '@');
  attroff(COLOR_PAIR(CLR_CURSOR));
}

void drawStatus(int x, int y, int c, int mode) {
  char *modeS = mode == MODE_HORIZONTAL ? "HOR" : "VER";
  #ifdef DEBUG
    mvprintw(STATUS_Y, 0, "pos: %d,%d | mode: %s | char: %d   ", x, y, modeS, c);
  #else
    mvprintw(STATUS_Y, 0, "pos: %d,%d | mode: %s    ", x, y, modeS);
  #endif
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


//// act on user input

void advance(S_CELL* cell, int mode, int *x, int *y, int width, int height, int delta) {
  int *coord = mode == MODE_HORIZONTAL ? x : y;
  int maxValue = mode == MODE_HORIZONTAL ? width : height;
  bool valid = TRUE;

  if (delta == 1) {
    valid = *coord < maxValue - 1;
  } else { // -1
    valid = *coord > 0;
  }

  if (valid) {
    drawCell(cell, *x, *y);
    *coord += delta;
  }
}

bool processInput(int c, S_CELL *cells, int *x, int *y, int *mode, int width, int height) {
  S_CELL* cell = getCell(cells, *x, *y, width);
  if (c == CHAR_Q) {
    return false; // leave orderly
  } else if (c == CHAR_TAB) {
    *mode = *mode == MODE_HORIZONTAL ? MODE_VERTICAL : MODE_HORIZONTAL;
  } else if (c == CHAR_LEFT && *x > 0) {
    --*x;
  } else if (c == CHAR_RIGHT && *x < width-1) {
    ++*x;
  } else if (c == CHAR_UP && *y > 0) {
    --*y;
  } else if (c == CHAR_DOWN && *y < height-1) {
    ++*y;
  } else if (c == CHAR_ENTER) { // toggle filled
    cell->filled = !cell->filled;
    advance(cell, *mode, x, y, width, height, 1);
  } else if ((c >= CHAR_a && c <= CHAR_z) || c == CHAR_SPACE) {
    if (cell->filled) { return true; }
    cell->value = (char)c;
    advance(cell, *mode, x, y, width, height, 1);
  } else if (c == CHAR_BCKSPC) {
    cell->value = (char)' ';
    advance(cell, *mode, x, y, width, height, -1);
  }
  return true;
}


//// main

int main(int args, char **argv) {
  int width  = 11;
  int height = 11;

  if (args == 3) {
    width  = atoi(argv[1]);
    height = atoi(argv[2]);
    if (width < 1 || width > 20 || height < 1 || height > 20) {
      printf("Dimension values must be between 1 and 20 (received %d and %d)!\n", width, height);
      exit(2);
    }
  }

  S_CELL *cells = malloc(width * height * sizeof(S_CELL));

  int mode = MODE_HORIZONTAL;
  int x = 0;
  int y = 0;
  int c = 0;

  if (!load(cells, width, height)) {
    clearCells(cells, width, height);
  }

  initscr();

  if (has_colors() == FALSE) {
    endwin();
    printf("This terminal does not support colors!\n");
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

  do {
    drawCursor(x, y);
    drawStatus(x, y, c, mode);
    
    refresh();

    c = getch();

    drawCell(getCell(cells, x, y, width), x, y);
  } while (processInput(c, cells, &x, &y, &mode, width, height));

  endwin();

  save(cells, width, height);

  free(cells);

  return 0;
}

