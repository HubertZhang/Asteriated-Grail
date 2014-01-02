#include "soulwarlock.h"
#include"server.h"
extern CardList cardlist;

/*
   普通技：被动 【灵魂吞噬】：（我方每有1点士气下降）你+1【黄色灵魂】。
 1.普通技：法术 【灵魂召还】：（弃X张法术牌【展示】）你+（X+1）【蓝色灵魂】。
 2.普通技：响应A 【灵魂转换】：（你每发动一次主动攻击①）可转换1点你拥有的【灵魂】的颜色。
 3.普通技：法术 【灵魂镜像】：（移除2点【黄色灵魂】）你弃3张牌，目标角色摸3张牌【强制】（但最多补到其手牌上限）。
   普通技：启动 【灵魂链接】：（移除1点【黄色灵魂】和1点【蓝色灵魂】，将【灵魂链接】放置于一名队友面前）
                          （每当你们之间将有人承受伤害时⑥，移除X点【蓝色灵魂】）将X点伤害转移给另一人，转移后的伤害为法术伤害⑥。
 4.独有技：法术 【灵魂震爆】：（移除3点【黄色灵魂】）对目标角色造成3点法术伤害③，若他手牌<3且手牌上限>5，则本次伤害额外+2。
 5.独有技：法术 【灵魂赐予】：（移除3点【蓝色灵魂】）目标角色+2【宝石】。
   必杀技：启动 【灵魂增幅】：【宝石】×1 你+2【黄色灵魂】和2【蓝色灵魂】。
*【黄色灵魂】和【蓝色灵魂】为灵魂术士专有指示物，上限各为6.

 */

Soulwarlock::Soulwarlock(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    yellow = 0;
    blue = 0;
    thetarget= -1;
    activation = 0;
    server->textg->character[order]->setText("灵魂");
}

void Soulwarlock::yellowChange(int number)
{
    if ((yellow + number) <= 6)
    {
       yellow = yellow + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[1] = number;
       sendMessageBuffer[2] = 0;
       BroadCast();
    }
    else if (yellow != 6)
    {
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[1] = 6-yellow;
        sendMessageBuffer[2] = 0;
        BroadCast();
        yellow = 6;
    }
    QString s;
    s.sprintf("你有%d黄魂",yellow);
    server->textg->textbrowser->append(s);
}
void Soulwarlock::blueChange(int number)
{
    if (blue + number <= 6)
    {
       blue = blue + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[2] = number;
       sendMessageBuffer[1] = 0;
       BroadCast();
    }
    else if (blue != 6)
    {
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[2] = 6-blue;
        sendMessageBuffer[1] = 0;
        BroadCast();
        blue = 6;
    }
    QString s;
    s.sprintf("你有%d蓝魂",blue);
    server->textg->textbrowser->append(s);
}
void Soulwarlock::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)
    {
        magicOne();//灵魂召还
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 4)
    {
        magicTwo();//灵魂镜像
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 5)
    {
        magicThree();//灵魂震爆
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 6)
    {
        magicFour();//灵魂赐予
        emit finishaction(order, Magic);
    }
}
void Soulwarlock::magicOne()
{
    server->textg->textbrowser->append("你使用了灵魂召还");
    int number = receiveMessageBuffer[2];
    foldCard(receiveMessageBuffer+3,number,true);

    blueChange(number+1);
}
void Soulwarlock::magicTwo()
{
    server->textg->textbrowser->append("你使用了灵魂镜像");
    int magicTarget = receiveMessageBuffer[2];

    foldCard(receiveMessageBuffer+3,3);
    yellowChange(-2);

    if (server->players[magicTarget]->cardLimit - server->players[magicTarget]->cardNumber < 3)
    {
        server->players[magicTarget]
        ->getCard(server->players[magicTarget]->cardLimit - server->players[magicTarget]->cardNumber);
    }
    else
    {
        server->players[magicTarget]->getCard(3);
    }
}
void Soulwarlock::magicThree()
{
    server->textg->textbrowser->append("你使用了灵魂震爆");
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];
    int damage = 3;
    foldCard(&cardUsed);

    if (server->players[magicTarget]->cardNumber < 3 && server->players[magicTarget]->cardLimit > 5)
    {
        damage = damage + 2;
    }

    yellowChange(-3);

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    server->players[magicTarget]->countDamage(damage,Magic);
}
void Soulwarlock::magicFour()
{
    server->textg->textbrowser->append("你使用了灵魂赐予");
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];
    foldCard(&cardUsed);

    blueChange(-3);

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    if ((server->players[magicTarget]->stonelimit- server->players[magicTarget]->energyCrystal - server->players[magicTarget]->energyGem) >= 2)
    {
         server->players[magicTarget]->energyGem = server->players[magicTarget]->energyGem + 2;
         server->players[magicTarget]->sendMessageBuffer[0] = EnergyChange;
         server->players[magicTarget]->sendMessageBuffer[1] = 2;
         server->players[magicTarget]->sendMessageBuffer[2] = 0;
         server->players[magicTarget]->BroadCast();//改变人物能量数量
    }
    else if  ((server->players[magicTarget]->stonelimit- server->players[magicTarget]->energyCrystal - server->players[magicTarget]->energyGem) == 1)
    {
        server->players[magicTarget]->energyGem = server->players[magicTarget]->energyGem + 1;
        server->players[magicTarget]->sendMessageBuffer[0] = EnergyChange;
        server->players[magicTarget]->sendMessageBuffer[1] = 1;
        server->players[magicTarget]->sendMessageBuffer[2] = 0;
        server->players[magicTarget]->BroadCast();//改变人物能量数量
    }
}
void Soulwarlock::characterConnect()
{
    connect(server->team[teamNumber],SIGNAL(moraleloss(int,int)),this,SLOT(skillone(int,int)));
    for (int i=0; i<6; i++)
    {
        if (server->players[i]->teamNumber == teamNumber)
        {
           connect(server->players[i],SIGNAL(bedamage1(int,int&,int)),this,SLOT(skilltwo(int,int&,int)));
        }
    }
}
void Soulwarlock::normalAttack()
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
    BroadCast();

    if (yellow + blue >= 1)
    {
        if (yellow >= 1 && blue >= 1)
        {
           sendMessageBuffer[0] = AskRespond;
           sendMessageBuffer[1] = 2;
           sendMessageBuffer[2] = 0;
           sendMessageBuffer[3] = 1;
        }
        else if (yellow >= 1 && blue == 0)
        {
            sendMessageBuffer[0] = AskRespond;
            sendMessageBuffer[1] = 1;
            sendMessageBuffer[2] = 0;
        }
        else if (yellow == 0 && blue >= 1)
        {
            sendMessageBuffer[0] = AskRespond;
            sendMessageBuffer[1] = 1;
            sendMessageBuffer[2] = 1;
        }

    sendMessage();
    receive();

    if (receiveMessageBuffer[0] == 2)
    {
        yellowChange(-1);
        blueChange(1);
    }
    else if (receiveMessageBuffer[0] == 3)
    {
        yellowChange(1);
        blueChange(-1);
    }
    }

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())
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
void Soulwarlock::activate()
{
    if (receiveMessageBuffer[1] == 0)
    {
//------------灵魂链接------------------------
        receive();
        if (receiveMessageBuffer[0] != -1)
        {
            activation = 1;
            thetarget = receiveMessageBuffer[0];
            QString s;
            s.sprintf("你使用了灵魂链接,对象%d",thetarget);
            server->textg->textbrowser->append(s);

            sendMessageBuffer[0] = Activated;
            sendMessageBuffer[1] = 1;
            BroadCast();

            yellowChange(-1);
            blueChange(-1);
        }
    }
    else
    {
//----------灵魂增幅---------------------------
        server->textg->textbrowser->append("你使用了灵魂增幅");
        useEnergy(1,true);
        yellowChange(2);
        blueChange(2);
    }
}

