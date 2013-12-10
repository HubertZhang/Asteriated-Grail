#ifndef ELEMENTALIST_H
#define ELEMENTALIST_H
#include"player.h"

class Elementalist : public Player
{
     Q_OBJECT

    int element;
    int thepoison;
    int thedamage;
    int isdiscard;
    void magicOne();
    void magicTwo();
    void magicThree();
    void elementIncrease();
public:
    Elementalist(Server *server, int order, int teamNumber, int character);
    void magicAction();   
    void normalMagic();
    void beMagicMissileAttack(int damage, bool *missileCycle);

public slots:
    void skillone(int order,int);
    void skilltwo(int);
    void skillthree(int damage);
};

#endif // ELEMENTALIST_H
