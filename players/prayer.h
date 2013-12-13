#ifndef PRAYER_H
#define PRAYER_H
#include"player.h"

class Prayer : public Player
{
    Q_OBJECT

    void magicOne();
    void magicTwo();
    void magicThree();
    void magicFour();
    bool combo;
    int mark;
    void markChange(int);

    int damageIncrease[6];
    int actionIncrease[6];
public:
    Prayer(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void activate();
    bool canActivate();
    void normalAttack();
    void characterConnect();
public slots:
    void skillone(int order, int, int& damage);
    void skilltwo(int order,int);
    void skillthree(int order, int card);

};

#endif // PRAYER_H