void Soulwarlock::skillone(int theteam,int morale)
{
    if (theteam == teamNumber)
    {
        server->textg->textbrowser->append("你使用了灵魂吞噬");
        yellowChange(morale);
    }
}
void Soulwarlock::skilltwo(int target, int& damage, int kind)
{
    if (thetarget != -1 && (kind == Attack || kind == Magic))
    {
        if (thetarget == target || order == target)
        {
            sendMessageBuffer[0] = AskRespond;
            sendMessageBuffer[1] = 1;
            sendMessageBuffer[2] = 2;
            sendMessageBuffer[3] = damage;
            sendMessage();
            receive();
            if (receiveMessageBuffer[0])
            {
                receive();
                if (receiveMessageBuffer[0] != -1)
                {
                   damage = damage - receiveMessageBuffer[0];

                   QString s;
                   s.sprintf("你转移了%d伤害",receiveMessageBuffer[0]);
                   server->textg->textbrowser->append(s);

                   if (target == order)
                   {
                       if ((6-server->playerOrder+order)%6 > (6-server->playerOrder+thetarget)%6)
                       {
                          server->players[thetarget]->takeDamage(receiveMessageBuffer[0],Magic);
                          takeDamage(damage,Magic);
                       }
                       else
                       {
                          takeDamage(damage,Magic);
                          server->players[thetarget]->takeDamage(receiveMessageBuffer[0],Magic);
                       }
                       damage = 0;
                   }
                   else if (target == thetarget)
                   {
                      if ((6-server->playerOrder+order)%6 > (6-server->playerOrder+thetarget)%6)
                      {
                          server->players[thetarget]->takeDamage(damage,Magic);
                          takeDamage(receiveMessageBuffer[0],Magic);
                      }
                      else
                      {
                          takeDamage(receiveMessageBuffer[0],Magic);
                          server->players[thetarget]->takeDamage(damage,Magic);
                      }
                      damage = 0;
                   }
                }
             }
        }
    }
}
