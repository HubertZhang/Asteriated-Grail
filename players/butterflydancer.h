#ifndef BUTTERFLYDANCER_H
#define BUTTERFLYDANCER_H
#include"player.h"

class Butterflydancer : public Player
{
    int cocoonNumber;
    int pupa;
    set<int> cocoon;
    void cocoonIncrease(int);
    void cocoonDecrease(int * idOfCard, int number);
    void pupaChange(int);
    void magicOne();
    void magicTwo();
    void magicThree();
public:
    Butterflydancer(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void changeCardLimit2(int amount);
};

#endif // BUTTERFLYDANCER_H
