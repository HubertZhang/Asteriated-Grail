#ifndef BUTTERFLYDANCER_H
#define BUTTERFLYDANCER_H
#include"player.h"

class Butterflydancer : public Player
{
    Q_OBJECT

    int cocoonNumber;
    int pupa;
    bool Morale;
    set<int> cocoon;
    void cocoonIncrease(int);
    void cocoonDecrease(int * idOfCard, int number, bool cansee = false);
    void pupaChange(int);
    void magicOne();
    void magicTwo();
    void magicThree();
public:
    Butterflydancer(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void changeCardLimit2(int amount);
    void characterConnect();
    void takeDamage(int damage, int kind);
    void start();
public slots:
    void skillone(int target, int& damage, int kind);
    void skilltwo(int,int&);
};

#endif // BUTTERFLYDANCER_H
