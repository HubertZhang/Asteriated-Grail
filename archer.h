#ifndef ARCHER_H
#define ARCHER_H
#include"player.h"

class Archer : public Player
{
     Q_OBJECT
    int skill;
    int target;
    void magicOne();//闪光陷阱
    void magicTwo();//狙击
public:
    Archer(Server* server,int order,int teamNumber,int character);
    void magicAction();
    void normalAttack();
    void end();

signals:

public slots:
    void skillone(int order);
};

#endif // ARCHER_H
