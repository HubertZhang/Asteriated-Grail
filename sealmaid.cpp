#include "sealmaid.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
extern CardList cardlist;
/*
  1,普通技：响应C 【法术激荡】：（【法术行动】结束时发动）额外+1【攻击行动】。
  2,独有技：法术 【地之封印】：（将【地之封印】放置于目标对手面前）该角色获得（直到他从手中打出或展示出地系牌时强制触发）：对他造成3点法术伤害③，触发后移除此牌。
    独有技：法术 【水之封印】【火之封印】【风之封印】【雷之封印】：同上，依次类推。
  3,必杀技：法术 【五系束缚】：【能量】×1 （将【五系束缚】放置于目标对手面前）该对手跳过其下个行动阶段。在其下个行动阶段开始前他可以选择摸（2+X）张牌来取消【五系束缚】的效果。X为场上封印的数量，X最高为2。不论效果是否发动，触发后移除此牌。
  4,必杀技：法术 【封印破碎】：【能量】×1 将场上任意一张基础效果牌收入自己手中。
*/

Sealmaid::Sealmaid(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    //skill = 0;
    //target = -1;
    server->textg->character[order]->setText("封印师");
}

void Sealmaid::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 2)
    {
        server->textg->textbrowser->append("你发动了五行封印");
        magicOne();//五行封印
    }
    else if (receiveMessageBuffer[1] == 3)
    {
        server->textg->textbrowser->append("你发动了五系束缚");
        magicTwo();//五系束缚
    }
    else if (receiveMessageBuffer[1] == 4)
    {
        server->textg->textbrowser->append("你发动了封印破碎");
        magicThree();//封印破碎
    }

    sendMessageBuffer[0] = AskRespond;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = 3;

    sendMessage();
    receive();

    if (receiveMessageBuffer[0])
    {
    server->textg->textbrowser->append("额外攻击");
    normalAttack();
    }
}

void Sealmaid::magicOne()
{
    server->textg->textbrowser->append("你使用了五行封印");

    connect(server->players[attackTarget],SIGNAL(fold(int,int)),this,SLOT(skillone(int,int)));

    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    card.erase(cardUsed);
    cardNumber = this->card.size();
    server->players[magicTarget]->addStatus(cardUsed);

    sendMessageBuffer[0] = CardChange;
    sendMessageBuffer[1] = -1;
    BroadCast();

    sendMessageBuffer[0] = StatusIncrease;
    sendMessageBuffer[1] = cardUsed;
    sendMessageBuffer[2] = magicTarget;
    BroadCast();//添加状态

}

void Sealmaid::magicTwo()
{
    server->textg->textbrowser->append("你使用了五系封印");

    int magicTarget = receiveMessageBuffer[2];
    //int cardUsed = receiveMessageBuffer[3];
    server->players[magicTarget]->addStatus(150);

    sendMessageBuffer[0] = StatusIncrease;
    sendMessageBuffer[1] = 150;
    sendMessageBuffer[2] = magicTarget;
    BroadCast();//添加状态
}

void Sealmaid::magicThree()
{
    server->textg->textbrowser->append("你使用了封印破碎");

    int magicTarget = receiveMessageBuffer[2];
    int statusTake = receiveMessageBuffer[3];

    if (cardlist.getName(statusTake) == shield)
    {
         server->players[magicTarget]->theShield = 0;
    }
    if (cardlist.getName(statusTake) == )

    int k;
    for (int i=0; i<server->players[magicTarget]->statusNumber; i++)
    {
        if (server->players[magicTarget]->status[i] == statusTake)
        {
            k = i;
            break;
        }
    }
    for (int i = k; i < server->players[magicTarget]->statusnumber-1;  i++)
    {
        server->players[magicTarget]->status[i] = server->players[magicTarget]->status[i+1];
    }

    server->players[magicTarget]->status[statusnumber-1] = -1;
    server->players[magicTarget]->statusnumber--;

    sendMessageBuffer[0] = StatusDecrease;
    sendMessageBuffer[1] = card;
    server->players[magicTarget]->BroadCast();//消去虚弱或中毒或圣盾


    //------收入手中--------------------------------
        card.insert(statusTake);
        sendMessageBuffer[2] = cardname;

        cardNumber = card.size();
        sendMessageBuffer[0] = GetCard;
        sendMessageBuffer[1] = 1;
        sendMessage();

        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = 1;
        BroadCast();//改变手牌数量
    //---------------------------------------------

}


