#ifndef ARBITER_H
#define ARBITER_H
#include"player.h"

class Arbiter : public Player
{
     void magicOne();
     void magicTwo();
     void magicThree();
     void trialIncrease(int number);
     int trial;
public:
    Arbiter(Server *server, int order, int teamNumber, int character);
    void activate();
    void magicAction();
    void characterConnect();
    bool canActivate();
    void changeCardLimit(int amount);
    void start();
    void takeDamage(int damage,int kind);
};

#endif // ARBITER_H
