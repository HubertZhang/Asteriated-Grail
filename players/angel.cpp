#include "angel.h"
#include"server.h"
extern CardList cardlist;
/*
  1,普通技：法术 【风之洁净】：（弃一张风系牌【展示】）移除场上任意一个基础效果。
  2,普通技：法术 【天使祝福】：（弃一张水系牌【展示】）目标角色给你2张牌或指定2名角色各给你1张牌。
  3,普通技：被动B 【天使羁绊】： （每当你移除场上任意一个基础效果或使用圣盾）目标角色+1治疗
  4,独有技：法术 【天使之墙】：
  5,必杀技：响应B 【天使之歌】：【能量】×1 （回合开始前发动）移除场上任意一个基础效果。
  6,必杀技：响应D 【神之庇护】：【能量】×X 为我方抵御X点因法术伤害而造成的士气下降。
*/
//需要在server中链接降低士气和神之庇护槽
Angel::Angel(Server *server, int order, int teamNumber, int character)
    :Player(server,order,teamNumber,character)
{
    server->textg->character[order]->setText("天使");
}

void Angel::characterConnect()
{
    for (int i=0; i<6; i++)
    {
        if (server->players[i]->teamNumber == teamNumber)
        {
           connect(server->players[i],SIGNAL(moraleloss3(int,int&,int)),this,SLOT(skillone(int,int&,int)));
        }
    }
}

void Angel::skillone(int team, int& amount, int kind)
{
    if (energyCrystal + energyGem >= 1 && kind == Magic)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;
        sendMessageBuffer[3] = amount;

        sendMessage();
        receive();

        if (sendMessageBuffer[0])
        {
            receive();
            if (receiveMessageBuffer[0] != -1)
            {
                amount = amount - receiveMessageBuffer[0];
                useEnergy(receiveMessageBuffer[0]);
            }
        }
    }
}

void Angel::start()
{
    sendMessageBuffer[0] = TurnBegin;
    BroadCast(); //回合开始

    if (energyCrystal + energyGem >= 1)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;

        sendMessage();
        receive();

        if (sendMessageBuffer[0])
        {
            receive();
            if (sendMessageBuffer[0] != -1)
            {
                int magicTarget = receiveMessageBuffer[0];
                int statusRemoved = receiveMessageBuffer[1];
                useEnergy(1);

                sendMessageBuffer[0] = DrawPicture;
                sendMessageBuffer[1] = 1;
                sendMessageBuffer[3] = magicTarget;
                sendMessageBuffer[2] = 0;
                BroadCast();

                server->players[magicTarget]->destroyStatus(statusRemoved);
                if (server->players[magicTarget]->theShield == statusRemoved)
                {
                    server->players[magicTarget]->theShield = 0;
                }
                server->gamePile->putIntoPile(statusRemoved);
                addCure();
            }
        }
    }

    try{
    handleStatus();
    beforeAction();

    int i = 1;
    while(i--)
    {
        try{
        sendMessageBuffer[0] = ActionType;
        sendMessage();

        receive();
        int returnAcction = receiveMessageBuffer[0];
        if (returnAcction == Attack)
        {
            normalAttack();
        }
        else if (returnAcction == Magic)
        {
            magicAction();
        }
        }
        catch(ActionIllegal)
        {
            i++;//Do the While Loop again
        }
    }
    }
    catch(int)
    {

    }
    end();
}

void Angel::normalMagic()
{
    int cardUsed = receiveMessageBuffer[2];
    Player::normalMagic();
    if(cardlist.getName(cardUsed)==shield)
    {
        addCure();
    }
}

void Angel::addCure()
{
    sendMessageBuffer[0] = AskRespond1;
    sendMessage();
    receive();

    server->textg->textbrowser->append("你发动了天使羁绊");
    int target = receiveMessageBuffer[0];
    server->players[target]->increaseCure(1);
}

void Angel::magicAction()
{
    switch(receiveMessageBuffer[1])
    {
    case 0:
    {
        normalMagic();
        break;
    }
    case 1:
    {
        server->textg->textbrowser->append("你发动了风之洁净");
        magicOne();//风之洁净
        emit finishaction(order, Magic);
        break;
    }
    case 2:
    {
        server->textg->textbrowser->append("你发动了天使祝福");
        magicTwo();//天使祝福
        emit finishaction(order, Magic);
        break;
    }
    case 3:
    {
        server->textg->textbrowser->append("你发动了天使之墙");
        magicThree();//天使之墙
        emit finishaction(order, Magic);
        break;
    }
    }

}

void Angel::magicOne()
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];
    int statusRemoved = receiveMessageBuffer[4];
    foldCard(&cardUsed,1);

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    server->players[magicTarget]->destroyStatus(statusRemoved);
    if (server->players[magicTarget]->theShield == statusRemoved)
    {
        server->players[magicTarget]->theShield = 0;
    }
    server->gamePile->putIntoPile(statusRemoved);
    addCure();
}

void Angel::magicTwo()
{
//    int magicTarget[2] = {receiveMessageBuffer[2],receiveMessageBuffer[3]};
//    int cardUsed = receiveMessageBuffer[4];
//    foldCard(&cardUsed,1,true);
//    sendMessageBuffer[0] = DrawPicture;
//    sendMessageBuffer[1] = 1;
//    sendMessageBuffer[2] = 0;
//    sendMessageBuffer[3] = magicTarget[0];
//    sendMessageBuffer[4] = magicTarget[1];
//    receive();
//    //?
//    card.insert(receiveMessageBuffer[]);
//    sendMessageBuffer[2] = re;

//    cardNumber = card.size();
//    sendMessageBuffer[0] = GetCard;
//    sendMessageBuffer[1] = 2;
}

void Angel::magicThree()
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];
    //-------加盾-------
    card.erase(cardUsed);
    cardNumber = this->card.size();
    server->players[magicTarget]->theShield = cardUsed;

    sendMessageBuffer[0] = CardChange;
    sendMessageBuffer[1] = -1;
    BroadCast();

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    server->players[magicTarget]->addStatus(cardUsed);
}
