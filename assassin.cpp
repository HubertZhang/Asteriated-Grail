#include "assassin.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
#include"player.h"
extern CardList cardlist;

/*
   普通技：被动D 【反噬】：（承受攻击伤害时发动⑥）攻击你的对手摸1张牌【强制】。
   1,普通技：响应D 【水影】：（任何人对你造成伤害时发动③）弃X张水系牌【展示】。
   2,必杀技：启动 【潜行】：【宝石】×1 【横置】持续到你的下个行动阶段开始，你的手牌上限-1；
   你不能成为主动攻击的目标；你的主动攻击对方无法应战且伤害额外+X，X为你剩余的【能量】数。【潜行】的效果结束时【重置】。

 */
Assassin::Assassin(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    Attacker = -1;
    activation = 1;
    server->textg->character[order]->setText("暗杀");
}

void Assassin::takeDamage(int damage,int kind)
{
    getCard(damage);
//--------------反噬----------------------
    if (kind == Attack)
    {
        server->textg->textbrowser->append("你使用了反噬");
        server->players[Attacker]->takeDamage(1,2);
    }
//----------------------------------------
    Attacker = -1;

    if(cardNumber > cardLimit)
    {
        Discards(cardNumber-cardLimit,kind);
    }

}

void Assassin::countDamage(int damage,int kind)
{
//-----------水影-----------------------------
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
            server->textg->textbrowser->append("你响应了水影");
            foldCard(receiveMessageBuffer+2,receiveMessageBuffer[1],true);
        }
        //server->textg->textbrowser->append("???");

//-------------------------------------------

    if(cureExist())
    {
        int realDamage = damage - useCure(damage);
        if (realDamage > 0)
        takeDamage(realDamage,kind);
    }
    else
    {
        takeDamage(damage,kind);
    }
}

bool Assassin::beAttacked(int attacker, int cardUsed, int chainLength, bool canBeAccept)
{
    sendMessageBuffer[0] = AttackRespond;
    sendMessageBuffer[1] = cardUsed;
    sendMessageBuffer[3] = attacker;
    sendMessageBuffer[2] = canBeAccept;
    sendMessage();

    Attacker = attacker;
    //测试----------------------------
    getmessage = false;
    //--------------------------------
    receive();


    int reaction =receiveMessageBuffer[0];
    switch(reaction) //returnKind: accept,noAccept,offset
    {
    case AcceptAttack:
    {
        return true;
    }
    case HeadOn:
    {
        //if(cardList[cardUsed1].type()!=cardList[cardUsed].type()) throw
        this->headOn(chainLength+1);//there should also be a catch!
        return false;
    }
    case Light:
    {
        int cardUsed = receiveMessageBuffer[1];
        foldCard(&cardUsed,1,true);
        emit miss(order);
        //BroadCast();//改变手牌数量，展示圣光
        return false;
    }
    }
}
void Assassin::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    //assert(this->card.find(cardUsed)!=this->card.end());
    //if(!server->players[attackTarget].canBeAttacked()) throw ActionIllegal();

    int damage = 2;
    foldCard(&cardUsed);

    //暗灭无法应战，需要吗？
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

//----------潜行------------------------------------
    if (activation == 0)
    {
        server->textg->textbrowser->append("你在潜下攻击");
        damage = damage + energyCrystal + energyGem;
        canBeAccept = false;
    }
//--------------------------------------------------
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();//展示攻击对象，攻击牌

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
}
void Assassin::activate()
{
    activation = 0;
    changeCardLimit(-1);

    sendMessageBuffer[0] = Activated;
    sendMessageBuffer[1] = 1;
    BroadCast();

    useEnergy(1,true);

}
void Assassin::beforeAction()
{
    if (activation == 0)
    {
        activation = 1;
        changeCardLimit(1);

        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 0;
        BroadCast();
    }

    if ((cardLimit - cardNumber) < 3 && activation == 0 &&
            (energyCrystal+energyGem==3 || server->team[teamNumber]->stone==0))
    {
        return;
    }

    //int receiveMessageBuffer[20];
    sendMessageBuffer[0] = BeforeAction;
    sendMessage();
    //actionType returnAcction = receive(receiveMessageBuffer);
    //测试----------------------------
    getmessage = false;
    //--------------------------------
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
        throw 1;
        break;
    }
    case (Fusion):
    {
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
