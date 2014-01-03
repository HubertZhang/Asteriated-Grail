#include "brave.h"
#include"server.h"
extern CardList cardlist;

/*
 *普通技：被动 【勇者之心】：游戏初始时，你+2【水晶】。
  0.普通技：响应A 【怒吼】：（主动攻击前发动①，移除1点【怒气】）本次攻击伤害额外+2；若未命中，你+1【知性】。
  普通技：法术 【挑衅】：（移除1点【怒气】，将【挑衅】放置于目标对手面前）你+1【知性】；
                         该对手在其下个行动阶段必须且只能主动攻击你，否则他跳过该行动阶段，触发后移除此牌。
  普通技：被动A 【精疲力竭】：（发动【禁断之力】后强制触发【强制】）【横置】额外+1【攻击行动】；
                                持续到你的下个行动阶段开始，你的手牌上限恒定为4【恒定】。
                               【精疲力竭】的效果结束时【重置】，并对自己造成3点法术伤害③。
  1.普通技：响应A 【明镜止水】：（主动攻击前发动①，移除4点【知性】）本次攻击对方不能应战。
  2.必杀技：响应A 【禁断之力】：【能量】×1 （主动攻击命中或未命中后发动②）
                                     弃掉你所有手牌【展示】，其中每有1张法术牌，你+1点【怒气】；
                                      若未命中②，其中每有1张水系牌，你+1点【知性】；
                                      若命中②，其中每有1张火系牌，本次攻击伤害额外+1，
                                      并对自己造成等同于火系牌数量的法术伤害③。
  3.必杀技：响应D 【死斗】：【宝石】×1 （每当你承受法术伤害时发动⑥）你+3点【怒气】。
*【怒气】和【知性】为勇者专有指示物，上限各为4。
 */
Brave::Brave(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    dander =0;
    intellect = 0;
    skill = 0;
    target = -1;
    activation = 0;
    combo = 0;
    server->textg->character[order]->setText("勇者!");
}

void Brave::characterConnect()
{
//------------勇者之心----------------------
      energyCrystal = energyCrystal + 2;

      sendMessageBuffer[0] = EnergyChange;
      sendMessageBuffer[1] = 0;
      sendMessageBuffer[2] = 2;
      BroadCast();//改变人物能量数量
}

void Brave::danderChange(int number)
{
    if ((dander + number) <= 4)
    {
       dander = dander + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[1] = number;
       sendMessageBuffer[2] = 0;
       BroadCast();
    }
    else if (dander != 4)
    {
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[1] = 4-dander;
        sendMessageBuffer[2] = 0;
        BroadCast();
        dander = 4;
    }
}


void Brave::changeCardLimit2(int amount)
{
    if (activation == 1)
    {
       return;
    }
    Player::changeCardLimit2(amount);
}

void Brave::intellectChange(int number)
{
    if (intellect + number <= 4)
    {
       intellect = intellect + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[2] = number;
       sendMessageBuffer[1] = 0;
       BroadCast();
    }
    else if (intellect != 4)
    {
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[2] = 4-intellect;
        sendMessageBuffer[1] = 0;
        BroadCast();
        intellect = 4;
    }
}

void Brave::takeDamage(int damage, int kind)
{
    if (damage > 0)
    {
    getCard(damage);

    if(cardNumber > cardLimit)
    {
        Discards(cardNumber-cardLimit,kind);
    }

//------------死斗----------------------
    if (energyGem >= 1 && kind == Magic && damage >= 1)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 3;

        sendMessage();
        receive();

        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你发动了死斗");
            useEnergy(1,true);
            danderChange(3);
        }
    }
    }
}

