#ifndef PSYCHIC_H
#define PSYCHIC_H
#include"player.h"

class Psychic : public Player
{
    Q_OBJECT

    void magicOne();
    void magicTwo();
    void magicThree();
    void magicFour();
    int specialTarget;
    bool Blood;
public:
    Psychic(Server *server, int order, int teamNumber, int character);
    void magicAction();
    void BroadCast();
    void start();
    void Discards(int amount, int kind);
    void activate();
public slots:
    void skillone(int target);
};

#endif // PSYCHIC_H
