#include "blademaster.h"
#include "cardList.h"
#include"server.h"
#include"card.h"

extern CardList cardlist;

Blademaster::Blademaster(/*QObject *parent*/Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    combo= false;
    shadowOfSword = false;
    attackTime = 0;
    skill = 0;
}
/*
void Blademaster::attack()
{
    normalAttack();
}
*/
void Blademaster::end()
{
    combo= false;
    shadowOfSword = false;
    attackTime = 0;
    skill = 0;
}

bool Blademaster::canAttack()
{
    for (int i=0; i<cardNumber; i++)
    {
        //if (cardlist.getType(card[i]) == attack)
        return true;
    }
    return false;
}

bool Blademaster::hasWind()
{
    set<int>::iterator i;
    for (i=card.begin(); i!=card.end(); i++)
    {
        if (cardlist.getName(*i) == windAttack)
        return true;
    }
    return false;
}

void Blademaster::normalAttack()
{
    attackTime++;
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];

    foldCard(&cardUsed);
    bool canattack = canAttack();


    //-------------------烈风技------------------------------------------------
    if (cardlist.getSkillOne(cardUsed) == 11 ||cardlist.getSkillTwo(cardUsed) == 11)
    {
        if (server->players[attackTarget]->shieldExist())
        {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 2;
        sendMessage();

        receive();
        if (receiveMessageBuffer[0])
        {
            skill = 1;
        }
        }
    }
    //---------------------疾风技----------------------------------------------
    else if (cardlist.getSkillOne(cardUsed) == 12 ||cardlist.getSkillTwo(cardUsed) == 12)
    {
        if (canattack)
        {
           sendMessageBuffer[0] = AskRespond;
           sendMessageBuffer[1] = 1;
           sendMessageBuffer[2] = 3;
           sendMessage();
           receive();
           if (receiveMessageBuffer[0])
           {
               skill = 2;
           }
        }
    }
    //------------------------------------------------------------------------------
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;
    //------------烈风技--------------------------
    if(skill == 1)
        canBeAccept = false;
    //------------------------------------------
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    sendMessageBuffer[3] = skill;
    BroadCast();//展示攻击对象，攻击牌

    if (attackTime == 3)
    {
        if(server->players[attackTarget]->shieldExist() && skill != 1)//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
            (server->team[teamNumber])->getStone(Gem);
            server->players[attackTarget]->countDamage(2);
        }
    }
    else if(server->players[attackTarget]->beAttacked(cardUsed,order,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist() && skill != 1)//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
            (server->team[teamNumber])->getStone(Gem);
            server->players[attackTarget]->countDamage(2);
        }
    }
//-----------------结束前---------------------------------------------
    if (skill == 2)
    {
        skill = 0;
        normalAttack();
    }
    else if (canattack)
    {
        skill = 0;
        int i = 0;
        if (hasWind() && !combo)
        {
            i++;
            sendMessageBuffer[i+1] = 1;
        }
        if ((energyGem+energyCrystal >= 1) && !shadowOfSword)
        {
            i++;
            sendMessageBuffer[i+1] = 4;
        }
        sendMessageBuffer[1] = i;

        if (i >= 1)
        {
            sendMessage();
            receive();//是否发动技能

            switch (receiveMessageBuffer[0])
            {
              case 0:
                break;
              case 1:
              {
                    combo = true;
                    normalAttack();
                    break;
              }
              case 4:
              {
                   shadowOfSword = true;
                   if (receiveMessageBuffer[3] == 0)
                   energyGem--;
                   else
                   energyCrystal--;
                   normalAttack();
                   break;
              }
            }
        }
    }

}
