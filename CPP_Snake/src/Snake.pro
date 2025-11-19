QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = desktopSnake

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../gui/desktop/main.cpp \
    ../../gui/desktop/mainwindow.cpp \
    ../../brick_game/snake/snakeModel.cpp
    

HEADERS += \
    ../../inc/mainwindow.h \
    ../../inc/ui_mainwindow.h \
    ../../inc/snakeController.h \
    ../../inc/snakeModel.h

#FORMS += \
#    mainwindow.ui

RESOURCES += snake_high_score

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
