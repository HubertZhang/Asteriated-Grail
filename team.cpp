#include "team.h"

Team::Team()
{
    grail = 0;
    stone = gem = crystal = 0;
    morale = 15;
}

void Team::getStone(Stone a)
{
    if (stone<5)
    {
        stone++;
        if(a == Gem)
        {
            gem++;
        }
        else crystal++;
    }
    else return;
}

void Team::lossMorale(int lostNumberOfCard)
{
    morale-=lostNumberOfCard;
    if(morale<=0) throw LostAllMorale(team);
}

void Team::getGrail()
{
    grail++;
    if(grail == 5) throw GrailFinished(team);
}
