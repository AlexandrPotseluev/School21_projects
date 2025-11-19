#ifndef TET_BACK_H
#define TET_BACK_H

#include "def_obj.h"

//INIT GAME
GameInfo_t *initInfo();
Figure_t *initFigure();
GameParams_t *initGame();
GameInfo_t updateCurrentState();
GameParams_t *updateParams(GameParams_t *new_param);

//GAME
Figure_t *rotateFigure(GameParams_t *game);
int **loc_blocks_memory(int m, int n);
int **newFigure();
void freeFigure(Figure_t *figure);
void freeInfo(GameInfo_t *info);
void copyBlocks(int **dest_fig, int **src_fig);
void plantFigure(GameParams_t *game);
void freeGame(GameParams_t *game);
void moveDown(GameParams_t *game);
void moveUp(GameParams_t *game);
void moveLeft(GameParams_t *game);
void moveRight(GameParams_t *game);
int collide(GameParams_t *game);
int checkLine(int i, int **field);
void dropLine(int i, int **field);
void score_and_levels(GameParams_t *game);
void eraseLine(GameParams_t *game);

//ACTION
void userInput(UserAction_t action, GameParams_t *game);
void get_stage(UserAction_t act, GameParams_t *game);
UserAction_t get_action(int user_input, bool hold);
void on_start_stage(UserAction_t act, GameParams_t *game);
void on_gameover_stage(UserAction_t act, GameParams_t *game);
void on_moving_stage(UserAction_t act, GameParams_t *game);
void on_spawn_stage(GameParams_t *game);
void on_shift_stage(GameParams_t *game);
void on_attach_stage(GameParams_t *game);
void on_pause_stage(UserAction_t act, GameParams_t *game);

#endif