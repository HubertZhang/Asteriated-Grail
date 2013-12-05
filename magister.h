#ifndef MAGISTER_H
#define MAGISTER_H
#include"player.h"
class Magister : public Player
{
    void magicOne();
    void maigcTwo();
public:
    Magister(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void beMagicMissileAttack();
};

#endif // MAGISTER_H
