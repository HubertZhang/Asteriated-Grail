#include "arbiter.h"
#include"server.h"
extern CardList cardlist;

/*
  普通技：被动 【仲裁法则】：游戏初始时，你+2【水晶】。
  普通技：启动 【仪式中断】：（仅【审判形态】下发动）【重置】脱离【审判形态】，我方【战绩区】+1【宝石】。
  1.普通技：法术 【末日审判】：（移除所有【审判】）对目标角色造成等量的法术伤害③；在你的行动阶段开始时，若【审判】已达到上限，该行动阶段你必须发动【末日审判】。
  普通技：被动D 【审判浪潮】：（你每承受一次伤害⑥）你+1【审判】。
  必杀技：启动 【仲裁仪式】【持续】：【宝石】×1 【横置】转为【审判形态】，你的手牌上限恒定为5【恒定】；每次在你的回合开始时，你+1【审判】。
  2.必杀技：法术 【判决天平】：【能量】×1 你+1【审判】，再选择一项：
  ●弃掉你的所有牌
  ●将你的手牌补到上限【强制】，我方【战绩区】+1【宝石】。
 */
Arbiter::Arbiter(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    activation = 0;
    trial = 0;
    server->textg->character[order]->setText("仲裁");
}

void Arbiter::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)
    {
        magicOne();//末日审判
    }
    else if (receiveMessageBuffer[1] == 2)
    {
        magicTwo();//判决天平1
    }
    else if (receiveMessageBuffer[1] == 3)
    {
        magicThree();//判决天平2
    }

}

void Arbiter::characterConnect()
{
//------------仲裁法则----------------------
    energyCrystal = energyCrystal + 2;

    sendMessageBuffer[0] = EnergyChange;
    sendMessageBuffer[1] = 0;
    sendMessageBuffer[2] = 2;
    BroadCast();//改变人物能量数量
}

bool Arbiter::canActivate()
{
    return (!((cardLimit - cardNumber) < 3 &&(activation == 0 && energyGem == 0)
            &&(energyCrystal+energyGem==stonelimit || server->team[teamNumber]->stone==0)));
}

void Arbiter::changeCardLimit(int amount)
{
    if (activation != 1)
    {
        cardLimit = cardLimit + amount;

        sendMessageBuffer[0] = CardLimitChange;
        sendMessageBuffer[1] = amount;
        BroadCast();
    }

}

void Arbiter::activate()
{
    if (sendMessageBuffer[1] == 1)
    {
//------------仲裁仪式------------------------
        activation = 1;
        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 1;
        BroadCast();

        sendMessageBuffer[0] = CardLimitChange;
        sendMessageBuffer[1] = 5 - cardLimit;
        BroadCast();

        cardLimit = 5;

        useEnergy(1,true);
    }
    else
    {
//----------仪式中断---------------------------
        activation = 0;
        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 0;
        BroadCast();

        sendMessageBuffer[0] = CardLimitChange;
        sendMessageBuffer[1] = 6 - cardLimit;
        BroadCast();

        cardLimit = 6;

        (server->team[teamNumber])->getStone(Gem);
    }
}

void Arbiter::magicOne()
{
    int magicTarget = receiveMessageBuffer[2];
    int damage = trial;

    trialIncrease(-trial);

    server->textg->textbrowser->append("你发动了末日审判");

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    BroadCast();

    server->players[magicTarget]->countDamage(damage,Magic);


    emit finishaction(order, Magic);
}

void Arbiter::start()
{
    sendMessageBuffer[0] = TurnBegin;
    BroadCast(); //回合开始

    try{

        if (activation == 1)
        {
            trialIncrease(1);
        }

    handleStatus();
    beforeAction();

    int i = 1;
    while(i--)
    {
        try{
        sendMessageBuffer[0] = ActionType;
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
        catch(ActionIllegal)
        {
            i++;//Do the While Loop again
        }
    }
    }
    catch(int)
    {

    }
    end();
}

void Arbiter::takeDamage(int damage,int kind)
{

    getCard(damage);

    if(cardNumber > cardLimit)
    {
        Discards(cardNumber-cardLimit,kind);
    }
    trialIncrease(1);
}

void Arbiter::trialIncrease(int number)
{
    if (trial + number <= 4)
    {
       trial = trial + number;

       sendMessageBuffer[0] = SpecialChange;
       sendMessageBuffer[1] = number;
       sendMessageBuffer[2] = 0;
       BroadCast();
    }
}

void Arbiter::magicTwo()
{
    server->textg->textbrowser->append("你发动了判决天平");
    useEnergy(1);
    trialIncrease(1);

    set<int>::iterator i;
    int j=0;
    int* c = new int[cardNumber];
    for (i=card.begin(); i!=card.end(); i++)
    {
         c[j] = *i;
         j++;
    }

    foldCard(c,cardNumber);


    emit finishaction(order, Magic);
}

void Arbiter::magicThree()
{
    server->textg->textbrowser->append("你发动了判决天平");
    useEnergy(1);
    trialIncrease(1);

    getCard(cardLimit - cardNumber);
    server->team[teamNumber]->getStone(Gem);

    emit finishaction(order, Magic);
}


