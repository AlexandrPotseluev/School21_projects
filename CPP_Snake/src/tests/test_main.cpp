#include <gtest/gtest.h>

#include "snakeController.h"

using namespace s21;
using namespace std;

// 1.Start position
TEST(Model, StartState) {
  SnakeModel m;
  EXPECT_EQ(m.state, 0);
  m.delModel();
};

TEST(Model, StateMove) {
  SnakeModel m;

  // Устанавливаем начальные параметры
  m.action = Start;
  m.hold = true;

  // Проверяем, что змея может двигаться
  m.moveSnake();

  // Убеждаемся, что нет столкновений
  ASSERT_FALSE(m.checkCollision());

  m.updateCurrentState();
  EXPECT_EQ(m.state, MOVE);

  m.delModel();
}

TEST(Model, AddApple) {
  SnakeModel m;
  int res = 0;
  m.state = SPAWN;
  m.updateCurrentState();
  for (int i = 0; i < Y_FIELD; i++) {
    for (int j = 0; j < X_FIELD; j++) {
      if(m.getDataField(i, j) < 0) res = m.getDataField(i, j);
    }
  }
  EXPECT_EQ(res, -1);

  m.delModel();
}

TEST(Model, ChangeDirection) {
  SnakeModel m;
  m.state = MOVE;
  m.userInput(Up,true);
  m.updateCurrentState();
  EXPECT_EQ(m.direction, UP);

  m.delModel();
}

TEST(Model, GameoverState) {
  SnakeModel m;
  SnakeController c(&m);
  m.action = Up;
  m.state = GAMEOVER;
  m.userInput(Start,true);
  m.updateCurrentState();
  EXPECT_EQ(c.getDirection(), RIGHT);
  EXPECT_EQ(c.getSpeed(), DELAY);

  m.delModel();
}

TEST(Model, Collision) {
  SnakeModel m;
  m.setSnakeY(0);
  m.setSnakeX(9);
  m.state = MOVE;
  m.updateCurrentState();
  ASSERT_TRUE(m.checkCollision());
  EXPECT_EQ(m.state,GAMEOVER);

  m.delModel();
}

TEST(Model, EatApple) {
  SnakeModel m;
  SnakeController c(&m);
  m.state = MOVE;
  m.setApplePosition(107);
  m.addApple();
  m.updateCurrentState();
  m.updateCurrentState();

  EXPECT_EQ(c.getState(), SPAWN);
  EXPECT_EQ(c.getScore(), 1);
  EXPECT_EQ(c.getHighScore(), 23);
  EXPECT_EQ(m.getSnakeSize(), 5);

  m.delModel();
}

TEST(Model, MaxSnakeSize_Check) {
  SnakeModel m;
  m.state = MOVE;
  m.setSnakeSize(199);
  m.setApplePosition(107);
  m.addApple();
  m.updateCurrentState();
  m.updateCurrentState();

  EXPECT_EQ(m.state, GAMEOVER);

  m.delModel();
}

TEST(Model, SpeedChangeScore) {
  SnakeModel m;
  m.setScore(4);
  m.state = MOVE;
  m.setApplePosition(107);
  m.addApple();
  m.updateCurrentState();
  m.updateCurrentState();

  EXPECT_EQ(m.getInfoSpeed(), DELAY - SPEEDSTEP);
  EXPECT_EQ(m.getInfoScore(), 5);

  m.delModel();
}

TEST(Model, SpeedChangeLevel) {
  SnakeModel m;
  m.setScore(4);
  m.setLevel(10);
  m.state = MOVE;
  m.setApplePosition(107);
  m.addApple();
  m.updateCurrentState();
  m.updateCurrentState();

  EXPECT_EQ(m.getInfoSpeed(), DELAY);
  EXPECT_EQ(m.getInfoScore(), 5);
  EXPECT_EQ(m.getInfoLevel(), 10);

  m.delModel();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// for (int i = 0; i < Y_FIELD; i++) {
//     for (int j = 0; j < X_FIELD; j++){
//       cout << c.getElemField(i,j);
//     }
//   cout << "\n";
//   }