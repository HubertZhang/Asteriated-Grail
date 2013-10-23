#ifndef TEAM_H
#define TEAM_H
#include "player.h"
#define RED 0
#define BLUE 1
class Team
{
    friend class Player;
private:
    int color;
    int grail;
    int morale;
    int gem;
    int crystal;
    int stone;
public:
    Team(int allocateColor);
};

#endif // TEAM_H
