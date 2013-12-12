#ifndef ADVENTURER_H
#define ADVENTURER_H
#include"player.h"

class Adventurer : public Player
{
    void attackOne();
    void magicOne();
    void magicTwo();
public:
    Adventurer(Server *server, int order, int teamNumber, int character);
    void refine();
    void magicAction();
    void normalAttack();
};

#endif // ADVENTURER_H
