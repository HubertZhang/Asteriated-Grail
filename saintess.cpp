#include "saintess.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
#include"player.h"
extern CardList cardlist;

/*
1.普通技：被动A 【冰霜祷言】：（每当你使用水系攻击时发动①）目标角色+1【治疗】。
2.独有技：法术 【治疗术】：目标角色+2【治疗】。
3.独有技：法术 【治愈之光】：指定最多3名角色各+1【治疗】。
  必杀技：启动 【怜悯】【持续】：【宝石】×1 【横置】你的手牌上限恒定为7【恒定】。
4.必杀技：法术 【圣疗】【回合限定】：【能量】×1 任意分配3【治疗】给1~3名角色，额外+1【攻击行动】或【法术行动】。
*/

Saintess::Saintess(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    server->textg->character[order]->setText("圣女");
}

void Saintess::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;
    foldCard(&cardUsed,1,false);
    //暗灭无法应战，需要吗？
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();//展示攻击对象，攻击牌

    if  (cardlist.getName(cardUsed) == waterAttack)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;

        sendMessage();

        receive();

        if (receiveMessageBuffer[0])
        {
           server->textg->textbrowser->append("你发动了冰霜祷言");
           int target = receiveMessageBuffer[1];
           server->players[target]->increaseCure(1);
        }
    }

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
}
void Saintess::headOn(int chainLength)
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;

    foldCard(&cardUsed,1,false);
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    //BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    if  (cardlist.getName(cardUsed) == waterAttack)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;

        sendMessage();

        receive();

        if (receiveMessageBuffer[0])
        {
           server->textg->textbrowser->append("你发动了冰霜祷言");
           int target = receiveMessageBuffer[1];
           server->players[target]->increaseCure(1);
        }
    }

    emit miss(order);

    if(server->players[attackTarget]->beAttacked(order,cardUsed,chainLength,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
          (server->team[teamNumber])->getStone(Crystal);
          server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
}
void Saintess::activate()
{
    activation = 1;
    cardLimit++;

    sendMessageBuffer[0] = Activated;
    BroadCast();

    sendMessageBuffer[0] = CardLimitChange;
    sendMessageBuffer[1] = 1;
    BroadCast();

    energyGem--;
    sendMessageBuffer[0] = EnergyChange;
    sendMessageBuffer[1] = -1;
    sendMessageBuffer[2] = 0;
    BroadCast();
}

void Saintess::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 2)
    {
        server->textg->textbrowser->append("你发动治疗术");
        magicOne();
    }
    else if (receiveMessageBuffer[1] == 3)
    {
        server->textg->textbrowser->append("你发动了治愈之光");
        magicTwo();
    }
    else if (receiveMessageBuffer[1] == 4)
    {
        server->textg->textbrowser->append("你发动了圣疗");
        magicThree();
    }

    sendMessageBuffer[0] = AskRespond;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = 1;

    sendMessage();

    receive();

    if (receiveMessageBuffer[0])
    {
    server->textg->textbrowser->append("额外攻击");
    normalAttack();
    }
}

void Saintess::magicOne()//治疗术
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    foldCard(&cardUsed,1,false);

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
   // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    server->players[magicTarget]->increaseCure(1);
}

void Saintess::magicTwo()//治愈之光
{
    int cardUsed = receiveMessageBuffer[2];
    int targetnumber = receiveMessageBuffer[3];

    foldCard(&cardUsed,1,false);

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = targetnumber;
    sendMessageBuffer[2] = 1;
    int i=0;
    for (i=0; i<targetnumber; i++)
    {
        sendMessageBuffer[3+i] = receiveMessageBuffer[4+i];
    }
    sendMessageBuffer[4+i] = cardUsed;
    BroadCast();

    for (i=0; i<targetnumber; i++)
    {
        server->players[receiveMessageBuffer[4+i]]->increaseCure(1);
    }
}

void Saintess::magicThree()//圣疗
{
    int targetnumber = receiveMessageBuffer[2];

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = targetnumber;
    sendMessageBuffer[2] = 0;
    int i=0;
    for (i=0; i<targetnumber; i++)
    {
        sendMessageBuffer[3+i] = receiveMessageBuffer[3+2*i];
    }
    BroadCast();

    if (receiveMessageBuffer[4] == 1)
    energyGem--;
    else
    energyCrystal--;

    sendMessageBuffer[0] = EnergyChange;
    sendMessageBuffer[1] = -receiveMessageBuffer[3];
    sendMessageBuffer[2] = 0;
    BroadCast();//改变人物能量数量

    for (i=0; i<targetnumber; i++)
    {
        server->players[receiveMessageBuffer[3+2*i]]->increaseCure(receiveMessageBuffer[4+2*i]);
    }

    sendMessageBuffer[0] = ActionType;
    sendMessage();

    receive();

    int returnAcction = receiveMessageBuffer[0];
    if (returnAcction == Attack)
    {
        attackAction();
    }
    else if (returnAcction == Magic)
    {
        magicAction();
    }
}
