#ifndef PALADIN_H
#define PALADIN_H
#include"player.h"

class Paladin : public Player
{
    void magicOne();
    void magicTwo();
public:
    Paladin(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void takeDamage(int,int);
};

#endif // PALADIN_H
