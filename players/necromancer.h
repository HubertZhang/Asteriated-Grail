#ifndef NECROMANCER_H
#define NECROMANCER_H
#include"player.h"

class Necromancer : public Player
{
    void magicOne();
    void magicTwo();
    void magicThree();
public:
    Necromancer(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void countDamage(int damage, int kind);

};

#endif // NECROMANCER_H
