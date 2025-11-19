#include "../../inc/mainwindow.h"

#include "../../inc/ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(SnakeController *cl, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller(cl) {
  ui->setupUi(this);
  ui->centralwidget->setAttribute(Qt::WA_DeleteOnClose, true);
  ui->centralwidget->setFocusPolicy(Qt::StrongFocus);

  initView();
  initViewInfo();
  initTextWidget();
  printField();
  snakeLoop();
}

void MainWindow::snakeLoop() {
  timer = new QTimer(this);
  timer->setInterval(controller->getSpeed());
  connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));
  timer->start();
}

void MainWindow::updateView() {
  controller->updateController();
  gameInfo->item(0, 1)->setData(QVariant(controller->getScore()),
                                Qt::DisplayRole);
  gameInfo->item(1, 1)->setData(QVariant(controller->getHighScore()),
                                Qt::DisplayRole);
  gameInfo->item(2, 1)->setData(QVariant(controller->getLevel()),
                                Qt::DisplayRole);
  gameInfo->item(3, 1)->setData(QVariant(controller->getSpeed()),
                                Qt::DisplayRole);

  if (controller->getState() == EXIT) close();
  if (controller->getState() == START) {
    text->setText("<p><b>PRESS<b><p>'s' - start game<p>'q' - exit!");
    text->show();
  } else if (controller->getState() == GAMEOVER) {
    text->setText("<p>GAMEOVER!<p><b>PRESS<b><p>'s' - new game<p>'q' - exit!");
    text->show();
  } else if (controller->getState() == PAUSE) {
    text->setText("<p>PAUSE!<p><b>PRESS<b><p>'s' to continue");
    text->show();
  } else
    text->hide();

  printField();
  controller->setHoldFalse();
}

void MainWindow::initView() {
  gameField = new QStandardItemModel(Y_FIELD, X_FIELD);

  for (int i = 0; i < Y_FIELD; i++)
    for (int j = 0; j < X_FIELD; j++)
      gameField->setItem(i, j, new QStandardItem(0));

  ui->ViewField->setModel(gameField);
  ui->ViewField->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->ViewField->setSelectionMode(QAbstractItemView::NoSelection);
  ui->ViewField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->ViewField->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->ViewField->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->ViewField->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->ViewField->horizontalHeader()->setDefaultSectionSize(1);
  ui->ViewField->setMinimumHeight(500);
  ui->ViewField->setMinimumWidth(400);
  ui->ViewField->setShowGrid(false);

  // ui->ViewField->verticalHeader()->setVisible(false);
  // ui->ViewField->horizontalHeader()->setVisible(false);
}

void MainWindow::printField() {
  for (int i = 0; i < Y_FIELD; i++) {
    for (int j = 0; j < X_FIELD; j++) {
      if (controller->getElemField(i, j) > 0) {
        gameField->item(i, j)->setBackground(QBrush(Qt::green));
      } else if (controller->getElemField(i, j) < 0) {
        gameField->item(i, j)->setBackground(QBrush(Qt::red));
      } else {
        gameField->item(i, j)->setBackground(QBrush(Qt::white));
      }
    }
  }
}
void MainWindow::initTextWidget() {
  text = new QLabel(this);
  text->resize(200, 200);
  text->setAlignment(Qt::AlignCenter);
  text->move(125, 200);
  text->setStyleSheet("background-color: yellow;");
  text->hide();
}

void MainWindow::initViewInfo() {
  gameInfo = new QStandardItemModel(Y_INFO, X_INFO);
  gameInfo->setItem(0, 0, new QStandardItem("Score"));
  gameInfo->setItem(1, 0, new QStandardItem("High Score"));
  gameInfo->setItem(2, 0, new QStandardItem("Level"));
  gameInfo->setItem(3, 0, new QStandardItem("Speed"));
  for (int i = 0; i < Y_INFO; i++)
    gameInfo->setItem(i, 1, new QStandardItem(0));

  for (int i = 0; i < Y_INFO; i++) {
    for (int j = 0; j < X_INFO; j++) {
      gameInfo->item(i, j)->setTextAlignment(Qt::AlignCenter);
    }
  }

  ui->ViewInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->ViewInfo->setSelectionMode(QAbstractItemView::NoSelection);
  ui->ViewInfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->ViewInfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->ViewInfo->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->ViewInfo->verticalHeader()->setVisible(false);
  ui->ViewInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->ViewInfo->horizontalHeader()->setVisible(false);
  ui->ViewInfo->setMinimumHeight(500);
  ui->ViewInfo->setMinimumWidth(200);
  ui->ViewInfo->setFont(QFont("Times", 20));
  ui->ViewInfo->setShowGrid(false);
  ui->ViewInfo->setModel(gameInfo);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  // Turn on event
  if (event->key()) controller->setHoldTrue();

  // Start or exit
  if (event->key() == Qt::Key_S) {
    timer->start(controller->getSpeed());
    controller->setAction(Start);
  }
  if (event->key() == Qt::Key_Q) controller->setAction(Terminate);

  // Move
  if (event->key() == Qt::Key_Left && controller->getDirection() != RIGHT)
    controller->setAction(Left);
  if (event->key() == Qt::Key_Right && controller->getDirection() != LEFT)
    controller->setAction(Right);
  if (event->key() == Qt::Key_Up && controller->getDirection() != DOWN) {
    controller->setAction(Up);
  }
  if (event->key() == Qt::Key_Down && controller->getDirection() != UP)
    controller->setAction(Down);

  // Change speed
  if (event->key() == Qt::Key_Plus) {
    controller->setAction(Action);
    timer->setInterval(controller->getSpeed());
  }

  // Pause
  if (event->key() == Qt::Key_Space) controller->setAction(Pause);
}

MainWindow::~MainWindow() {
  controller->delController();
  if (gameField) delete gameField;
  if (gameInfo) delete gameInfo;
  if (timer) delete timer;
  if (text) {
    delete text;
  }
  delete ui;
}

}  // namespace s21
