#ifndef ASSASSIN_H
#define ASSASSIN_H
#include"player.h"

class Assassin : public Player
{
     Q_OBJECT
    int Attacker;
public:
    Assassin(Server* server,int order,int teamNumber,int character);
    void takeDamage(int damage,int kind);
    void countDamage(int damage,int kind);
    void normalAttack();
    void activate();
    void beforeAction();
    bool canActivate();
    void characterConnect();

public slots:
    void skillone(int attacker,int target, int& damage);
};

#endif // ASSASSIN_H
