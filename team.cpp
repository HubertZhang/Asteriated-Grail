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
    BroadCast();//改变星石数量
    return;
}

void Team::lossStone(Stone a)
{
    stone--;
    if (a == Gem)
    {
        gem--;
    }
    else crystal--;
    BroadCast();
    return;
}

void Team::lossMorale(int lostNumberOfCard)
{
    morale-=lostNumberOfCard;
    BroadCast();//改变士气数量
    if(morale<=0) throw LostAllMorale(team);
}

void Team::getGrail()
{
    grail++;
    BroadCast();//改变星杯数量
    if(grail == 5) throw GrailFinished(team);
}

void Team::BroadCast()
{

}
