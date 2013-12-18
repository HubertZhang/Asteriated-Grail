#include "swordking.h"
#include"server.h"
extern CardList cardlist;

/*
 *1.普通技：响应A 【剑魂守护】：（主动攻击未命中时发动②）将本次打出的攻击牌面朝下放置在你的角色旁，作为【剑魂】。
    普通技：被动A 【佯攻】：（主动攻击未命中时发动②）你+1【剑气】。
  2.普通技：响应A 【剑气斩】：（主动攻击命中后发动②，移除X点【剑气】，X最高为3）对除你所攻击的目标以外的任意一名角色造成X点法术伤害③。
    普通技：被动 【天使与恶魔】：若你现有【能量】为单数，你的所有【剑魂】视为【天使之魂】；若为双数，视为【恶魔之魂】；若没有【能量】，则不属于任何一种。
  3.普通技：响应A 【天使之魂】：（主动攻击前发动①，移除一个【天使之魂】）本次攻击若命中②，你+2【治疗】；若未命中②，我方+1士气；不能和【剑魂守护】同时发动。
  4.普通技：响应A 【恶魔之魂】：（主动攻击前发动①，移除一个【恶魔之魂】）本次攻击伤害额外+1；若未命中②，你+2【剑气】；不能和【剑魂守护】同时发动。
  5.必杀技：响应C 【不屈意志】：【能量】×1 （【攻击行动】结束时发动）你摸1张牌【强制】,+1【剑气】,额外+1【攻击行动】。

 */

Swordking::Swordking(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    swordsoul = 0;
    swordkee = 0;
    skill = 0;
    target = -1;
    server->textg->character[order]->setText("剑帝");
}

void Swordking::swordsoulChange(int number)
{
    if ((swordsoul + number) <= 3)
    {
       swordsoul = swordsoul + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[2] = number;
       sendMessageBuffer[1] = 0;
       BroadCast();
    }
    else if (swordsoul != 3)
    {
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[2] = 3-swordsoul;
        sendMessageBuffer[1] = 0;
        BroadCast();
        swordsoul = 3;
    }
}

void Swordking::swordkeeChange(int number)
{
    if (swordkee + number <= 5)
    {
       swordkee = swordkee + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[1] = number;
       sendMessageBuffer[2] = 0;
       BroadCast();
    }
    else if (swordkee != 5)
    {
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[2] = 5-swordkee;
        sendMessageBuffer[1] = 0;
        BroadCast();
        swordkee = 5;
    }
}

void Swordking::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;
    connect(server->players[attackTarget],SIGNAL(miss(int)),this,SLOT(skillone(int)));
    target = attackTarget;

    if (skill == 5)
    {
        useEnergy(1);
        takeDamage(1,2);
        swordkeeChange(1);
        skill = 0;
    }

    foldCard(&cardUsed);

    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

//-----------天使之魂/恶魔之魂-----------------------
    if (swordsoul >= 1 && energyCrystal+energyGem>=1)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        if ((energyCrystal+energyGem)%2 == 1)
        {
            sendMessageBuffer[2] = 2;
        }
        else
        {
            sendMessageBuffer[2] = 3;
        }

        sendMessage();

        receive();
        if (receiveMessageBuffer[0])
        {
            swordsoulChange(-1);
            if ((energyCrystal+energyGem)%2 == 1)
            {
                server->textg->textbrowser->append("你响应了天使之魂");
                skill = 1;
            }
            else if ((energyCrystal+energyGem) == 2)
            {
                server->textg->textbrowser->append("你响应了恶魔之魂");
                skill = 2;
                damage++;
            }
        }
    }
//-------------------------------------------------

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())
        {
            server->players[attackTarget]->destroySheild();
            skillone(attackTarget);
        }
        else
        {
//-----------天使之魂---------------------------
            if (skill == 1)
            {
                increaseCure(2);
            }
//-----------剑气斩-----------------------------
            if (swordkee >= 1)
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
                        int magicTarget = receiveMessageBuffer[0];
                        int swordkeenumber = receiveMessageBuffer[1];
                        int damage = swordkeenumber;

                        swordkeeChange(-swordkeenumber);

                        server->players[magicTarget]->countDamage(damage,Magic);
                    }
                }
            }
//-------------------------------------------------
           emit attacked(order, attackTarget, damage);
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }

    skill = 0;
    target = -1;
    disconnect(server->players[attackTarget],SIGNAL(miss(int)),this,SLOT(skillone(int)));

    emit finishaction(order, Attack);

    if (energyCrystal+energyGem >= 1)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 4;
        sendMessage();
        receive();//是否发动技能

        if (receiveMessageBuffer[0] == 1)
        {
           sendMessageBuffer[0] = AdditionalAction;
           sendMessageBuffer[1] = 0;
           sendMessage();

           receive();

           if (receiveMessageBuffer[0] == 0)
           {
               skill = 5;
               normalAttack();
           }
        }
    }
}

void Swordking::skillone(int order)
{
    if (order == target)
    {
        if (skill != 1 && skill != 2)
        {
        //---------------------剑魂守护-----------------------------------------
           sendMessageBuffer[0] = AskRespond;
           sendMessageBuffer[1] = 1;
           sendMessageBuffer[2] = 0;

           sendMessage();

           receive();

           if (receiveMessageBuffer[0])
           {
               swordsoulChange(1);
           }
        }
        //---佯攻--------------------
        swordkeeChange(1);
        //---天使之魂or恶魔之魂-----------
        if (skill == 1)
        {
            if (server->team[teamNumber]->morale<15)
            {
                server->team[teamNumber]->morale++;
                server->team[teamNumber]->BroadCast();
            }
        }
        else if (skill == 2)
        {
            swordkeeChange(2);
        }

        target = -1;
    }
}


