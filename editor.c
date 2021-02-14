#include <ncurses.h>

// cheat sheet:
// https://github.com/thenamankumar/ncurses-cheatsheet/blob/master/cheatsheet.md#initialization-functions

// tutorial
// https://www.youtube.com/watch?v=lV-OPQhPvSM&list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v

int WIDTH = 11;
int HEIGHT = 11;

#define ESCAPE 27
#define SPACE  32

#define LEFT  68
#define RIGHT 67
#define UP    65
#define DOWN  66

struct cell {
  bool filled;
  char value;
};

int main() {
  struct cell cells[WIDTH][HEIGHT];

  int x = 0;
  int y = 0;
  int c = 0;

  initscr();
  //cbreak();
  //raw();
  noecho();
  //attron(A_INVIS);

  for (;;) {

    int xx, yy;
    for (yy = 0; yy < HEIGHT; ++yy) {
      for (xx = 0; xx < WIDTH; ++xx) {
        mvaddch(y, x, cells[x][y].value);
      }
    }

    mvaddch(y, x, '@');
    mvprintw(19, 0, "y: %d, y: %d, (c: %d)", y, x, c);
    move(20, 20);
    refresh();

    c = getch();

    //mvaddch(y, x, ' ');
    clear();

    if (c == SPACE) {
       break;
    } else if (c == LEFT && x > 0) {
      --x;
    } else if (c == RIGHT && x < WIDTH-1) {
      ++x;
    } else if (c == UP && y > 0) {
      --y;
    } else if (c == DOWN && y < HEIGHT-1) {
      ++y;
    //} else if (c >= SPACE) {
    //  cells[x][y].filled = !cells[x][y].filled;
    } else if (c >= 97 && c <= 122) {
      cells[x][y].value = (char)c;
    }
  }

  endwin();

  printf("all done!");

  return 0;
}

