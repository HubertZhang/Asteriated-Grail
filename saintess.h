#ifndef SAINTESS_H
#define SAINTESS_H
#include "player.h"

class Saintess : public Player
{
    void magicOne();//治疗术
    void magicTwo();//治愈之光
    void magicThree();//圣疗
public:
    Saintess(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void normalAttack();
    void headOn(int);
    void activate();
    void changeCardLimit(int);
};

#endif // SAINTESS_H
