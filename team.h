#ifndef TEAM_H
#define TEAM_H
#include "termination.h"
#include <set>
using std::set;

class Player;
enum Stone{Gem,Crystal};

class Team
{
    int stone;
    int crystal;
    int gem;
    int morale;
    int grail;
public:
    int team;
    Team();
    Player* player[3];
    void getStone(Stone a);
    void lossMorale(int);
    void getGrail();
};

#endif // TEAM_H
