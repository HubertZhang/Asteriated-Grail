#include "prayer.h"
#include"server.h"
extern CardList cardlist;

/*
 *1.普通技：法术 【光辉信仰】：（仅【祈祷形态】下发动，移除1点【祈祷符文】）你弃2张牌，我方【战绩区】+1【宝石】，目标队友+1【治疗】。
  2.普通技：法术 【漆黑信仰】：（仅【祈祷形态】下发动，移除1点【祈祷符文】）对目标角色和自己各造成2点法术伤害③。
  3.独有技：法术 【威力赐福】：（将【威力赐福】放置于目标队友面前）该队友获得（攻击命中后可以移除此牌发动②）：本次攻击伤害额外+2。
  4.独有技：法术 【迅捷赐福】：（将【迅捷赐福】放置于目标队友面前）该队友获得（【法术行动】或【攻击行动】结束时可以移除此牌发动）：额外+1【攻击行动】。
    必杀技：启动 【祈祷】【持续】：【宝石】×1 【横置】转为【祈祷形态】，你每发动一次主动攻击①，你+2【祈祷符文】。
    必杀技：响应C 【法力潮汐】【回合限定】：【能量】×1 （【法术行动】结束后发动）额外+1【法术行动】。
*/

Prayer::Prayer(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    combo = false;
    activation = 0;
    for (int i=0; i<6; i++)
    {
        damageIncrease[i] = -1;
        actionIncrease[i] = -1;
    }
    server->textg->character[order]->setText("祈祷");
}

bool Prayer::canActivate()
{
    return (!((cardLimit - cardNumber) < 3 && (activation == 1 || energyGem == 0)
            &&(energyCrystal+energyGem==stonelimit || server->team[teamNumber]->stone==0)));
}

void Prayer::markChange(int number)
{
    if (mark + number <= 3)
    {
       mark = mark + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[1] = number;
       sendMessageBuffer[2] = 0;
       BroadCast();
    }
    else if (mark != 3)
    {
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[1] = 3 - mark;
        sendMessageBuffer[2] = 0;
        BroadCast();

        mark = 3;
    }
}
void Prayer::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)//光辉信仰
    {
         magicOne();
         emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 2)//漆黑信仰
    {
         magicTwo();
         emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 3)//威力赐福
    {
        magicThree();
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 4)//迅捷赐福
    {
        magicFour();
        emit finishaction(order, Magic);
    }
//--------- 法力潮汐--------------
    if (!combo && (energyCrystal+energyGem>= 1))
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;
        sendMessage();
        receive();//是否发动技能

        int temp = receiveMessageBuffer[0];

        if (temp != 0)
        {
           sendMessageBuffer[0] = AdditionalAction;
           sendMessageBuffer[1] = 1;
           sendMessage();

           receive();
           if (receiveMessageBuffer[0] != -1)
           {
               server->textg->textbrowser->append("你发动了法力潮汐");

               combo = true;
               useEnergy(1);
               magicAction();
           }
        }
    }
    combo = false;
}

void Prayer::magicOne()
{
    server->textg->textbrowser->append("你发动了光辉信仰");

    int magicTarget = receiveMessageBuffer[2];

    if (cardNumber >= 1)
    {
      int cardUsed[2];
      cardUsed[0] = receiveMessageBuffer[3];
      cardUsed[1] = receiveMessageBuffer[4];

      foldCard(cardUsed,2);
    }
    else if (cardNumber == 1)
    {
        int cardUsed = receiveMessageBuffer[3];
        foldCard(&cardUsed);
    }

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = 0;
    sendMessageBuffer[3] = magicTarget;
    BroadCast();

    markChange(-1);

    (server->team[teamNumber])->getStone(Gem);

    server->players[magicTarget]->increaseCure(1);
}
void Prayer::magicTwo()
{
    server->textg->textbrowser->append("你发动了漆黑信仰");

    int magicTarget = receiveMessageBuffer[2];
    int damage = 2;

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = 0;
    sendMessageBuffer[3] = magicTarget;
    BroadCast();

    markChange(-1);

    countDamage(damage,Magic);
    server->players[magicTarget]->countDamage(damage,Magic);
}
void Prayer::magicThree()
{
    server->textg->textbrowser->append("你发动了威力赐福");

    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    damageIncrease[magicTarget] = cardUsed;

    card.erase(cardUsed);
    cardNumber = this->card.size();

    sendMessageBuffer[0] = CardChange;
    sendMessageBuffer[1] = -1;
    BroadCast();

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    server->players[magicTarget]->addStatus(cardUsed);
}
void Prayer::magicFour()
{
    server->textg->textbrowser->append("你发动了迅捷赐福");

    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    actionIncrease[magicTarget] = cardUsed;

    card.erase(cardUsed);
    cardNumber = this->card.size();

    sendMessageBuffer[0] = CardChange;
    sendMessageBuffer[1] = -1;
    BroadCast();

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    server->players[magicTarget]->addStatus(cardUsed);
}

void Prayer::activate()
{
    activation = 1;

    sendMessageBuffer[0] = Activated;
    sendMessageBuffer[1] = 1;
    BroadCast();

    useEnergy(1,true);
}
void Prayer::normalAttack()
{
    if (activation == 1)
    {
        markChange(2);
    }

    Player::normalAttack();
}

void Prayer::characterConnect()
{
    for (int i=0; i<6; i++)
    {
        if (server->players[i]->teamNumber == teamNumber && i != order)
        {
           connect(server->players[i],SIGNAL(attacked(int,int,int&)),this,SLOT(skillone(int,int,int&)));
           connect(server->players[i],SIGNAL(finishaction(int, int)),this,SLOT(skilltwo(int,int)));
           connect(server->players[i],SIGNAL(decreasestatus(int,int)),this,SLOT(skillthree(int,int)));
        }
    }
}

void Prayer::skillthree(int order, int card)
{
    if (damageIncrease[order]  ==  card)
    {
        server->textg->textbrowser->append("威力赐福移除");
        damageIncrease[order] = -1;
    }

    if (actionIncrease[order] == card)
    {
        server->textg->textbrowser->append("迅捷赐福移除");
        actionIncrease[order] = -1;
    }
}

void Prayer::skillone(int attacker, int target, int& damage)
{
    if (damageIncrease[attacker] != -1)
    {
        server->players[attacker]->sendMessageBuffer[0] = SpecialAsk;
        server->players[attacker]->sendMessageBuffer[1] = 4;

        server->players[attacker]->sendMessage();
        server->players[attacker]->receive();

        if (server->players[attacker]->receiveMessageBuffer[0] == 1)
        {
            damage = damage + 2;
            server->players[attacker]->destroyStatus(damageIncrease[attacker]);
        }
    }
}

void Prayer::skilltwo(int target, int kind)
{
    if (actionIncrease[target] != -1)
    {
        server->players[target]->sendMessageBuffer[0] = SpecialAsk;
        server->players[target]->sendMessageBuffer[1] = 5;

        server->players[target]->sendMessage();
        server->players[target]->receive();

        if (server->players[target]->receiveMessageBuffer[0] == 0)
        {
            server->players[target]->sendMessageBuffer[0] = AdditionalAction;
            server->players[target]->sendMessageBuffer[1] = 0;
            server->players[target]->sendMessage();

            server->players[target]->receive();

            if (server->players[target]->receiveMessageBuffer[0] == 0)
            {
                server->players[target]->destroyStatus(actionIncrease[target]);
                server->players[target]->normalAttack();
            }
        }
    }
}





