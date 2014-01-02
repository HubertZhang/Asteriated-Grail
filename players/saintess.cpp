#include "saintess.h"
#include"server.h"
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
    activation = 0;
    server->textg->character[order]->setText("圣女");
}

void Saintess::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;
    foldCard(&cardUsed);
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
        sendMessageBuffer[0] = AskRespond1;
        //sendMessageBuffer[1] = 1;
        //sendMessageBuffer[2] = 1;

        sendMessage();

        receive();

        server->textg->textbrowser->append("你发动了冰霜祷言");
        int target = receiveMessageBuffer[0];
        server->players[target]->increaseCure(1);
    }

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
           emit attacked(order, attackTarget, damage);
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
    emit finishaction(order, Attack);
}
void Saintess::headOn(int chainLength)
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;

    foldCard(&cardUsed);
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
        sendMessageBuffer[0] = AskRespond1;
        //sendMessageBuffer[1] = 1;
        //sendMessageBuffer[2] = 1;

        sendMessage();

        receive();

        server->textg->textbrowser->append("你发动了冰霜祷言");
        int target = receiveMessageBuffer[0];
        server->players[target]->increaseCure(1);
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
          emit attacked(order, attackTarget, damage);
          (server->team[teamNumber])->getStone(Crystal);
          server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
}
void Saintess::activate()
{
    activation = 1;

    sendMessageBuffer[0] = Activated;
    sendMessageBuffer[1] = 1;
    BroadCast();

    changeCardLimit1(7);

    useEnergy(1,true);
}

bool Saintess::canActivate()
{
    return (!((cardLimit - cardNumber) < 3 && (activation == 1 || energyGem == 0)
            &&(energyCrystal+energyGem==stonelimit || server->team[teamNumber]->stone==0)));
}

void Saintess::changeCardLimit2(int amount)
{
    if (activation == 1)
    {
        return;
    }
    Player::changeCardLimit2(amount);
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
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 3)
    {
        server->textg->textbrowser->append("你发动了治愈之光");
        magicTwo();
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 4)
    {
        server->textg->textbrowser->append("你发动了圣疗");
        magicThree();
    }
}

void Saintess::magicOne()//治疗术
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    foldCard(&cardUsed);

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
   // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    server->players[magicTarget]->increaseCure(2);
}

void Saintess::magicTwo()//治愈之光
{
    int cardUsed = receiveMessageBuffer[2];
    int targetnumber = receiveMessageBuffer[3];

    foldCard(&cardUsed);

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = targetnumber;
    sendMessageBuffer[2] = 1;
    int i=0;
    for (i=0; i<targetnumber; i++)
    {
        sendMessageBuffer[3+i] = receiveMessageBuffer[4+i];
    }
    sendMessageBuffer[3+i] = cardUsed;
    BroadCast();

    for (i=0; i<targetnumber; i++)
    {
        server->players[receiveMessageBuffer[4+i]]->increaseCure(1);
    }
}

void Saintess::magicThree()//圣疗
{
    int target1 = receiveMessageBuffer[2];
    int target2 = receiveMessageBuffer[3];
    int target3 = receiveMessageBuffer[4];

    int targetnumber;
    if(target1== target2 && target1 == target3)
    {
        targetnumber = 1;
        receiveMessageBuffer[3] = target1;
        receiveMessageBuffer[4] = 3;
    }
    else if (target1== target2 || target1== target3 ||target3== target2)
    {
        targetnumber = 2;
        if (target1 == target2)
        {
            receiveMessageBuffer[3] = target1;
            receiveMessageBuffer[4] = 2;
            receiveMessageBuffer[5] = target3;
            receiveMessageBuffer[6] = 1;
        }
        else if (target1 == target3)
        {
            receiveMessageBuffer[3] = target1;
            receiveMessageBuffer[4] = 2;
            receiveMessageBuffer[5] = target2;
            receiveMessageBuffer[6] = 1;
        }
        else
        {
            receiveMessageBuffer[3] = target1;
            receiveMessageBuffer[4] = 1;
            receiveMessageBuffer[5] = target2;
            receiveMessageBuffer[6] = 2;
        }
    }
    else
    {
        targetnumber = 3;
        receiveMessageBuffer[3] = target1;
        receiveMessageBuffer[4] = 1;
        receiveMessageBuffer[5] = target2;
        receiveMessageBuffer[6] = 1;
        receiveMessageBuffer[7] = target3;
        receiveMessageBuffer[8] = 1;
    }

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = targetnumber;
    sendMessageBuffer[2] = 0;
    int i=0;
    for (i=0; i<targetnumber; i++)
    {
        sendMessageBuffer[3+i] = receiveMessageBuffer[3+2*i];
    }
    BroadCast();

    useEnergy(1);

    for (i=0; i<targetnumber; i++)
    {
        server->players[receiveMessageBuffer[3+2*i]]->increaseCure(receiveMessageBuffer[4+2*i]);
    }

    emit finishaction(order, Magic);

    sendMessageBuffer[0] = AdditionalAction;
    sendMessageBuffer[1] = 2;
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
