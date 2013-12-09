#include "sealer.h"
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

Sealer::Sealer(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    number = 0;
    server->textg->character[order]->setText("封印");
}

void Sealer::characterConnect()
{
    for (int i=0; i<6; i++)
    {
         connect(server->players[i],SIGNAL(fold(int,int)),this,SLOT(skilltwo(int,int)));
         connect(server->players[i],SIGNAL(decreasestatus(int,int)),this,SLOT(skillthree(int,int)));
    }
}

void Sealer::magicAction()
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
    sendMessageBuffer[2] = 0;

    sendMessage();
    receive();
    if (receiveMessageBuffer[0] == 1)
    {
    sendMessageBuffer[0] = AdditionalAction;
    sendMessageBuffer[1] = 0;
    sendMessage();

    receive();
    if (receiveMessageBuffer[0] == 0)
    {
      server->textg->textbrowser->append("额外攻击");
      normalAttack();
    }
    }
}

void Sealer::magicOne()
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    sealcard[magicTarget].insert(cardUsed);
    number++;//五行封印个数

    card.erase(cardUsed);
    cardNumber = this->card.size();

    sendMessageBuffer[0] = CardChange;
    sendMessageBuffer[1] = -1;
    BroadCast();

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
   // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    server->players[magicTarget]->addStatus(cardUsed);

}

void Sealer::magicTwo()
{
    int magicTarget = receiveMessageBuffer[2];
    //int cardUsed = receiveMessageBuffer[3];

    connect(server->players[magicTarget],SIGNAL(beweak(int,int,int)),this,SLOT(skillone(int,int,int)));

    useEnergy(1);

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    //sendMessageBuffer[2] = cardUsed;
   // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    server->players[magicTarget]->addStatus(150);
}

void Sealer::magicThree()
{
    int magicTarget = receiveMessageBuffer[2];
    int statusTake = receiveMessageBuffer[3];

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    //sendMessageBuffer[2] = cardUsed;
   // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    server->players[magicTarget]->destroyStatus(statusTake);
    if (server->players[magicTarget]->theShield == statusTake)
    {
        server->players[magicTarget]->theShield = 0;
    }

    useEnergy(1);


    //------收入手中--------------------------------
        card.insert(statusTake);
        sendMessageBuffer[2] = statusTake;

        cardNumber = card.size();
        sendMessageBuffer[0] = GetCard;
        sendMessageBuffer[1] = 1;
        sendMessage();

        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = 1;
        BroadCast();//改变手牌数量
    //---------------------------------------------

}

void Sealer::skillone(int target,int accepttype,int w)
{
    if (w != -1)
    {
        server->players[target]->destroyStatus(150);
        server->players[target]->destroyStatus(w);
        if (accepttype == Accept)
        {
            if (number<=2)
            {
                server->players[target]->takeDamage(number+5,2);
            }
            else
            {
                server->players[target]->takeDamage(7,2);
            }
        }
    }
    else
    {
        server->players[target]->destroyStatus(150);
        if (accepttype == Accept)
        {
            if (number<=2)
            {
                server->players[target]->takeDamage(number+2,2);
            }
            else
            {
                server->players[target]->takeDamage(4,2);
            }
        }
    }
    disconnect(server->players[target],SIGNAL(beweak(int,int,int)),this,SLOT(skillone(int,int,int)));
}

void Sealer::skilltwo(int player,int card)//弃牌与五行封印
{
    int type = cardlist.getNature(card);
    //QString s;
    //s.sprintf("tt%d",order);
    //server->textg->textbrowser->append(s);
    set<int>::iterator i;
    for (i=sealcard[player].begin(); i!=sealcard[player].end(); i++)
    {
        //s.sprintf("tt%d %d",order,i);
        //server->textg->textbrowser->append(s);
        if (cardlist.getSkillOne(*i) == (41+type) || cardlist.getSkillOne(*i) == (41+type))
        {
            server->players[player]->takeDamage(3,Magic);
            server->players[player]->destroyStatus(*i);
            server->gamePile->putIntoPile(card);
            break;
        }
    }
}

void Sealer::skillthree(int player, int card)
{
    //QString s;
    //s.sprintf("rr%d",order);
    //server->textg->textbrowser->append(s);
    if (sealcard[player].count(card) >= 1)
    {
       // s.sprintf("rr%d %d",order);
        //server->textg->textbrowser->append(s);
        sealcard[player].erase(card);
        number--;
    }
}


