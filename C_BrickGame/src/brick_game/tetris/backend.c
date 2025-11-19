#include "backend.h"

int **loc_blocks_memory(int m, int n) {
  int **b = (int **)calloc(m * n + m, m * n * sizeof(int) + m * sizeof(int *));
  int *ptr = (int *)(b + m);

  for (int i = 0; i < m; i++) b[i] = ptr + n * i;

  return b;
}

void copyBlocks(int **dest_fig, int **src_fig) {
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++) dest_fig[i][j] = src_fig[i][j];
}

int **newFigure() {
  int fig_templates[FIGURE_COUNT][FIGURE_SIZE * FIGURE_SIZE] = {
      {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

      {0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},

      {0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

      {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},

      {1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

      {1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},

      {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

  int figure_type = rand() % 7;
  int **blocks = loc_blocks_memory(FIGURE_SIZE, FIGURE_SIZE);
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++)
      blocks[i][j] = fig_templates[figure_type][i * FIGURE_SIZE + j];

  return blocks;
}

Figure_t *initFigure() {
  Figure_t *figure = (Figure_t *)malloc(sizeof(Figure_t));
  figure->x = 0;
  figure->y = 0;
  figure->blocks = loc_blocks_memory(FIGURE_SIZE, FIGURE_SIZE);
  return figure;
}

void freeFigure(Figure_t *figure) {
  if (figure) {
    figure->x = 0;
    figure->y = 0;
    if (figure->blocks) free(figure->blocks);
    free(figure);
  }
}

GameInfo_t *initInfo() {
  GameInfo_t *info = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  info->field = loc_blocks_memory(FIELD_Y, FIELD_X);
  info->next = newFigure();
  info->high_score = 0;
  info->score = 0;
  info->speed = 0;
  info->pause = 0;
  info->level = 0;

  return info;
}

void freeInfo(GameInfo_t *info) {
  if (info) {
    if (info->field) free(info->field);
    if (info->next) free(info->next);
    free(info);
  }
}

void plantFigure(GameParams_t *game) {
  Figure_t *fig = game->figure;
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++)
      if (fig->blocks[i][j]) {
        int fx = fig->x + j;
        int fy = fig->y + i;
        game->data->field[fy][fx] = fig->blocks[i][j];
      }
}

GameParams_t *initGame() {
  GameParams_t *game = (GameParams_t *)malloc(sizeof(GameParams_t));
  game->data = initInfo();
  game->figure = initFigure();
  game->stage = 0;
  game->erased_lines = 0;
  game->fps = FPS;

  return game;
}

void freeGame(GameParams_t *game) {
  if (game) {
    freeInfo(game->data);
    freeFigure(game->figure);
    free(game);
  }
}

void moveDown(GameParams_t *game) { game->figure->y++; }
void moveUp(GameParams_t *game) { game->figure->y--; }

void moveLeft(GameParams_t *game) { game->figure->x--; }

void moveRight(GameParams_t *game) { game->figure->x++; }

int collide(GameParams_t *game) {
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++)
      if (game->figure->blocks[i][j]) {
        int fx = game->figure->x + j;
        int fy = game->figure->y + i;
        if (fx < 0 || fx >= FIELD_X || fy < 0 || fy >= FIELD_Y) return 1;
        if (game->data->field[fy][fx]) return 1;
      }

  return 0;
}

int checkLine(int i, int **field) {
  for (int j = 0; j < FIELD_X; j++)
    if (field[i][j] == 0) return 0;
  return 1;
}

void dropLine(int i, int **field) {
  if (i == 0) {
    for (int j = 0; j < FIELD_X; j++) field[i][j] = 0;
  } else {
    for (int k = i; k > 0; k--)
      for (int j = 0; j < FIELD_X; j++) field[k][j] = field[k - 1][j];
  }
}

void score_and_levels(GameParams_t *game) {
  if (game->erased_lines < 5) {
    game->data->score = 2 * game->data->score + 100;
    if (game->data->score >= 700) {
      game->data->level++;
      game->data->speed++;
    }
  } else {
    game->data->score = game->data->score + 700;
    game->data->level++;
    game->data->speed++;
  }

  if (game->data->score > game->data->high_score)
    game->data->high_score = game->data->score;

  FILE *high_score = fopen(DATAFILE_PATH, "wb+");
  if (high_score) {
    fprintf(high_score, "%d\n", game->data->high_score);
    fclose(high_score);
  }
}

void eraseLine(GameParams_t *game) {
  for (int i = FIELD_Y - 1; i >= 0; i--)
    while (checkLine(i, game->data->field)) {
      dropLine(i, game->data->field);
      game->erased_lines++;
      score_and_levels(game);
    }
}

Figure_t *rotateFigure(GameParams_t *game) {
  Figure_t *rot_figure = initFigure();
  Figure_t *cur_figure = game->figure;
  rot_figure->x = cur_figure->x;
  rot_figure->y = cur_figure->y;
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++)
      rot_figure->blocks[i][j] = cur_figure->blocks[j][FIGURE_SIZE - 1 - i];
  return rot_figure;
}

void on_start_stage(UserAction_t act, GameParams_t *game) {
  switch (act) {
    case Start:
      FILE *high_score = fopen(DATAFILE_PATH, "r");
      if (high_score) {
        fscanf(high_score, "%d\n", &game->data->high_score);
        fclose(high_score);
      } else
        game->data->high_score = 0;
      game->stage = SPAWN;
      break;
    case Terminate:
      game->stage = EXIT;
      break;
    default:
      game->stage = START;
      break;
  }
}

void on_gameover_stage(UserAction_t act, GameParams_t *game) {
  switch (act) {
    case Start:
      GameInfo_t *new_data = initInfo();
      Figure_t *new_figure = initFigure();
      freeInfo(game->data);
      freeFigure(game->figure);
      game->data = new_data;
      game->figure = new_figure;
      game->stage = START;
      break;
    case Terminate:
      game->stage = EXIT;
      break;
    default:
      game->stage = GAMEOVER;
      break;
  }
}

void on_spawn_stage(GameParams_t *game) {
  Figure_t *new_fig = initFigure();
  new_fig->x = FIGSTART_X;
  new_fig->y = FIGSTART_Y;
  copyBlocks(new_fig->blocks, game->data->next);

  freeFigure(game->figure);
  game->figure = new_fig;

  if (collide(game))
    game->stage = GAMEOVER;
  else {
    free(game->data->next);
    game->data->next = newFigure();
    game->stage = SHIFT;
  }
}

void on_shift_stage(GameParams_t *game) {
  if (game->fps <= 0) {
    game->fps = FPS;
    moveDown(game);
  } else if (game->data->speed == 0)
    game->fps--;
  else
    game->fps -= (1 + game->data->speed / 2);

  if (collide(game)) {
    moveUp(game);
    plantFigure(game);
    game->stage = ATTACH;
  } else
    game->stage = MOVING;
}

void on_attach_stage(GameParams_t *game) {
  eraseLine(game);

  if (game->data->level >= MAX_LEVEL)
    game->stage = GAMEOVER;
  else
    game->stage = SPAWN;
}

void on_moving_stage(UserAction_t act, GameParams_t *game) {
  switch (act) {
    case Action:
      Figure_t *rot = rotateFigure(game);
      Figure_t *cur = game->figure;
      game->figure = rot;
      if (collide(game)) {
        game->figure = cur;
        freeFigure(rot);
      } else
        freeFigure(cur);
      break;
    case Down:
      moveDown(game);
      if (collide(game)) moveUp(game);
      break;
    case Right:
      moveRight(game);
      if (collide(game)) moveLeft(game);
      break;
    case Left:
      moveLeft(game);
      if (collide(game)) moveRight(game);
      break;
    case Terminate:
      game->stage = EXIT;
      break;
    case Up:
      game->stage = SHIFT;
      break;
    case Pause:
      game->stage = PAUSE;
      break;
    default:
      break;
  }
}

void on_pause_stage(UserAction_t act, GameParams_t *game) {
  switch (act) {
    case Pause:
      game->stage = MOVING;
      break;
    case Terminate:
      game->stage = EXIT;
      break;
    default:
      break;
  }
}

void get_stage(UserAction_t action, GameParams_t *game) {
  switch (game->stage) {
    case START:
      on_start_stage(action, game);
      break;
    case SPAWN:
      on_spawn_stage(game);
      break;
    case MOVING:
      on_moving_stage(action, game);
      break;
    case SHIFT:
      on_shift_stage(game);
      break;
    case ATTACH:
      on_attach_stage(game);
      break;
    case GAMEOVER:
      on_gameover_stage(action, game);
      break;
    case PAUSE:
      on_pause_stage(action, game);
      break;
    default:
      break;
  }
}

UserAction_t get_action(int user_input, bool hold) {
  UserAction_t action = Up;

  switch (user_input) {
    case ENTER_KEY:
      action = Start;
      break;
    case 'a':
      if (!hold) {
        action = Action;
        hold = true;
      } else
        hold = false;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case KEY_LEFT:
      if (!hold) {
        action = Left;
        hold = true;
      } else
        hold = false;
      break;
    case KEY_RIGHT:
      if (!hold) {
        action = Right;
        hold = true;
      } else
        hold = false;
      break;
    case ESCAPE:
      action = Terminate;
      break;
    case ' ':
      action = Pause;
      break;
    default:
      action = Up;
      break;
  }

  return action;
}

GameInfo_t updateCurrentState(void) {
  GameParams_t *params = updateParams(NULL);
  return *params->data;
}

GameParams_t *updateParams(GameParams_t *params) {
  static GameParams_t *data;
  if (params != NULL) data = params;
  return data;
}

void userInput(UserAction_t action, GameParams_t *game) {
  get_stage(action, game);
}