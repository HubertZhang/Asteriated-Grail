#ifndef MAGISTER_H
#define MAGISTER_H
#include"player.h"
class Magister : public Player
{
    void magicOne();
    void magicTwo();
    void magicThree();
public:
    Magister(Server *server, int order, int teamNumber, int character);
    void magicAction();
};

#endif // MAGISTER_H
