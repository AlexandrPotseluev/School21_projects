#include "../../inc/snakeModel.h"

namespace s21 {

SnakeModel::SnakeModel() { initGame(); };

void SnakeModel::initGame() {
  // Add field
  snake.next = nullptr;
  snake.field = new int *[Y_FIELD];
  for (int i = 0; i < Y_FIELD; i++) snake.field[i] = new int[X_FIELD];

  // Set field and info
  clearInfo();
  clearField();
  startSnakeSettings();
  addSnakeToFiled();
  action = Right;
  state = START;
  hold = false;
}

GameInfo_t SnakeModel::updateCurrentState() {
  getState();
  return snake;
}

void SnakeModel::getState() {
  if (state == START) {
    userInput(action, hold);
    if (action == Start) state = SPAWN;
  }
  if (state == SPAWN) {
    apple_pos = getAppleIndex();
    if (apple_pos == -1)
      state = GAMEOVER;
    else {
      addApple();
      state = MOVE;
    }
  }
  if (state == MOVE) {
    userInput(action, hold);
    if (action == Pause)
      state = PAUSE;
    else {
      moveSnake();
      if (checkCollision()) {
        snake.speed = DELAY;
        state = GAMEOVER;
      } else
        state = SHIFT;
    }
  }
  if (state == SHIFT) {
    if (snake.field[y_snake][x_snake] == APPLE_VALUE) {
      eatApple();
      scoreCount();
      shiftSnake();
      if (snakeSize == MAXSNAKESIZE)
        state = GAMEOVER;
      else
        state = SPAWN;
    } else {
      shiftSnake();
      state = MOVE;
    }
  }
  if (state == GAMEOVER) {
    userInput(action, hold);
  }
  if (state == PAUSE) {
    userInput(action, hold);
    if (action == Start) state = MOVE;
  }
}

void SnakeModel::userInput(UserAction_t action, bool hold) {
  if (hold == true) {
    if (action == Terminate) state = EXIT;
    if (action == Start && state == GAMEOVER) {
      clearInfo();
      clearField();
      snakeSize = SNAKESTARTSIZE;
      y_snake = Y_FIELD / 2;
      x_snake = X_FIELD / 2;
      startSnakeSettings();
      addSnakeToFiled();
      direction = RIGHT;
      action = Right;
      state = SPAWN;
    }
    if (state == MOVE) {
      if (action == Right) direction = RIGHT;
      if (action == Left) direction = LEFT;
      if (action == Up) direction = UP;
      if (action == Down) direction = DOWN;
    }
    if (action == Action) changeSpeed();
  }
}

void SnakeModel::readRecord() {
  FILE *hs = fopen(SNAKE_HIGHSCORE, "r");
  if (hs) {
    fscanf(hs, "%d\n", &snake.high_score);
    fclose(hs);
  }
}

void SnakeModel::writeRecord() {
  FILE *hs = fopen(SNAKE_HIGHSCORE, "wb+");
  if (hs) {
    fprintf(hs, "%d\n", snake.high_score);
    fclose(hs);
  }
}

int SnakeModel::getRandomEmptyCell(int emptyCells) {
  srand(time(NULL));
  int emptyCellIndex = 0;
  int emptyCellsList[emptyCells];
  int listIndex = 0;
  int count = 0;

  emptyCellsList[emptyCells] = {0};
  for (int i = 0; i < Y_FIELD; i++)
    for (int j = 0; j < X_FIELD; j++) {
      if (snake.field[i][j] == 0) {
        emptyCellsList[listIndex] = count;
        listIndex++;
      }
      count++;
    }
  emptyCellIndex = emptyCellsList[rand() % listIndex];
  return emptyCellIndex;
}

int SnakeModel::getAppleIndex() {
  int result = -1;
  int emptyCells = Y_FIELD * X_FIELD - snakeSize;
  if (emptyCells > 0) result = getRandomEmptyCell(emptyCells);
  return result;
}
void SnakeModel::addApple() {
  int y_apple = apple_pos / X_FIELD;
  int x_apple = apple_pos % X_FIELD;
  snake.field[y_apple][x_apple] = APPLE_VALUE;
}

void SnakeModel::eatApple() {
  snakeSize++;
  for (int i = 0; i < Y_FIELD; i++)
    for (int j = 0; j < X_FIELD; j++)
      if (snake.field[i][j] > 0) snake.field[i][j]++;
}

bool SnakeModel::checkCollision() {
  bool res = false;
  if ((x_snake > X_FIELD - 1) || (x_snake < 0) || (y_snake > Y_FIELD - 1) ||
      (y_snake < 0) || (snake.field[y_snake][x_snake] > 0))
    res = true;

  return res;
}

void SnakeModel::scoreCount() {
  snake.score++;

  if (snake.high_score < snake.score) {
    snake.high_score = snake.score;
    writeRecord();
  }

  if (snake.score > 0 && snake.score % 5 == 0 && snake.level < 10) {
    snake.level++;
    changeSpeed();
  }
}

void SnakeModel::changeSpeed() {
  if (snake.speed > MAXSPEED) snake.speed -= SPEEDSTEP;
}

void SnakeModel::moveSnake() {
  if (direction == RIGHT) x_snake++;
  if (direction == LEFT) x_snake--;
  if (direction == UP) y_snake--;
  if (direction == DOWN) y_snake++;
}

void SnakeModel::shiftSnake() {
  snake.field[y_snake][x_snake] = snakeSize + 1;
  for (int i = 0; i < Y_FIELD; i++)
    for (int j = 0; j < X_FIELD; j++)
      if (snake.field[i][j] > 0) snake.field[i][j]--;
}

int SnakeModel::getDataField(int i, int j) { return snake.field[i][j]; }
int SnakeModel::getInfoScore() { return snake.score; }
int SnakeModel::getInfoHighScore() { return snake.high_score; }
int SnakeModel::getInfoSpeed() { return snake.speed; }
int SnakeModel::getInfoPause() { return snake.pause; }
int SnakeModel::getInfoLevel() { return snake.level; }
int SnakeModel::getSnakeSize() { return snakeSize; }
void SnakeModel::setInfoPauseOn() { snake.pause = 1; }
void SnakeModel::setInfoPauseOff() { snake.pause = 0; }

void SnakeModel::startSnakeSettings() {
  snake.speed = DELAY;
  readRecord();
}

void SnakeModel::addSnakeToFiled() {
  for (int i = 0; i < snakeSize; i++)
    snake.field[y_snake][x_snake - i] = snakeSize - i;
}

void SnakeModel::clearInfo() {
  snake.score = 0;
  snake.high_score = 0;
  snake.level = 0;
  snake.speed = 0;
  snake.pause = 0;
}

void SnakeModel::clearField() {
  for (int i = 0; i < Y_FIELD; i++)
    for (int j = 0; j < X_FIELD; j++) snake.field[i][j] = 0;
}

void SnakeModel::delModel() {
  clearInfo();
  if (snake.field != nullptr) {
    for (int i = 0; i < Y_FIELD; i++) {
      delete[] snake.field[i];
    }
    delete[] snake.field;
  }
  snake.field = nullptr;
}

}  // namespace s21
