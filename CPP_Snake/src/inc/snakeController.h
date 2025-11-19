#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H
#include "snakeModel.h"

namespace s21 {
class SnakeController {
private:
    SnakeModel *model;

public:
    SnakeController(SnakeModel *m) : model(m) {}
    void delController() {model->delModel();}
    void updateController() {model->updateCurrentState();}
    int getElemField(int i, int j) const {return model->getDataField(i,j);}
    int getScore() const {return model->getInfoScore();}
    int getHighScore() const{return model->getInfoHighScore();}
    int getLevel() const {return model->getInfoLevel();}
    int getSpeed() const {return model->getInfoSpeed();}
    int getPause() const {return model->getInfoPause();}
    int getHold() {return model->hold;}
    Direction getDirection() const {return model->direction;}
    UserAction_t getAction() const {return model->action;}
    SnakeState getState() const {return model->state;}
    void setAction(UserAction_t newAction){model->action = newAction;}
    void setHoldTrue(){model->hold = true;}
    void setHoldFalse(){model->hold = false;}
    void setPauseOn(){model->setInfoPauseOn();}
    void setPauseOff(){model->setInfoPauseOff();}
};
}  // namespace s21

#endif // SNAKECONTROLLER_H
