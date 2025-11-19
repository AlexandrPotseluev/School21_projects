#ifndef CONSOLEVIEWSNAKE_H
#define CONSOLEVIEWSNAKE_H

#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <term.h>
#include "snakeController.h"

namespace s21 {

    class ConsoleViewSnake
{
    private:
        SnakeController *controller;
        bool break_flag = true;

    public:
        ConsoleViewSnake(SnakeController *c):controller(c){
            initGuiSnake();
        };

        void initGuiSnake(){
            setlocale(LC_ALL, "");
            initscr();
            start_color();
            init_pair(1, COLOR_CYAN, COLOR_CYAN);
            init_pair(2, COLOR_BLUE, COLOR_BLUE);
            init_pair(3, COLOR_RED, COLOR_RED);
            curs_set(0);
            cbreak();
            noecho();
            keypad(stdscr, true);
            timeout(controller->getSpeed());
        };

        void keyUserEvent(int user_act){
            //Turn on event
            if(user_act) controller->setHoldTrue();

            //Start or exit event
            if(user_act == 's') controller->setAction(Start);
            if(user_act == 'q') controller->setAction(Terminate);

            //Move
            if(user_act == KEY_LEFT && controller->getDirection() != RIGHT) controller->setAction(Left);
            if(user_act == KEY_RIGHT && controller->getDirection() != LEFT) controller->setAction(Right);
            if(user_act == KEY_UP && controller->getDirection() != DOWN) {controller->setAction(Up);}
            if(user_act == KEY_DOWN && controller->getDirection() != UP) controller->setAction(Down);

            //Change speed
            if (user_act == '+'){
                controller->setAction(Action);
                timeout(controller->getSpeed());
            }

            //Pause
            if (user_act == ' ') controller->setAction(Pause);
        };

        void printInfoSnake(){
            mvprintw(2, X_FIELD * 2 + 14, "%d", controller->getScore());
            mvprintw(4, X_FIELD * 2 + 14, "%d", controller->getHighScore());
            mvprintw(6, X_FIELD * 2 + 14, "%d", controller->getLevel());
            mvprintw(8, X_FIELD * 2 + 14, "%d", controller->getSpeed());
        }

        void printPause(){
            clear();
            printTable();
            mvprintw(Y_FIELD / 2 - 3, X_FIELD / 2, "...PAUSE...");
            mvprintw(Y_FIELD / 2 - 2, X_FIELD / 3 + 1, "Press 's' to");
            mvprintw(Y_FIELD / 2 - 1, X_FIELD / 2 + 3, "start.");
            move(Y_FIELD + 1, X_FIELD * 2 + X_FIELD * 2 + 3);
        }

        void printTable(){
            clear();
            mvhline(0, 0, ACS_HLINE, X_FIELD * 2 + X_FIELD * 2 + 2);
            mvhline(Y_FIELD + 1, 0, ACS_HLINE, X_FIELD * 2 + X_FIELD * 2 + 2);
            mvhline(Y_FIELD - 6, X_FIELD * 2 + 2, ACS_HLINE, X_FIELD * 2);
            mvvline(1, 0, ACS_VLINE, Y_FIELD);
            mvvline(1, X_FIELD * 2 + 1, ACS_VLINE, Y_FIELD);
            mvvline(1, X_FIELD * 2 + X_FIELD * 2 + 2, ACS_VLINE, Y_FIELD);
            mvaddch(0, 0, ACS_ULCORNER);
            mvaddch(0, X_FIELD * 2 + X_FIELD * 2 + 2, ACS_URCORNER);
            mvaddch(Y_FIELD + 1, 0, ACS_LLCORNER);
            mvaddch(Y_FIELD + 1, X_FIELD * 2 + X_FIELD * 2 + 2, ACS_LRCORNER);
            mvaddch(0, X_FIELD * 2 + 1, ACS_TTEE);
            mvaddch(Y_FIELD + 1, X_FIELD * 2 + 1, ACS_BTEE);

            mvprintw(2, X_FIELD * 2 + 3, "SCORE:");
            mvprintw(4, X_FIELD * 2 + 3, "HIGH SCORE:");
            mvprintw(6, X_FIELD * 2 + 3, "LEVEL:");
            mvprintw(8, X_FIELD * 2 + 3, "SPEED:");
            mvprintw(10, X_FIELD * 2 + 3, "NEXT:");
            mvprintw(Y_FIELD - 5, X_FIELD * 2 + 2, "SPACE - Pause game");
            mvprintw(Y_FIELD - 4, X_FIELD * 2 + 2, "arrL - Move left");
            mvprintw(Y_FIELD - 3, X_FIELD * 2 + 2, "arrR - Move right");
            mvprintw(Y_FIELD - 2, X_FIELD * 2 + 2, "arrD - Move down");
            mvprintw(Y_FIELD - 1, X_FIELD * 2 + 2, "a - Rotate");
            mvprintw(Y_FIELD - 0, X_FIELD * 2 + 2, "q - Exit game");
        }

        void printGameOverScreen(){
            clear();
            printTable();
            mvprintw(Y_FIELD / 2 - 3, X_FIELD / 2, "GAME OVER!");
            mvprintw(Y_FIELD / 2 - 2, X_FIELD / 2, "Press 's'");
            mvprintw(Y_FIELD / 2 - 1, X_FIELD / 2, "to start");
            mvprintw(Y_FIELD / 2, X_FIELD / 2, "or 'q'");
            mvprintw(Y_FIELD / 2 + 1, X_FIELD / 2, "to finish");
            move(Y_FIELD + 1, X_FIELD * 2 + X_FIELD * 2 + 3);
        }

        void printStartScreen() {
            clear();
            printTable();
            mvprintw(Y_FIELD / 2, X_FIELD / 2, "Are you ready?");
            mvprintw(Y_FIELD / 2 + 1, X_FIELD / 2, "Press 's'");
            move(Y_FIELD + 1, X_FIELD * 2 + X_FIELD * 2 + 3);
        }

        void printOneBlock(int i, int j, int block) {
            attron(COLOR_PAIR(block));
            mvaddch(i + 1, 1 + j * 2, ' ');
            mvaddch(i + 1, 1 + j * 2 + 1, ' ');
            attroff(COLOR_PAIR(block));
        }

        void printFieldSnake() {
            for (int i = 0; i < Y_FIELD; i++){
                for (int j = 0; j < X_FIELD; j++){
                    if (controller->getElemField(i,j) > 0)
                    printOneBlock(i, j, 2);
                    if(controller->getElemField(i,j) < 0)
                    printOneBlock(i, j, 3);
                }
            }
        }

        void startEventLoop(){
            
            while (break_flag) {
                if (controller->getState() == EXIT) break_flag = false;
                if (controller->getState() == PAUSE) printPause();
                if (controller->getState() == GAMEOVER) printGameOverScreen();
                if (controller->getState() == START) printStartScreen();
                if (controller->getState() == MOVE ||
                controller->getState() == START ||
                controller->getState() == PAUSE || 
                controller->getState() == GAMEOVER) keyUserEvent(getch());
                controller->updateController();
                timeout(controller->getSpeed());
                printTable();
                printInfoSnake();
                printFieldSnake();
            }
            endwin();
            del_curterm(cur_term);
            controller->delController();

        }
    };
}

#endif