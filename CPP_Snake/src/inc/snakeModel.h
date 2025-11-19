#ifndef SNAKEMODEL_H
#define SNAKEMODEL_H

#include <cstdio>
#include <cstdlib>
#include <iostream>

#define Y_FIELD 20
#define X_FIELD 10
#define Y_INFO 4
#define X_INFO 2
#define APPLE_VALUE -1
#define SNAKESTARTSIZE 4
#define DELAY 700
#define MAXSPEED 200
#define MAXSNAKESIZE 200
#define SPEEDSTEP 50
#define SNAKE_HIGHSCORE "snake_high_score"

namespace s21 {

typedef enum {
    Start = 0,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

typedef enum {
    RIGHT = 0,
    LEFT,
    UP,
    DOWN
} Direction;

typedef enum {
    START = 0,
    SPAWN,
    MOVE,
    PAUSE,
    SHIFT,
    ATTACH,
    GAMEOVER,
    EXIT
} SnakeState;

class SnakeModel {
private:
    GameInfo_t snake;
    int snakeSize = SNAKESTARTSIZE;
    int y_snake = Y_FIELD / 2;
    int x_snake = X_FIELD / 2;
    int apple_pos = -1;

public:
    UserAction_t action = Right;
    Direction direction = RIGHT;
    SnakeState state = START;
    bool hold = false;

    GameInfo_t updateCurrentState();
    void userInput(UserAction_t action, bool hold);
    void initGame();
    void getState();
    void readRecord();
    int getRandomEmptyCell(int emptyCells);
    int getAppleIndex();
    void addApple();
    void eatApple();
    bool checkCollision();
    void scoreCount();
    void writeRecord();
    void changeSpeed();
    void moveSnake();
    void shiftSnake();
    int getDataField(int i, int j);
    int getInfoScore();
    int getInfoHighScore();
    int getInfoSpeed();
    int getInfoLevel();
    int getSnakeSize();
    void setSnakeX(int newSnakeX){x_snake = newSnakeX;};
    void setSnakeY(int newSnakeY){y_snake = newSnakeY;};
    void setApplePosition(int appPos){apple_pos = appPos;};
    void setSnakeSize(int newSize){snakeSize = newSize;};
    void setScore(int newScore){snake.score = newScore;};
    void setLevel(int newLevel){snake.level = newLevel;};
    int getInfoPause();
    void setInfoPauseOn();
    void setInfoPauseOff();
    void startSnakeSettings();
    void addSnakeToFiled();
    void clearInfo();
    void clearField();
    void delModel();
    SnakeModel();
    ~SnakeModel(){delModel();}
};

}  // namespace s21
#endif // SNAKEMODEL_H
