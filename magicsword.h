#ifndef MAGICSWORD_H
#define MAGICSWORD_H
#include"player.h"

class Magicsword : public Player
{
    void magicOne();
    bool combo;
public:
    Magicsword(Server *server, int order, int teamNumber, int character);
    void normalAttack();
    void headOn(int chainLength);
    void activate();
    void magicAction();
    void beforeAction();
};

#endif // MAGICSWORD_H
