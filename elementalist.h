#ifndef ELEMENTALIST_H
#define ELEMENTALIST_H
#include"player.h"

class Elementalist : public Player
{
public:
    Elementalist(Server *server, int order, int teamNumber, int character);
};

#endif // ELEMENTALIST_H
