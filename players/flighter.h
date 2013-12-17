#ifndef FLIGHTER_H
#define FLIGHTER_H
#include"player.h"

class Flighter : public Player
{
    Q_OBJECT

    int grudge;
    int skill;
    int target;
    int thetarget;
    void grudgeChange(int number);
public:
    Flighter(Server *server, int order, int teamNumber, int character);
    void normalAttack();
    void countDamage(int damage, int kind);
    void activate();
    void magicAction();
    void beforeAction();
    void headOn(int chainLength);
    bool canActivate();

public slots:
    void skillone(int order);
};

#endif // FLIGHTER_H
