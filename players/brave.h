#ifndef BRAVE_H
#define BRAVE_H
#include "player.h"

class Brave : public Player
{
    Q_OBJECT

    int dander;
    int intellect;
    int skill;
    int target;
    int combo;
    void danderChange(int);
    void intellectChange(int);
    void magicOne();
public:
    Brave(Server *server, int order, int teamNumber, int character);
    void normalAttack();
    void magicAction();
    void takeDamage(int damage, int kind);
    void characterConnect();
    void beforeAction();
    void changeCardLimit2(int amount);

public slots:
    void skillone(int order);

};

#endif // BRAVE_H
