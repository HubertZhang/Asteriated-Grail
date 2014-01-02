#ifndef SOULWARLOCK_H
#define SOULWARLOCK_H
#include "player.h"

class Soulwarlock : public Player
{
    Q_OBJECT
    void magicOne();
    void magicTwo();
    void magicThree();
    void magicFour();
    void yellowChange(int);
    void blueChange(int);
    int yellow;
    int blue;
    int thetarget;
public:
    Soulwarlock(Server *server, int order, int teamNumber, int character);
    void normalAttack();
    void magicAction();
    void activate();
    void characterConnect();

public slots:
    void skillone(int theteam, int morale);
    void skilltwo(int target, int& damage, int kind);
};

#endif // SOULWARLOCK_H
