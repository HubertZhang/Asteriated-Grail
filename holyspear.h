#ifndef HOLYSPEAR_H
#define HOLYSPEAR_H
#include"player.h"

class Holyspear : public Player
{
    int skill;
    void magicOne();
    void magicTwo();
    void magicThree();
public:
    Holyspear(Server *server, int order, int teamNumber, int character);
    void normalAttack();
    void magicAction();
    void headOn(int chainLength);
};

#endif // HOLYSPEAR_H
