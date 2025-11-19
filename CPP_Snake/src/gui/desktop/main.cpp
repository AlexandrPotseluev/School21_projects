#include <QApplication>

#include "../../inc/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::SnakeModel m;
  s21::SnakeController c(&m);
  s21::MainWindow w(&c);
  w.show();
  return a.exec();
}
