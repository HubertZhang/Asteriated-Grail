#ifndef TEAM_H
#define TEAM_H
#include "termination.h"
#include <set>
using std::set;

enum Stone{Gem,Crystal};

class Team
{
    int stone;
    int crystal;
    int gem;
    int morale;
    int grail;
    int team;
public:
    Team();
    void getStone(Stone a);
    void lossStone(Stone a);
    void lossMorale(int);
    void getGrail();
    void BroadCast();//改变士气，星石等时广播
};

#endif // TEAM_H
