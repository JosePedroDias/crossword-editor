#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

// cheat sheet:
// https://github.com/thenamankumar/ncurses-cheatsheet/blob/master/cheatsheet.md#initialization-functions
//
// tutorial:
// https://www.youtube.com/watch?v=lV-OPQhPvSM&list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v

int width  = 11;
int height = 11;

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

struct cell {
  bool filled;
  char value;
};

void drawCell(struct cell *ce, int x, int y) {
  char v = ce->value | ' ';
  if (ce->filled) {
    v = ' ';
  }

  if (ce->filled) {
    attron(COLOR_PAIR(CLR_FILLED));
  }

  mvaddch(y*2+1, x*2+1, v);

  if (ce->filled) {
    attroff(COLOR_PAIR(CLR_FILLED));
  }
}

int main() {
  struct cell cells[width][height];

  FILE *file;

  int x = 0;
  int y = 0;
  int c = 0;
  int xx, yy;

  file = fopen(DEFAULT_FILE, "rb");
  if (file != NULL) {
    // load from file
    fread(&cells, sizeof(struct cell), width*height, file);
    fclose(file);
  } else {
    // clean cells
    for (yy = 0; yy < height; ++yy) {
      for (xx = 0; xx < width; ++xx) {
        struct cell* ce = &cells[xx][yy];
        ce->value = ' ';
        ce->filled = FALSE;
      }
    }
  }

  initscr();

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  //cbreak();
  //raw();
  noecho();
  //attron(A_INVIS);
  start_color();

  //        pairNumber   foreground    background
  init_pair(CLR_GRID,    COLOR_YELLOW, COLOR_BLACK);
  init_pair(CLR_FILLED,  COLOR_BLACK,  COLOR_WHITE);
  init_pair(CLR_CURSOR,  COLOR_RED,    COLOR_BLACK);

  // draw grid
  attron(COLOR_PAIR(CLR_GRID));
  for (yy = 0; yy <= height; ++yy) {
    for (xx = 0; xx <= width; ++xx) {
      if (xx == width) {
        mvprintw(yy*2,width*2, "+");
      } else {
        mvprintw(yy*2,   xx*2, "+-");
      }
      if (yy != height) {
        mvprintw(yy*2+1, xx*2, "|");
      }
    }
  }
  attroff(COLOR_PAIR(CLR_GRID));

  // draw all content
  for (yy = 0; yy < height; ++yy) {
    for (xx = 0; xx < width; ++xx) {
      struct cell* ce = &cells[xx][yy];
      drawCell(ce, xx, yy);
    }
  }

  for (;;) {

    // draw cursor
    attron(COLOR_PAIR(CLR_CURSOR));
    mvaddch(y*2+1, x*2+1, '@');
    attroff(COLOR_PAIR(CLR_CURSOR));

    mvprintw(STATUS_Y, 0, "y: %d, y: %d, (c: %d)    ", y, x, c);
    refresh();

    c = getch();

    drawCell(&cells[x][y], x, y);

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
      cells[x][y].filled = !cells[x][y].filled;
    } else if (c >= 97 && c <= 122) { // a to z
      cells[x][y].value = (char)c;
    }
  }

  endwin();

  file = fopen(DEFAULT_FILE, "wb");
  fwrite(&cells, sizeof(struct cell), width*height, file);
  fclose(file);

  return 0;
}

