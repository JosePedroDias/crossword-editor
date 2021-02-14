#include <stdlib.h>
#include <ncurses.h>

// cheat sheet:
// https://github.com/thenamankumar/ncurses-cheatsheet/blob/master/cheatsheet.md#initialization-functions

// tutorial
// https://www.youtube.com/watch?v=lV-OPQhPvSM&list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v

int WIDTH = 11;
int HEIGHT = 11;

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

#define STATUS_Y 25

struct cell {
  bool filled;
  char value;
};

int main() {
  struct cell cells[WIDTH][HEIGHT];

  int x = 0;
  int y = 0;
  int c = 0;
  int xx, yy;

  // clean cells
  for (yy = 0; yy < HEIGHT; ++yy) {
    for (xx = 0; xx < WIDTH; ++xx) {
      struct cell* ce = &cells[xx][yy];
      ce->value = ' ';
      ce->filled = FALSE;
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
  for (yy = 0; yy <= HEIGHT; ++yy) {
    for (xx = 0; xx <= WIDTH; ++xx) {
      if (xx == WIDTH) {
        mvprintw(yy*2,WIDTH*2, "+");
      } else {
        mvprintw(yy*2,   xx*2, "+-");
      }
      if (yy != HEIGHT) {
        mvprintw(yy*2+1, xx*2, "|");
      }
    }
  }
  attroff(COLOR_PAIR(CLR_GRID));

  for (;;) {
    attron(COLOR_PAIR(CLR_CURSOR));
    mvaddch(y*2+1, x*2+1, '@');
    attroff(COLOR_PAIR(CLR_CURSOR));

    mvprintw(STATUS_Y, 0, "y: %d, y: %d, (c: %d)    ", y, x, c);
    refresh();

    c = getch();

    {
      struct cell* ce = &cells[x][y];
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

    if (c == CHAR_SPACE) { // leave
       break;
    } else if (c == CHAR_LEFT && x > 0) {
      --x;
    } else if (c == CHAR_RIGHT && x < WIDTH-1) {
      ++x;
    } else if (c == CHAR_UP && y > 0) {
      --y;
    } else if (c == CHAR_DOWN && y < HEIGHT-1) {
      ++y;
    } else if (c == CHAR_ENTER) { // toggle filled
      cells[x][y].filled = !cells[x][y].filled;
    } else if (c >= 97 && c <= 122) { // a to z
      cells[x][y].value = (char)c;
    }
  }

  endwin();

  printf("all done!");

  return 0;
}

