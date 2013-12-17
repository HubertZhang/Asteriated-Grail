#ifndef SWORDKING_H
#define SWORDKING_H
#include"player.h"

class Swordking : public Player
{
    Q_OBJECT

    int swordsoul;
    int swordkee;
    int skill;
    int target;
public:
    Swordking(Server *server, int order, int teamNumber, int character);
    void normalAttack();
    void swordsoulChange(int number);
    void swordkeeChange(int number);

public slots:
    void skillone(int order);
};

#endif // SWORDKING_H
