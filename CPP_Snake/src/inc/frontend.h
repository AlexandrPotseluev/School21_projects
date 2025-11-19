#ifndef TET_FRONT_H
#define TET_FRONT_H

#include "def_obj.h"

//GUI
void initGui();
void printOneBlock(int i, int j, int block);
void printTable(void);
void printStartScreen();
void printGameOverScreen();
void printInfo(GameParams_t *game);
void printField(GameParams_t *game);
void printGame(GameParams_t *game);
void print_pause();

#endif