#ifndef BLADEMASTER_H
#define BLADEMASTER_H
#include"player.h"

class Blademaster : public Player
{
    Q_OBJECT

    bool combo; //连续技
    bool shadowOfSword;//剑影
    int attackTime;//攻击次数
    int skill;

    bool canAttack();
    bool hasWind();

public:
    Blademaster(/*QObject *parent = 0,*/Server* server,int order,int teamNumber,int character=1);

    //void attack();
    void end();
    void normalAttack();

signals:

public slots:

};

#endif // BLADEMASTER_H
