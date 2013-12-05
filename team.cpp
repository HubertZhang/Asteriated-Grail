#include "team.h"
#include "textgui.h"
#include"server.h"

using namespace std;

Team::Team(Server* a,int i):s(a),team(i)
{
    grail = 0;
    stone = gem = crystal = 0;
    morale = 15;

    QString string;
    string.sprintf("%d",grail);
    s->textg->teamstatus[team][0]->setText(string);
    string.sprintf("%d",morale);
    s->textg->teamstatus[team][1]->setText(string);
    string.sprintf("%d",gem);
    s->textg->teamstatus[team][2]->setText(string);
    string.sprintf("%d",crystal);
    s->textg->teamstatus[team][3]->setText(string);
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
    else
    {
      crystal--;
    }
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
//---------------测试-----------
void Team::BroadCast()
{   
    QString string;
    string.sprintf("%d",grail);
    s->textg->teamstatus[team][0]->setText(string);
    string.sprintf("%d",morale);
    s->textg->teamstatus[team][1]->setText(string);
    string.sprintf("%d",gem);
    s->textg->teamstatus[team][2]->setText(string);
    string.sprintf("%d",crystal);
    s->textg->teamstatus[team][3]->setText(string);

    vector<int> tempMessage;
    tempMessage.push_back(8);
    tempMessage.push_back(team);
    tempMessage.push_back(gem);
    tempMessage.push_back(crystal);
    tempMessage.push_back(grail);
    tempMessage.push_back(morale);
    s->networkServer.sendMessage(-1,tempMessage);
}
//------------------------------
