#ifndef ELEMENTALIST_H
#define ELEMENTALIST_H
#include"player.h"

class Elementalist : public Player
{
    int element;
    void magicOne();
    void maigcTwo();
    void magicThree();
public:
    Elementalist(Server *server, int order, int teamNumber, int character);
    void magicAction();

};

#endif // ELEMENTALIST_H
