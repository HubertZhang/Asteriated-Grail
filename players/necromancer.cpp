#include "necromancer.h"
#include"server.h"
extern CardList cardlist;
/*
 普通技：响应C 【不朽】：（【法术行动】结束时发动）你+1【治疗】。
 普通技：被动 【圣渎】：你的【治疗】不能抵御攻击伤害，你的【治疗】上限+3。
 2.普通技：法术 【瘟疫】：（弃一张地系牌【展示】）对其他所有角色各造成1点法术伤害③。
 3.普通技：法术 【死亡之触】：（移除你面前的a【治疗】并弃b张同系牌【展示】，a,b的数值由你决定，但每项最少为2）对目标角色造成(a+b-3)点法术伤害③；不能和【不朽】同时发动。
 4.必杀技：法术 【墓碑陨落】：【宝石】×1 对其他所有角色各造成2点法术伤害③，你+1【治疗】。
*/
Necromancer::Necromancer(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    cureLimit = cureLimit + 3;
    server->textg->character[order]->setText("死灵");
}

void Necromancer::magicAction()
{
    int message = receiveMessageBuffer[1];
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 2)//瘟疫
    {
         magicOne();
    }
    else if (receiveMessageBuffer[1] == 3)//死亡之触
    {
         magicTwo();
    }
    else if (receiveMessageBuffer[1] == 4)//墓碑陨落
    {
        magicThree();
    }
//------------不朽------------------
    if (message != 2)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;

        if (receiveMessageBuffer[0])
        {
        server->textg->textbrowser->append("你发动了不朽");
        increaseCure(1);
        }
    }
}
void Necromancer::countDamage(int damage, int kind)
{
//---------圣渎--------------------------
    if(cureExist() && kind != Attack)
    {
        int realDamage = damage - useCure(damage);
        if (realDamage > 0)
        takeDamage(realDamage,kind);
    }
    else
    {
        takeDamage(damage,kind);
    }
}
void Necromancer::magicOne()
{
    server->textg->textbrowser->append("你发动了瘟疫");
    int cardUsed = receiveMessageBuffer[2];
    int damage = 1;

    foldCard(&cardUsed);

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 5;
    sendMessageBuffer[2] = 1;
    for (int i=0; i<5; i++)
    {
        sendMessageBuffer[i+3] = (order + i + 1)%6;
    }
    sendMessageBuffer[8] = cardUsed;
    BroadCast();

    for (int i=0; i<5; i++)
    {
        server->players[(order + i + 1)%6]->countDamage(damage,Magic);
    }
}
void Necromancer::magicTwo()
{
    server->textg->textbrowser->append("你发动了死亡之触");
    int MagicTarget = receiveMessageBuffer[2];
    int curenumber = receiveMessageBuffer[3];
    int cardnumber = receiveMessageBuffer[4];
    int damage = curenumber + cardnumber - 3;

    foldCard(receiveMessageBuffer+5,cardnumber);
    decreaseCure(curenumber);

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = cardnumber;
    sendMessageBuffer[3] = MagicTarget;
    for (int i=0; i<cardnumber; i++)
    {
        sendMessageBuffer[i+4] = receiveMessageBuffer[i+5];
    }
    BroadCast();

    server->players[MagicTarget]->countDamage(damage,Magic);
}
void Necromancer::magicThree()
{
    server->textg->textbrowser->append("你发动了墓碑陨落");
    int damage = 2;

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 5;
    sendMessageBuffer[2] = 0;
    for (int i=0; i<5; i++)
    {
        sendMessageBuffer[i+3] = (order + i + 1)%6;
    }
    BroadCast();

    useEnergy(1,true);

    for (int i=0; i<5; i++)
    {
        server->players[(order + i + 1)%6]->countDamage(damage,Magic);
    }

    increaseCure(1);
}
