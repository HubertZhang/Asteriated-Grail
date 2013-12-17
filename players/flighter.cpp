#include "flighter.h"
#include"server.h"
extern CardList cardlist;
/*
 *普通技：被动D 【念气力场】：所有对你造成的伤害每次最高为4点③。
  0.普通技：响应A 【蓄力一击】：（主动攻击前发动①，+1【斗气】）
                             本次攻击伤害额外+1；
                             若未命中②，对自己造成X点法术伤害③，X为你拥有的【斗气】数；
                             若【斗气】已达到上限，则不能发动【蓄力一击】。
  1.普通技：响应C 【念弹】：（【法术行动】结束时发动，+1【斗气】）
                             对目标对手造成1点法术伤害③；
                             若发动前对方治疗点为0，则对自己造成X点法术伤害③，X为你拥有的【斗气】数；
                             若【斗气】已达到上限，则不能发动【念弹】。
  普通技：启动 【百式幻龙拳】【持续】：（移除3点【斗气】）
                            【横置】你的所有主动攻击伤害额外+2，所有应战攻击伤害额外+1；
                                   在你接下来的行动阶段，你不能执行【法术行动】和【特殊行动】、
                                   你的主动攻击必须以同一名角色为目标，并且不能发动【蓄力一击】；
                                   若不如此做，则取消【百式幻龙拳】效果并【重置】。
  2.普通技：响应A 【苍炎之魂】：（主动攻击前发动①，移除1点【斗气】）
                            本次攻击对方不能应战；
                            攻击结束后对自己造成X点法术伤害③，X为你拥有的【斗气】数；
                            不能和【蓄力一击】同时发动。
  必杀技：启动 【斗神天驱】：【能量】×1 弃到3张牌，你+2【治疗】。
 */

Flighter::Flighter(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    activation = 0;
    grudge = 0;
    skill = 0;
    target = -1;
    thetarget = -1;
}

void Flighter::countDamage(int damage, int kind)//念气力场
{
    if (damage > 4)
        damage = 4;
    Player::countDamage(damage,kind);
}

void Flighter::beforeAction()
{
    if (!canActivate())
    {
        return;
    }
    sendMessageBuffer[0] = BeforeAction;
    sendMessage();

    receive();
    int returnAction = receiveMessageBuffer[0];
    switch (returnAction)
    {
    case (Activate):
    {
        if (activation == 1)
        {
            activation = 0;
            sendMessageBuffer[0] = Activated;
            sendMessageBuffer[1] = 0;
            BroadCast();
        }
        activate();
        break;
    }
    case (Purchase):
    {
        if (activation == 1)
        {
            activation = 0;
            sendMessageBuffer[0] = Activated;
            sendMessageBuffer[1] = 0;
            BroadCast();
        }
        this->purchase();
        throw 1;
        break;
    }
    case (Fusion):
    {
        if (activation == 1)
        {
            activation = 0;
            sendMessageBuffer[0] = Activated;
            sendMessageBuffer[1] = 0;
            BroadCast();
        }
        this->fusion();
        throw 1;
        break;
    }
    case (Refine):
    {
        this->refine();
        throw 1;
        break;
    }
    }
    return;
}

void Flighter::headOn(int chainLength)
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;
    if (activation == 1)
        damage++;

    foldCard(&cardUsed);
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

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

void Flighter::grudgeChange(int number)
{
    if ((grudge + number) <= 6)
    {
       grudge = grudge + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[1] = number;
       sendMessageBuffer[2] = 0;
       BroadCast();
    }
    else if (grudge != 6)
    {
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[1] = 6-grudge;
        sendMessageBuffer[2] = 0;
        BroadCast();
        grudge = 3;
    }
}

void Flighter::activate()
{
    if (sendMessageBuffer[1] == 1)
    {
//------------百式幻龙拳------------------------
        activation = 1;
        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 1;
        BroadCast();

        grudgeChange(-3);
    }
    else
    {
//----------斗神天驱---------------------------
        useEnergy(1);

        if (cardNumber > 3)
        {
            int amount = cardNumber - 3;
            sendMessageBuffer[0] = FoldCard;
            sendMessageBuffer[1] = amount;
            sendMessage();

            receive();

            foldCard(receiveMessageBuffer,amount);
        }

        increaseCure(2);
    }
}

void Flighter::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;

    if (activation == 1 && thetarget == -1)
    {
        thetarget = attackTarget;
    }

    if (activation == 1 && thetarget != attackTarget)
    {
        activation = 0;
        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 0;
        BroadCast();
    }
    else if (activation == 1 && thetarget == attackTarget)
    {
        damage = damage + 2;
    }

    target = attackTarget;
    connect(server->players[attackTarget],SIGNAL(miss(int)),this,SLOT(skillone(int)));

    foldCard(&cardUsed);

    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;
    if (grudge != 6)
    {
//----------蓄力一击/苍炎之魂----------------------
        if (grudge >= 1)
        {
            sendMessageBuffer[3] = 2;
            sendMessageBuffer[1] = 2;
        }
        else
        {
            sendMessageBuffer[1] = 1;
        }

        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[2] = 0;
        sendMessage();

        receive();

        if (receiveMessageBuffer[0] == 1)
        {
            if (activation == 1)
            {
                activation = 0;
                sendMessageBuffer[0] = Activated;
                sendMessageBuffer[1] = 0;
                BroadCast();
                damage = damage - 2;
            }
            grudgeChange(1);
            damage++;
            skill = 1;
        }
        else if (receiveMessageBuffer[0] == 2)
        {
            grudgeChange(-1);
            canBeAccept = false;
            skill = 2;
        }
    }
    else
    {
//----------苍炎之魂----------------------
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 2;
        sendMessage();

        receive();

        if (receiveMessageBuffer[0] == 2)
        {
            grudgeChange(-1);
            canBeAccept = false;
            skill = 2;
        }
    }

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())
        {
            server->players[attackTarget]->destroySheild();
            if (skill == 1)
            {
                countDamage(grudge,Magic);
            }
        }
        else
        {
           emit attacked(order, attackTarget, damage);
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }

    if (skill == 2)
    {
        countDamage(grudge,Magic);
    }

    skill = 0;
    target = -1;
    disconnect(server->players[attackTarget],SIGNAL(miss(int)),this,SLOT(skillone(int)));

    emit finishaction(order, Attack);
}

void Flighter::magicAction()
{
    if (activation == 1)
    {
        activation = 0;
        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 0;
        BroadCast();
    }
    Player::magicAction();
//------------念弹--------------------
    if (grudge != 6)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;
        sendMessage();

        receive();

        if (receiveMessageBuffer[0])
        {
            receive();
            if (receiveMessageBuffer[0] != -1)
            {
               grudgeChange(1);
               int magicTarget = receiveMessageBuffer[0];
               int damage = 1;
               bool a = server->players[magicTarget]->cureExist();
               server->players[magicTarget]->countDamage(damage,Magic);
               if (!a)
               {
                   countDamage(grudge,Magic);
               }
            }
        }
    }
}

void Flighter::skillone(int order)
{
    if (order == target && skill == 1)
    {
        countDamage(grudge,Magic);
    }
}

bool Flighter::canActivate()
{
    return (!((cardLimit - cardNumber) < 3 && ((activation == 1||grudge < 3) && energyCrystal+energyGem == 0)
                &&(energyCrystal+energyGem==stonelimit || server->team[teamNumber]->stone==0)));
}

