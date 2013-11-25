#ifndef SEALMAID_H
#define SEALMAID_H
#include"player.h"

class Sealmaid : public Player
{
    Q_OBJECT

    void magicOne();//五行封印
    void magicTwo();//五系束缚
    void magicThree();//封印破碎
public:
    Sealmaid(Server* server,int order,int teamNumber,int character);
    void magicAction();

public slots:
    void skillone(int order);
    void skilltwo(int order);
};

#endif // SEALMAID_H
