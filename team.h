#ifndef TEAM_H
#define TEAM_H
#include <QObject>
#include "termination.h"

enum Stone{Gem,Crystal};

class Server;
class Team: public QObject
{
    Q_OBJECT
public:
    int crystal;
    int gem;
    int stone;
    int morale;
    int grail;
    const int team;
    Server* s;
    Team(Server*,int);
    void getStone(Stone a);
    void lossStone(Stone a);
    void lossMorale(int);
    void getGrail();
    void BroadCast();//改变士气，星石等时广播

    signals:
    void moraleloss(int,int);//灵魂术士
    void moraleloss2(int,int&);//灵魂术士
};

#endif // TEAM_H
