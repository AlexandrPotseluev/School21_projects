#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTimer>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QDebug>
#include <QLabel>

#include "snakeController.h"

#define ENTER_BUTTON Qt::Key_Enter
#define ESC_BUTTON Qt::Key_Enter

namespace s21 {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(SnakeController *c, QWidget *parent = nullptr);
    ~MainWindow();
    void initView();
    void initViewInfo();
    void printField();
    void snakeLoop();
    void initTextWidget();

public slots:
    void updateView();

private:
    Ui::MainWindow *ui;
    SnakeController *controller;
    QStandardItemModel* gameField;
    QStandardItemModel* gameInfo;
    QTimer *timer;
    QLabel *text;

protected:
    void keyPressEvent(QKeyEvent *);

};

} // namespace s21
#endif // MAINWINDOW_H
