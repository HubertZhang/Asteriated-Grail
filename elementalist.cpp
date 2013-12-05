#include "elementalist.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
extern CardList cardlist;
/*
 *0.普通技：响应D 【元素吸收】：（对目标角色造成法术伤害时发动③）你+1【元素】。
 *1. 普通技：法术 【元素点燃】：（移除3点【元素】）对目标角色造成2点法术伤害③，额外+1【法术行动】；不能和【元素吸收】同时发动。
  2. 独有技：法术 【陨石】：对目标角色造成1点法术伤害③，额外+1【法术行动】；（若你额外弃1张地系牌【展示】①）本次伤害额外+1。
  3. 独有技：法术 【风刃】：对目标角色造成1点法术伤害③，额外+1【攻击行动】；（若你额外弃1张风系牌【展示】①）本次伤害额外+1。
  4. 独有技：法术 【火球】：对目标角色造成2点法术伤害③；（若你额外弃1张火系牌【展示】①）本次伤害额外+1。
  5. 独有技：法术 【雷击】：对目标角色造成1点法术伤害③，我方【战绩区】+1【宝石】；（若你额外弃1张雷系牌【展示】①）本次伤害额外+1。
  6. 独有技：法术 【冰冻】：对目标角色造成1点法术伤害③，并指定1名角色+1【治疗】；（若你额外弃1张水系牌【展示】①）本次伤害额外+1。
  7. 必杀技：法术 【月光】：【宝石】×1 对目标角色造成（X+1）点法术伤害③，X为你剩余的【能量】数。

 */
Elementalist::Elementalist(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    element = 0;
    //target = -1;
    server->textg->character[order]->setText("元素");
}

void Elementalist::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)//元素点燃
    {
         magicOne();
    }
    else if (receiveMessageBuffer[1] == 2)//陨石,风刃,火球,雷击,冰冻
    {
         //magicTwo();
    }
    else if (receiveMessageBuffer[1] == 3)//月光
    {
        magicThree();
    }
}


void Elementalist::magicOne()
{
    int magicTarget = receiveMessageBuffer[2];

    int damage = 2;

    element = element -3;
    BroadCast();//???

    server->textg->textbrowser->append("你发动了元素点燃");

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    BroadCast();

    server->players[magicTarget]->countDamage(damage,Magic);

    sendMessageBuffer[0] = AdditionalAction;
    sendMessageBuffer[1] = 1;
    sendMessage();

    receive();

    if (receiveMessageBuffer[0] == 1)
    {
        magicAction();
    }
}

void Elementalist::maigcTwo()
{
    int magicTarget = receiveMessageBuffer[2];
    int usenumber = receiveMessageBuffer[3];
    int cardUsed = receiveMessageBuffer[4];
    int damage = 1;

    foldCard(receiveMessageBuffer+4,usenumber);
    //展示攻击对象，攻击牌
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = usenumber;
    for (int i=0; i<usenumber; i++)
    {
        sendMessageBuffer[i+3] = receiveMessageBuffer[4+i];
    }
    sendMessageBuffer[3+usenumber] = magicTarget;
    BroadCast();

    int cardname = cardlist.getName(cardUsed);

    if (cardname == groundAttack)
    {
        server->textg->textbrowser->append("你发动了陨石");

        server->players[magicTarget]->countDamage(damage + usenumber -1,Magic);

        sendMessageBuffer[0] = AdditionalAction;
        sendMessageBuffer[1] = 1;
        sendMessage();

        receive();

        if (receiveMessageBuffer[0] == 1)
        {
            magicAction();
        }
    }
    else if (cardname == windAttack)
    {
        server->textg->textbrowser->append("你发动了风刃");

        server->players[magicTarget]->countDamage(damage + usenumber -1,Magic);

        sendMessageBuffer[0] = AdditionalAction;
        sendMessageBuffer[1] = 0;
        sendMessage();

        receive();

        if (receiveMessageBuffer[0] == 0)
        {
            normalAttack();
        }
    }
    else if (cardname == fireAttack)
    {
        server->textg->textbrowser->append("你发动了火球");

        server->players[magicTarget]->countDamage(damage + usenumber,Magic);
    }
    else if (cardname == thunderAttack)
    {
        server->textg->textbrowser->append("你发动了雷击");

        server->players[magicTarget]->countDamage(damage + usenumber -1,Magic);
        server->team[teamNumber]->getStone(Gem);
    }
    else if (cardname == waterAttack)
    {
        server->textg->textbrowser->append("你发动了冰冻");

        server->players[magicTarget]->countDamage(damage + usenumber -1,Magic);

        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;

        sendMessage();

        receive();

        int target = receiveMessageBuffer[0];
        server->players[target]->increaseCure(1);
    }
}

void Elementalist::magicThree()
{
    int magicTarget = receiveMessageBuffer[2];
    int damage = 1 + element;

    server->textg->textbrowser->append("你发动了月光");

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    BroadCast();

    useEnergy(1,true);

    server->players[magicTarget]->countDamage(damage,Magic);
}











