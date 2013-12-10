#ifndef BERSERKER_H
#define BERSERKER_H
#include"player.h"

class Berserker : public Player
{
    Q_OBJECT

    int skill;
public:
    Berserker(Server* server,int order,int teamNumber,int character=2);
    void normalAttack();
    void headOn(int);
    void end();
signals:

public slots:

};

#endif // BERSERKER_H