void Brave::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;
    connect(server->players[attackTarget],SIGNAL(miss(int)),this,SLOT(skillone(int)));
    target = attackTarget;

    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

    if (dander >= 1)
    {
//------------怒吼-----------------------------
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;
        sendMessage();
        receive();

        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你发动了怒吼");
            skill = 1;
            damage = damage + 2;
            danderChange(-1);
        }

    }
    if (intellect >= 4)
    {
//-----------明镜止水---------------------
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;
        sendMessage();
        receive();

        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你发动了明镜止水");
            canBeAccept = false;
            intellectChange(-4);
        }
    }

    foldCard(&cardUsed);

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
            skillone(attackTarget);
        }
        else
        {
            if (energyCrystal + energyGem >= 1)
            {
//-------------------------禁断之力------------------------------
                sendMessageBuffer[0] = AskRespond;
                sendMessageBuffer[1] = 1;
                sendMessageBuffer[2] = 2;
                sendMessage();
                receive();

                if (receiveMessageBuffer[0])
                {
                    server->textg->textbrowser->append("你发动了禁断之力");
                    useEnergy(1);
                    int number = receiveMessageBuffer[1];

                    int magicnumber = 0;
                    int firenumber = 0;
                    set<int>::iterator i;
                    for (i=card.begin(); i!=card.end(); i++)
                    {
                        if (cardlist.getType(*i) == magic)
                        {
                            magicnumber++;
                        }
                        if (cardlist.getNature(*i) == fire)
                        {
                            firenumber++;
                        }
                    }

                    foldCard(receiveMessageBuffer+2,number,true);

//--------------------精疲力竭-----------------------------------------
                    activation = 1;
                    sendMessageBuffer[0] = Activated;
                    sendMessageBuffer[1] = 1;
                    BroadCast();

                    changeCardLimit1(4);

                    if (cardLimit < cardNumber)
                    {
                         Discards(cardNumber-cardLimit,2);
                    }


                    danderChange(magicnumber);
                    damage = damage + firenumber;
                    countDamage(firenumber,Magic);
                    combo++;
                }
            }
           emit attacked(order, attackTarget, damage);
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }

    skill = 0;
    target = -1;
    disconnect(server->players[attackTarget],SIGNAL(miss(int)),this,SLOT(skillone(int)));
    emit finishaction(order, Attack);

    if (combo >= 1)
    {
        combo--;
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

void Brave::beforeAction()
{
    if (activation == 1)
    {
        activation = 0;

        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 0;
        BroadCast();

        changeCardLimit1(6);
        countDamage(3,Magic);
    }

    Player::beforeAction();
}

void Brave::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)//挑衅
    {
         magicOne();
         emit finishaction(order, Magic);
    }
}

void Brave::magicOne()
{
    server->textg->textbrowser->append("你发动了挑衅");

    int magicTarget = receiveMessageBuffer[1];

    danderChange(-1);
    intellectChange(1);

    server->players[magicTarget]->addStatus(151);
}

void Brave::skillone(int order)
{
    if (order == target && skill == 1)
    {
        intellectChange(1);
    }

    if (energyCrystal + energyGem >= 1 && order == target)
    {
//-------------------------禁断之力------------------------------
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 2;
        sendMessage();
        receive();

        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你发动了禁断之力");
            useEnergy(1);
            int number = receiveMessageBuffer[1];

            int magicnumber = 0;
            int waternumber = 0;
            set<int>::iterator i;
            for (i=card.begin(); i!=card.end(); i++)
            {
                if (cardlist.getType(*i) == magic)
                {
                    magicnumber++;
                }
                if (cardlist.getNature(*i) == water)
                {
                    waternumber++;
                }
            }

            foldCard(receiveMessageBuffer+2,number,true);
//----------------------精疲力竭----------------------------------
            activation = 1;
            sendMessageBuffer[0] = Activated;
            sendMessageBuffer[1] = 1;
            BroadCast();

            changeCardLimit1(4);

            if (cardLimit < cardNumber)
            {
                 Discards(cardNumber-cardLimit,2);
            }

            danderChange(magicnumber);
            intellectChange(waternumber);

            combo++;
        }
    }
}


