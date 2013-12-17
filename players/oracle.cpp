#include "oracle.h"
#include"server.h"
extern CardList cardlist;

/*
 *1.普通技：响应C 【神圣启示】：（【特殊行动】结束时发动）你+1【治疗】。
  2.普通技：法术 【神圣祈福】：（弃2张法术牌【展示】）你+2【治疗】。
  3.普通技：法术 【水之神力】：（弃1张水系牌【展示】）将手中的一张牌交给目标队友【强制】，你和他各+1【治疗】。
    普通技：被动 【圣使守护】：你的【治疗】上限+4，你每次用【治疗】去抵御伤害时，最多只能使用1点。
    必杀技：启动 【神圣契约】：【能量】×1 将你的X【治疗】转移给一名队友，以此法所转移的【治疗】无视他的【治疗】上限，但他的【治疗】最高为4。
  4.必杀技：法术 【神圣领域】：【能量】×1 你弃2张牌，再选择一项：
●（移除你的1【治疗】）对目标角色造成2点法术伤害③。
●你+2【治疗】，目标队友+1【治疗】。

 */

Oracle::Oracle(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    activation = 0;
    cureLimit = cureLimit + 4;
    server->textg->character[order]->setText("神官");
}

void Oracle::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 2)//神圣祈福
    {
         magicOne();
         emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 3)//水之神力
    {
         magicTwo();
         emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 5)//神圣领域
    {
        magicThree();
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 6)//神圣领域
    {
        magicFour();
        emit finishaction(order, Magic);
    }
}

bool Oracle::canActivate()
{
   return  (!((cardLimit - cardNumber) < 3 && (energyCrystal+energyGem == 0)
                &&(energyCrystal+energyGem==stonelimit || server->team[teamNumber]->stone==0)));
}

void Oracle::beforeAction()
{
    if (!canActivate())
    {
        return;
    }
    //int receiveMessageBuffer[20];
    sendMessageBuffer[0] = BeforeAction;
    sendMessage();

    receive();
    int returnAction = receiveMessageBuffer[0];
    switch (returnAction)
    {
    case (Activate):
    {
        activate();
        break;
    }
    case (Purchase):
    {
        this->purchase();

        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;

        sendMessage();

        receive();

        if (receiveMessageBuffer[0])
        {
        server->textg->textbrowser->append("你发动了神圣启示");
        increaseCure(1);
        }
        throw 1;
        break;
    }
    case (Fusion):
    {
        this->fusion();

        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;

        sendMessage();

        receive();

        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你发动了神圣启示");
        increaseCure(1);
        }
        throw 1;
        break;
    }
    case (Refine):
    {
        this->refine();

        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;

        sendMessage();

        receive();

        if (receiveMessageBuffer[0])
        {
        server->textg->textbrowser->append("你发动了神圣启示");
        increaseCure(1);
        }
        throw 1;
        break;
    }
    }
    return;
}

void Oracle::activate()
{
    receive();

    if (receiveMessageBuffer[0] != -1)
    {
       useEnergy(1);

       int target = receiveMessageBuffer[0];
       int cure = receiveMessageBuffer[1];

       decreaseCure(cure);

       if (server->players[target]->cureNumber + cure > 4)
       {
           server->players[target]->increaseCure(4-server->players[target]->cureNumber,false);
       }
       else
       {
           server->players[target]->increaseCure(cure,false);
       }
    }
}

void Oracle::magicOne()
{
    server->textg->textbrowser->append("你发动了神圣祈福");
    int cardUsed[2];
    cardUsed[0] = receiveMessageBuffer[2];
    cardUsed[1] = receiveMessageBuffer[3];

    foldCard(cardUsed,2,true);

    increaseCure(2);
}

void Oracle::magicTwo()
{
    server->textg->textbrowser->append("你发动了水之神力");

    int cardUsed[2];
    cardUsed[0] = receiveMessageBuffer[2];

    receive();
    int attackTarget = receiveMessageBuffer[2];
    cardUsed[1] = receiveMessageBuffer[3];
    int cardGiven = receiveMessageBuffer[3];

    foldCard(cardUsed,2);

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed[0];
    BroadCast();

    increaseCure(1);
    server->players[attackTarget]->increaseCure(1);

    server->players[attackTarget]->card.insert(cardGiven);
    server->players[attackTarget]->sendMessageBuffer[2] = cardGiven;

    server->players[attackTarget]->cardNumber++;
    int a = server->players[attackTarget]->cardNumber;
    int b = server->players[attackTarget]->cardLimit;
    server->players[attackTarget]->sendMessageBuffer[0] = GetCard;
    server->players[attackTarget]->sendMessageBuffer[1] = 1;
    server->players[attackTarget]->sendMessage();

    server->players[attackTarget]->sendMessageBuffer[0] = CardChange;
    server->players[attackTarget]->sendMessageBuffer[1] = 1;
    server->players[attackTarget]->BroadCast();//改变手牌数量

    if (a > b)
    {
        server->players[attackTarget]->takeDamage(a-b,2);
    }

}
void Oracle::magicThree()
{
    server->textg->textbrowser->append("你发动了神圣领域");
    int magicTarget = receiveMessageBuffer[2];

    if (cardNumber > 1)
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

    useEnergy(1);
    decreaseCure(1);
   //展示攻击对象，攻击牌
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = 0;
    sendMessageBuffer[3] = magicTarget;
    BroadCast();

    int damage = 2;
    server->players[magicTarget]->countDamage(damage,Magic);

}

void Oracle::magicFour()
{
    server->textg->textbrowser->append("你发动了神圣领域");
    int magicTarget = receiveMessageBuffer[2];

    if (cardNumber > 1)
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

    useEnergy(1);
   //展示攻击对象，攻击牌
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = 0;
    sendMessageBuffer[3] = magicTarget;
    BroadCast();

    increaseCure(2);
    server->players[magicTarget]->increaseCure(1);
}


