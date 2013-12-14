#ifndef ORACLE_H
#define ORACLE_H
#include "player.h"

class Oracle : public Player
{
    void magicOne();
    void magicTwo();
    void magicThree();
    void magicFour();
public:
    Oracle(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void beforeAction();
    bool canActivate();
    void activate();
};

#endif // ORACLE_H
