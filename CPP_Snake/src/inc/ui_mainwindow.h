/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

namespace s21 {

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QTableView *ViewField;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QTableView *ViewInfo;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *s21__MainWindow)
    {
        if (s21__MainWindow->objectName().isEmpty())
            s21__MainWindow->setObjectName("s21__MainWindow");
        s21__MainWindow->resize(800, 600);
        s21__MainWindow->setMinimumSize(QSize(800, 600));
        centralwidget = new QWidget(s21__MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 10, 440, 531));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        ViewField = new QTableView(gridLayoutWidget);
        ViewField->setObjectName("ViewField");
        ViewField->horizontalHeader()->setMinimumSectionSize(1);
        ViewField->verticalHeader()->setMinimumSectionSize(1);

        gridLayout->addWidget(ViewField, 0, 0, 1, 2);

        gridLayoutWidget_2 = new QWidget(centralwidget);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(450, 10, 331, 471));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        ViewInfo = new QTableView(gridLayoutWidget_2);
        ViewInfo->setObjectName("ViewInfo");

        gridLayout_2->addWidget(ViewInfo, 2, 1, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(430, 480, 331, 61));
        QFont font;
        font.setFamilies({QString::fromUtf8("Ubuntu")});
        font.setPointSize(10);
        font.setBold(false);
        font.setItalic(false);
        font.setStrikeOut(false);
        font.setKerning(true);
        label->setFont(font);
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);
        s21__MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(s21__MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 779, 26));
        menubar->setDefaultUp(false);
        s21__MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(s21__MainWindow);
        statusbar->setObjectName("statusbar");
        s21__MainWindow->setStatusBar(statusbar);

        retranslateUi(s21__MainWindow);

        QMetaObject::connectSlotsByName(s21__MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *s21__MainWindow)
    {
        s21__MainWindow->setWindowTitle(QCoreApplication::translate("s21::MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("s21::MainWindow", "Settings:\n"
"s - start, q - esc, space - pause,\n"
"Up arr, Down arr, Left arr, Right arr,", nullptr));
    } // retranslateUi

};

} // namespace s21

namespace s21 {
namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui
} // namespace s21

#endif // UI_MAINWINDOW_H
