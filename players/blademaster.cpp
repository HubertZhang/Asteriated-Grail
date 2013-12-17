#include "blademaster.h"
#include"server.h"
#include <QApplication>

extern CardList cardlist;
/*剑圣技能：
 * 1【连续技】：（【攻击行动】结束时发动）额外+1风系【攻击行动】。
   【圣剑】：若你的主动攻击为你本次行动阶段的第三次【攻击行动】，则此攻击强制命中。
   2【烈风技】：（攻击的目标拥有圣盾时发动）无视对方圣盾的效果，且此攻击对方无法应战。
   3【疾风技】：（作为主动攻击打出时发动）额外+1【攻击行动】。
   4【剑影】：【能量】×1 （【攻击行动】结束时发动）额外+1【攻击行动】。
*/


Blademaster::Blademaster(/*QObject *parent*/Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    combo= false;
    shadowOfSword = false;
    attackTime = 0;
    skill = 0;
    server->textg->character[order]->setText("剑圣");
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
    for (int i=0; i<20; i++)
    {
        receiveMessageBuffer[i] = -1;
    }
    getmessage = false;
}

bool Blademaster::canAttack()
{
    set<int>::iterator i;
    for (i=card.begin(); i!=card.end(); i++)
    {
        if (cardlist.getType(*i) == attack)
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
    int damage = 2;

    foldCard(&cardUsed);
    bool canattack = canAttack();


    //-------------------烈风技------------------------------------------------
    if (cardlist.getSkillOne(cardUsed) == 11 ||cardlist.getSkillTwo(cardUsed) == 11)
    {
        if (server->players[attackTarget]->shieldExist())
        {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;

        sendMessage();
        //测试----------------------------
        getmessage = false;
        //--------------------------------
        receive();
        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你响应了烈风技");
            skill = 2;
        }
        }
    }
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;
    //------------烈风技--------------------------
    if(skill == 2)
        canBeAccept = false;
    //------------------------------------------
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;

    BroadCast();//展示攻击对象，攻击牌

    if (attackTime == 3)
    {
        server->textg->textbrowser->append("圣剑!!");
         emit attacked(order, attackTarget, damage);
        (server->team[teamNumber])->getStone(Gem);
        server->players[attackTarget]->countDamage(damage,Attack);
    }
    else if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist() && skill != 2)//If there is a shield...
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
//-----------------结束前---------------------------------------------

    emit finishaction(order, Attack);

    if (canattack)
    {
        skill = 0;
        int i = 0;
        if (hasWind() && !combo)
        {
            i++;
            sendMessageBuffer[i+1] = 0;//连续技
        }
        if (cardlist.getSkillOne(cardUsed) == 12 ||cardlist.getSkillTwo(cardUsed) == 12)
        {
             i++;
             sendMessageBuffer[i+1] = 2;//疾风技
        }
        if (((energyGem+energyCrystal) >= 1) && !shadowOfSword)
        {
            i++;
            sendMessageBuffer[i+1] = 3;//剑影
        }
        sendMessageBuffer[1] = i;//技能个数

        if (i >= 1)
        {
            sendMessageBuffer[0] = AskRespond;
            sendMessage();

            receive();//是否发动技能

            int temp = receiveMessageBuffer[0];

            if (temp != 0)
            {
               receive();

               if (receiveMessageBuffer[0] != -1)
               {
                  receiveMessageBuffer[2] = receiveMessageBuffer[1];
                  receiveMessageBuffer[1] = receiveMessageBuffer[0];
                  switch (temp)
                  {
                  case 3:
                  {
                     server->textg->textbrowser->append("你响应了疾风技");
                     normalAttack();
                     break;
                  }
                  case 1:
                  {
                     server->textg->textbrowser->append("你响应了连续技");
                     combo = true;
                     normalAttack();
                     break;
                  }
                  case 4:
                  {
                     server->textg->textbrowser->append("你响应了剑影");
                     shadowOfSword = true;

                     useEnergy(1);
                     normalAttack();
                     break;
                  }
                  default:
                     break;
            }
            }
            }
        }
    }

}

void Blademaster::headOn(int chainLength)
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

    //-------------------烈风技------------------------------------------------
    if (cardlist.getSkillOne(cardUsed) == 11 ||cardlist.getSkillTwo(cardUsed) == 11)
    {
        if (server->players[attackTarget]->shieldExist())
        {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;

        sendMessage();
        //测试----------------------------
        getmessage = false;
        //--------------------------------
        receive();
        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你响应了烈风技");
            skill = 2;
        }
        }
    }

    //------------烈风技--------------------------
    if(skill == 2)
        canBeAccept = false;
    //------------------------------------------

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    //BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    emit miss(order);
\
    if(server->players[attackTarget]->beAttacked(order,cardUsed,chainLength,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist()&& skill != 2)//If there is a shield...
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

    skill = 0;
}
