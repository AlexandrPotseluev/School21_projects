#include "frontend.h"

void initGui() {
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_CYAN);
  init_pair(2, COLOR_BLUE, COLOR_BLUE);
  curs_set(0);
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(TIMEOUT);
}

void printTable(void) {
  clear();

  mvhline(0, 0, ACS_HLINE, FIELD_X * 2 + INFO_X * 2 + 2);
  mvhline(FIELD_Y + 1, 0, ACS_HLINE, FIELD_X * 2 + INFO_X * 2 + 2);
  mvhline(FIELD_Y - 6, FIELD_X * 2 + 2, ACS_HLINE, INFO_X * 2);
  mvvline(1, 0, ACS_VLINE, FIELD_Y);
  mvvline(1, FIELD_X * 2 + 1, ACS_VLINE, FIELD_Y);
  mvvline(1, FIELD_X * 2 + INFO_X * 2 + 2, ACS_VLINE, FIELD_Y);

  mvaddch(0, 0, ACS_ULCORNER);
  mvaddch(0, FIELD_X * 2 + INFO_X * 2 + 2, ACS_URCORNER);
  mvaddch(FIELD_Y + 1, 0, ACS_LLCORNER);
  mvaddch(FIELD_Y + 1, FIELD_X * 2 + INFO_X * 2 + 2, ACS_LRCORNER);
  mvaddch(0, FIELD_X * 2 + 1, ACS_TTEE);
  mvaddch(FIELD_Y + 1, FIELD_X * 2 + 1, ACS_BTEE);

  mvprintw(2, FIELD_X * 2 + 3, "HIGH SCORE:");
  mvprintw(4, FIELD_X * 2 + 3, "SCORE:");
  mvprintw(6, FIELD_X * 2 + 3, "LEVEL:");
  mvprintw(8, FIELD_X * 2 + 3, "SPEED:");
  mvprintw(10, FIELD_X * 2 + 3, "NEXT:");

  mvprintw(FIELD_Y - 5, FIELD_X * 2 + 2, "SPACE - Pause game");
  mvprintw(FIELD_Y - 4, FIELD_X * 2 + 2, "arrL - Move left");
  mvprintw(FIELD_Y - 3, FIELD_X * 2 + 2, "arrR - Move right");
  mvprintw(FIELD_Y - 2, FIELD_X * 2 + 2, "arrD - Move down");
  mvprintw(FIELD_Y - 1, FIELD_X * 2 + 2, "a - Rotate");
  mvprintw(FIELD_Y - 0, FIELD_X * 2 + 2, "ESC - Exit game");
}

void printStartScreen() {
  clear();
  printTable();
  mvprintw(FIELD_Y / 2, FIELD_X / 2, "Are you ready?");
  mvprintw(FIELD_Y / 2 + 1, FIELD_X / 2, "Press ENTER");
  move(FIELD_Y + 1, FIELD_X * 2 + INFO_X * 2 + 3);
}

void printGameOverScreen() {
  clear();
  printTable();
  mvprintw(FIELD_Y / 2 - 3, FIELD_X / 2, "GAME OVER!");
  mvprintw(FIELD_Y / 2 - 2, FIELD_X / 2, "Press ENTER");
  mvprintw(FIELD_Y / 2 - 1, FIELD_X / 2, "to start");
  mvprintw(FIELD_Y / 2, FIELD_X / 2, "or ESC");
  mvprintw(FIELD_Y / 2 + 1, FIELD_X / 2, "to finish");
  move(FIELD_Y + 1, FIELD_X * 2 + INFO_X * 2 + 3);
}

void printInfo(GameParams_t *game) {
  mvprintw(2, FIELD_X * 2 + 14, "%d", game->data->high_score);
  mvprintw(4, FIELD_X * 2 + 14, "%d", game->data->score);
  mvprintw(6, FIELD_X * 2 + 14, "%d", game->data->level);
  mvprintw(8, FIELD_X * 2 + 14, "%d", game->data->speed);

  for (int row = 0; row < FIGURE_SIZE; row++)
    for (int col = 0; col < FIGURE_SIZE; col++) {
      if (game->data->next[row][col]) {
        attron(COLOR_PAIR(game->data->next[row][col]));
        mvaddch(11 + row, FIELD_X * 2 + 6 * 2 + col * 2 - 2, ACS_CKBOARD);
        mvaddch(11 + row, FIELD_X * 2 + 6 * 2 + col * 2 - 3, ACS_CKBOARD);
        attroff(COLOR_PAIR(game->data->next[row][col]));
      }
    }
}

void printOneBlock(int i, int j, int block) {
  attron(COLOR_PAIR(block));
  mvaddch(i + 1, 1 + j * 2, ' ');
  mvaddch(i + 1, 1 + j * 2 + 1, ' ');
  attroff(COLOR_PAIR(block));
}

void printField(GameParams_t *game) {
  for (int i = 0; i < FIELD_Y; i++)
    for (int j = 0; j < FIELD_X; j++) {
      if (game->data->field[i][j])
        printOneBlock(i, j, game->data->field[i][j]);
      else {
        int x = j - game->figure->x;
        int y = i - game->figure->y;
        if (x >= 0 && x < FIGURE_SIZE && y >= 0 && y < FIGURE_SIZE)
          printOneBlock(i, j, game->figure->blocks[y][x]);
      }
    }
}

void printGame(GameParams_t *game) {
  printTable();
  printInfo(game);
  printField(game);

  move(FIELD_Y + 1, FIELD_X * 2 + INFO_X * 2 + 3);
}

void print_pause(GameParams_t *game) {
  clear();
  printGame(game);
  mvprintw(FIELD_Y / 2 - 3, FIELD_X / 2, "...PAUSE...");
  mvprintw(FIELD_Y / 2 - 2, FIELD_X / 3 + 1, "Press SPACE to");
  mvprintw(FIELD_Y / 2 - 1, FIELD_X / 2 + 3, "start.");
  move(FIELD_Y + 1, FIELD_X * 2 + INFO_X * 2 + 3);
}
