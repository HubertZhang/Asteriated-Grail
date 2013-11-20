#ifndef TEAM_H
#define TEAM_H
#include <QObject>
#include "termination.h"

enum Stone{Gem,Crystal};

class Server;
class Team: public QObject
{
    Q_OBJECT
    int stone;
    int crystal;
    int gem;
    int morale;
    int grail;
    int team;
    Server* s;
public:
    Team(Server*,int);
    void getStone(Stone a);
    void lossStone(Stone a);
    void lossMorale(int);
    void getGrail();

    void BroadCast();//改变士气，星石等时广播
public slots:
};

#endif // TEAM_H
