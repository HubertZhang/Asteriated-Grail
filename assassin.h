#ifndef ASSASSIN_H
#define ASSASSIN_H
#include"player.h"

class Assassin : public Player
{
    int Attacker;
public:
    Assassin(Server* server,int order,int teamNumber,int character);
    void takeDamage(int damage,int kind);
    void countDamage(int damage,int kind);
    bool beAttacked(int attacker, int cardUsed, int chainLength, bool canBeAccept);
    void normalAttack();
    void activate();
    void beforeAction();
};

#endif // ASSASSIN_H
