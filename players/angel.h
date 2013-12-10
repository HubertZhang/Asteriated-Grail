#ifndef ANGEL_H
#define ANGEL_H
#include "player.h"
class Angel : public Player
{
    void magicOne();//风之洁净
    void magicTwo();//天使祝福
    void magicThree();//天使之墙
    void addCure();
public:
    Angel(Server* server,int order,int teamNumber,int character);
    void magicAction();
    void normalMagic();
};

#endif // ANGEL_H
