#include "consoleViewSnake.h"

int main() {
  s21::SnakeModel m;
  s21::SnakeController c(&m);
  s21::ConsoleViewSnake w(&c);
  w.startEventLoop();
  return 0;
}