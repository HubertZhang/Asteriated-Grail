#include "berserker.h"
#include "server.h"
extern CardList cardlist;
/*狂战技能：
 *普通技：被动A 【狂化】：你发动的所有攻击伤害额外+1。
  1,独有技：响应A 【血影狂刀】：（作为主动攻击打出时发动）
  若命中手牌为2的对手②，本次攻击伤害额外+2；
  若命中手牌为3的对手②，本次攻击伤害额外+1。
  2,独有技：响应A 【血腥咆哮】：（作为主动攻击打出时发动）若攻击的目标拥有的【治疗】为2，则本次攻击强制命中。
  3,必杀技：响应A 【撕裂】：【宝石】×1 （攻击命中后发动②）本次攻击伤害额外+2。
 */

Berserker::Berserker(/*QObject *parent*/Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    skill = 0;
    server->textg->character[order]->setText("狂战");
}

void Berserker::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    //assert(this->card.find(cardUsed)!=this->card.end());
    //if(!server->players[attackTarget].canBeAttacked()) throw ActionIllegal();
    int damage = 3;
    foldCard(&cardUsed);
    //暗灭无法应战，需要吗？
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

//-------------------血影狂刀------------------------------------------------
    if (cardlist.getSkillOne(cardUsed) == 21 ||cardlist.getSkillTwo(cardUsed) == 21)
    {
        if (server->players[attackTarget]->cardNumber == 2 ||
            server->players[attackTarget]->cardNumber == 3)
        {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;

        sendMessage();
        receive();
        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你响应了血影狂刀");
            skill = 1;
        }
        }
    }
//------------血腥咆哮-------------------------------------------------------------
    else if (cardlist.getSkillOne(cardUsed) == 22 ||cardlist.getSkillTwo(cardUsed) == 22)
    {
        if (server->players[attackTarget]->cureNumber == 2)
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
            server->textg->textbrowser->append("你响应了血腥咆");
            skill = 2;
        }
        }
    }
//--------------------------------------------------------------------------------

    //BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();//展示攻击对象，攻击牌

    if (skill == 2)
    {
        (server->team[teamNumber])->getStone(Gem);
        if (energyGem >= 1)
        {
            sendMessageBuffer[0] = AskRespond;
            sendMessageBuffer[1] = 1;
            sendMessageBuffer[2] = 2;

            sendMessage();

            receive();
            if (receiveMessageBuffer[0])
            {
                server->textg->textbrowser->append("你响应了撕裂");
                damage = damage + 2;

                useEnergy(1,true);
            }
        }
        emit attacked(order, attackTarget, damage);
        server->players[attackTarget]->countDamage(damage,Attack);
    }

    else if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
            if (skill == 1)
            {
                damage = damage + 4 - server->players[attackTarget]->cardNumber;
            }
            if (energyGem >= 1)
            {
                sendMessageBuffer[0] = AskRespond;
                sendMessageBuffer[1] = 1;
                sendMessageBuffer[2] = 2;

                sendMessage();
                //测试----------------------------
                getmessage = false;
                //--------------------------------
                receive();
                if (receiveMessageBuffer[0])
                {
                    server->textg->textbrowser->append("你响应了撕裂");
                    damage = damage + 2;

                    useEnergy(1,true);

                }
            }
           emit attacked(order, attackTarget, damage);
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
    emit finishaction(order, Attack);
}

void Berserker::headOn(int chainLength)
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 3;

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

void Berserker::end()
{
    skill = 0;
}

