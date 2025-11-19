#ifndef TETRIS_DEF_H
#define TETRIS_DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <term.h>
#include "def_obj.h"

#define FIELD_X 10
#define FIELD_Y 20
#define INFO_X 10
#define INFO_Y 20
#define FIGURE_SIZE 4
#define FIGURE_COUNT 7
#define FIGSTART_X (FIELD_X / 2 - FIGURE_SIZE / 2)
#define FIGSTART_Y 0
#define MAX_LEVEL 10
#define ESCAPE 27
#define ENTER_KEY 10
#define TIMEOUT_INIT 800
#define DATAFILE_PATH "./build/tetris_score"
#define FPS 30
#define TIMEOUT 50

typedef struct
{
    int x;
    int y;
    int **blocks;
} Figure_t;

typedef enum
{
    START = 0,
    SPAWN,
    MOVING,
    SHIFT,
    ATTACH,
    PAUSE,
    GAMEOVER,
    EXIT
} GameStage_t;

typedef enum
{
    Start = 0,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct
{
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

typedef struct
{
    GameInfo_t *data;
    Figure_t *figure;
    GameStage_t stage;
    int erased_lines;
    int fps;
} GameParams_t;

#endif