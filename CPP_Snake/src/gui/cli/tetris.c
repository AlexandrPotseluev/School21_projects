#include "tetris.h"

int main() {
  srand(time(NULL));
  initGui();
  game_loop();
  return 0;
}

void game_loop(void) {
  GameParams_t *tetris = initGame();
  int user_act = 0;
  bool break_flag = true;
  bool hold = false;
  updateParams(tetris);

  while (break_flag) {
    if (tetris->stage == EXIT) break_flag = false;

    userInput(get_action(user_act, hold), tetris);

    if (tetris->stage == PAUSE) print_pause();

    if (tetris->stage == GAMEOVER) printGameOverScreen();

    if (tetris->stage == START) printStartScreen();

    if (tetris->stage == MOVING || tetris->stage == START ||
        tetris->stage == PAUSE || tetris->stage == GAMEOVER)
      user_act = getch();

    updateCurrentState();
    printGame(tetris);
  }
  endwin();
  del_curterm(cur_term);

  freeGame(tetris);
}