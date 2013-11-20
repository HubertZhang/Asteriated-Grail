#include "player.h"
#include "server.h"
#include"cardList.h"
#include <assert.h>
#include"cardpile.h"
#include"textgui.h"
#include<QString>
#include<Windows.h>
#include <QApplication>
#include<QTimer>
#include<string>
using namespace std;
//extern CardList cardlist;
CardList cardlist;
//测试------------------------------------------------------
void Player::getMessage()
{
    string s = server->textg->lineEdit->text().toStdString();
    int i=0;
    int j=0;
    while(s[i]!='\0')
    {
        if (s[i] != ',')
        {
            int k=0;
            while (s[i+k] != ',' && s[i+k] != '\0')
            {
                k++;
            }
            int ss = 0;
            for (int t=0; t<k; t++)
            {
                int sum=1;
                for (int m=0; m<k-t-1; m++)
                {
                    sum = sum *10;
                }
                ss = ss + int((s[i+t]-'0')*sum);
            }
            receiveMessageBuffer[j] = ss;
            j++;
        }
        i++;
    }
    /*
    switch(sendMessageBuffer[0])
    {
        case BeforeAction:
    {
        break;
    }
    default:
        break;
    }
    //s.sprintf("player%d,team%d",order,teamNumber);
    //server->textg->textbrowser->append(s);
    */
    getmessage = true;
}
void Player::sendMessage()
{
    switch(sendMessageBuffer[0])
    {
    case TurnBegin:
    {
        QString s;
        s = "---------------------------------------";
        server->textg->textbrowser->append(s);
        s.sprintf("player%d's action:",order);
        server->textg->textbrowser->append(s);
        break;
    }
    case BeforeAction:
    {
        QString s;
        s = "特殊行动阶段,请选择:";
        server->textg->textbrowser->append(s);
        s = "0:什么也不做, 1:启动, 2:购买, 3:合成(宝石,水晶), 4:提炼(宝石,水晶)";
        server->textg->textbrowser->append(s);
        break;
    }
    case ActionType:
    {
        QString s;
        s = "行动阶段,请选择:";
        server->textg->textbrowser->append(s);
        s = "0:攻击(目标,卡牌), 1:魔法(技,目标,卡牌)";
        server->textg->textbrowser->append(s);
        break;
    }
    case GetCard:
    {
        QString s;
        s.sprintf("玩家%d 获得%d张卡牌 ",order,sendMessageBuffer[1]);
        server->textg->textbrowser->append(s);
        break;
        /*
        QString s;
        s.sprintf("玩家%d 获得卡牌:",order);
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
            QString s1 = cardlist.getQName(sendMessageBuffer[i+2]);
            //s1.sprintf("%d ",sendMessageBuffer[i+2]);
            s = s + s1;
        }
        server->textg->textbrowser->append(s);
        break;

        QString s2;
        set<int>::iterator i;
        int j;
        for (i=card.begin(),j=0; i!=card.end(); i++,j++)
        {
            s2.sprintf("%d",*i);
            server->textg->playercard[order][j]->setText(s2);
        }
*/
    }
    case WeakRespond:
    {
        server->textg->textbrowser->append("你被虚弱了,请选择:");
        server->textg->textbrowser->append("0:跳过回合,1:摸三张牌");
        break;
    }
    case AttackRespond:
    {
        QString s;
        //if(sendMessageBuffer[3])
        s.sprintf("玩家%d 请选择:",order);
        server->textg->textbrowser->append(s);
        server->textg->textbrowser->append("0:承受攻击,1:应战(对象,卡牌),2:圣光");
        break;
    }
    case CureRespond:
    {
        server->textg->textbrowser->append("请选择使用治疗数:");
        break;
    }
    default:
        break;

    }
}
void Player::BroadCast()
{
    switch(sendMessageBuffer[0])
    {
    case TurnBegin:
    {
     QString s;
     s = "----------------------------";
     server->textg->textbrowser->append(s);
     s.sprintf("玩家%d:",order);
     server->textg->textbrowser->append(s);
     break;
    }
    case Show:
    {
        QString s;
        s.sprintf("玩家%d 使用卡牌: ",order);
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
            QString s1 = cardlist.getQName(sendMessageBuffer[i+2]);
            s = s + s1;
        }
        server->textg->textbrowser->append(s);
        break;
    }
    case StatusIncrease:
    {
        QString s;
        s.sprintf("玩家%d添加",sendMessageBuffer[2],sendMessageBuffer[1]);
        s = s + cardlist.getQName(sendMessageBuffer[1]);
        server->textg->textbrowser->append(s);
        break;
    }
    case StatusDecrease:
    {
        QString s;
        s.sprintf("玩家%d‘s",order,sendMessageBuffer[1]);
        s = s + cardlist.getQName(sendMessageBuffer[1]);
        s = s + "移除";
        server->textg->textbrowser->append(s);
        int i;
        for (i=0; i<statusnumber; i++)
        {
            QString s1 = cardlist.getQName(status[i]);
            //s.sprintf("%d",status[i]);
            server->textg->playerstatus[order][i]->setText(s1);
        }
        for (;i<6;i++)
        server->textg->playerstatus[order][i]->clear();
        break;
    }
    case CardChange:
    {
     //QString s;
     set<int>::iterator i;
     int j;
     for (i=card.begin(),j=0; i!=card.end(); i++,j++)
     {
         QString s1 = cardlist.getQName(*i);
         server->textg->playercard[order][j]->setText(s1);
     }
     for (;j<12;j++)
     server->textg->playercard[order][j]->clear();
     break;
    }
    case EnergyChange:
    {
        QString s;
        //s.sprintf("玩家%d 获得了%d个宝石,%d个水晶",order,sendMessageBuffer[1],sendMessageBuffer[2]);
        //server->textg->textbrowser->append(s);
        s.sprintf("%d",energyGem);
        server->textg->playergem[order]->setText(s);
        s.sprintf("%d",energyCrystal);
        server->textg->playercrystal[order]->setText(s);
        break;
    }
    case AttackHappen:
    {
        QString s;
        s.sprintf("玩家%d->",order);
        QString s1;
        s1.sprintf("玩家%d:",sendMessageBuffer[1]);
        s = s + s1;
        s = s + cardlist.getQName(sendMessageBuffer[2]);
        server->textg->textbrowser->append(s);
        break;
    }
    case CureChange:
    {
        QString s;
        s.sprintf("%d",cureNumber);
        server->textg->playercure[order]->setText(s);
        break;
    }
    default:
     break;
    }

}
void Player::receive()
{
    //QTimer t;
    //t.start();
    while(1)
    {
        Sleep(100);
        QCoreApplication::processEvents();
        if (getmessage)
        break;
    }
    getmessage = false;
}
void Player::end()
{
    for (int i=0; i<20; i++)
    {
        receiveMessageBuffer[i] = -1;
    }
    getmessage = false;
    //receiveMessageBuffer[0] = TurnEnd;
    BroadCast();
}
//----------------------------------------------------------
/*
actionType Player::receive(int* a)
{
    return Attack;
}
*/
Player::Player(/*QObject *parent = 0,*/ Server* p,int order1,int teamnumber,int character)
    :/*QObject(parent),*/ server(p),order(order1),teamNumber(teamnumber),character(character)
{
    cardLimit = 6;
 //   cardNumber = card.size();
    cureLimit = 2;
    cureNumber = 0;
    energyGem = 0;
    energyCrystal = 0;
    stonelimit = 3;
    theShield = 0;
    statusnumber = 0;
    getmessage = false;
    for (int i=0; i<20; i++)
    {
        receiveMessageBuffer[i]=-1;
    }
    for (int i=0; i<20; i++)
    {
        sendMessageBuffer[i]=-1;
    }
 //------------测试-----------
    QString s;
    s.sprintf("%d",teamnumber);
    server->textg->playerteam[order]->setText(s);

 //--------------------------
}

void Player::start()
{
    //BroadCast(TurnBegin,order,order);
    sendMessageBuffer[0] = TurnBegin;
    //sendMessageBuffer[1] = order;
    BroadCast();
    //sendMessage();//回合开始

    try{

    handleStatus();
    beforeAction();
    //int receiveMessageBuffer[20];

    int i = 1;
    while(i--)
    {
        try{
        //sendMessage(Action);
        sendMessageBuffer[0] = ActionType;
        sendMessage();

        //actionType returnAcction = receive(receiveMessageBuffer);
        //测试----------------------------
        getmessage = false;
        //--------------------------------
        receive();
        int returnAcction = receiveMessageBuffer[0];
        if (returnAcction == Attack)
        {
            attackAction();
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

    //end();
    //receiveMessageBuffer[0] = TurnEnd;
    //BroadCast(TurnEnd,order,order);
    //BroadCast();
}
void Player::handleStatus()
{
    //Kind = 1;it's your turn;
    //int receiveMessageBuffer[20];

    //Check for weak,poison,and some specail card(封印束缚，勇者挑衅等)
    for(int j = statusnumber;j != 0;j --)
    {
        int cardname = cardlist.getName(status[j - 1]);
        switch(cardname)
        {
            case weak:
            {
                weakRespond(status[j - 1],j-1);
                break;
            }
            case poison:
            {
                poisonRespond(status[j - 1],j-1);
                break;
            }
            /*
            case FIVEBOUND:
            {
                break;
            }
            case PROVOKE:
            {
                break;
            }
            */
            default:
            break;
        }
    }
}
void Player::beforeAction()
{

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
void Player::activate()
{
    sendMessageBuffer[0] = Activated;
    //sendMessageBuffer[1] = order;
    BroadCast();
    return;
}
/*
void Player::end()
{
    //receiveMessageBuffer[0] = TurnEnd;
    BroadCast();
}
*/
void Player::foldCard(int* idOfCard,int amount,bool canBeSee)
{
    for (int i=0; i<amount; i++)
    {
        card.erase(*(idOfCard+i));
        server->gamePile->putIntoPile(*(idOfCard+i));
    }
    cardNumber = this->card.size();

    if (canBeSee)
    {
        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = -amount;
        BroadCast();

        sendMessageBuffer[0] = Show;
        sendMessageBuffer[1] = amount;
        for (int i=0; i<amount; i++)
        {
            sendMessageBuffer[i+2] = idOfCard[i];
        }
        BroadCast();//改变手牌数量并展示
    }
    else
    {
        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = -amount;
        BroadCast();//改变手牌数量
    }
}
void Player::getCard(int amount)
{
    for (int i=0; i<amount; i++)
    {
        int cardname = server->gamePile->getCard();
        card.insert(cardname);
        sendMessageBuffer[i+2] = cardname;
    }
    cardNumber = card.size();
    sendMessageBuffer[0] = GetCard;
    sendMessageBuffer[1] = amount;
    sendMessage();

    sendMessageBuffer[0] = CardChange;
    sendMessageBuffer[1] = amount;
    BroadCast();//改变手牌数量
}
void Player::addStatus (int cardUsed)
{
    status[statusnumber] = cardUsed;
    statusnumber ++;
    //------测试------------
    int j;
    for (int i=0,j=0; i<statusnumber; i++,j++)
    {
        QString s1 = cardlist.getQName(status[i]);
        //s.sprintf("%d",status[i]);
        server->textg->playerstatus[order][j]->setText(s1);
    }
    for (;j<6;j++)
    server->textg->playerstatus[order][j]->clear();
    //-------------------------------
}
bool Player::cureExist()
{
    if (cureNumber > 0)
        return true;
    else
        return false;
}
bool Player::shieldExist()
{
    if (theShield != 0)
        return true;
    else
        return false;
}
void Player::destroySheild()
{
    for (int i=0; i<statusnumber-1; i++)
    {
        if (status[i] == theShield)
        {
            destroyStatus(theShield,i);
            break;
        }
    }
    theShield = 0;
}
void Player::destroyStatus(int card, int order)
{
    server->gamePile->putIntoPile(card);

    for (int i = order; i < statusnumber-1;  i++)
    {
        status[i] = status[i+1];
    }

    status[statusnumber-1] = 0;
    statusnumber--;

    sendMessageBuffer[0] = StatusDecrease;
    sendMessageBuffer[1] = card;
    BroadCast();//消去虚弱或中毒或圣盾

    return;
}
void Player::weakRespond(int card,int order)
{
    destroyStatus(card,order);
    //sendMessage(WEAK);//Kind = 18;
    sendMessageBuffer[0] = WeakRespond;
    sendMessage();
    //actionType returnKind = receive(receiveMessageBuffer);
    //测试----------------------------
    getmessage = false;
    //--------------------------------
    receive();
    int returnKind = receiveMessageBuffer[0];
    if(returnKind == Accept)//ACCEPT
    {
        takeDamage(3);
    }
    if(returnKind == NoAccept)
    {
        throw(1);
        //跳至下一个玩家
    }
}
void Player::poisonRespond(int card,int order)
{
    destroyStatus(card,order);
    countDamage(1);
}

void Player::purchase()
{
    //测试函数----------------
    QString s;
    s.sprintf("玩家%d选择购买",order);
    //s = s + "选择购买";
    server->textg->textbrowser->append(s);
    //-----------------------
    (server->team[teamNumber])->getStone(Gem);
    (server->team[teamNumber])->getStone(Crystal);
    getCard(3);
    return;
}
void Player::refine()
{
    //测试函数----------------
    QString s;
    s.sprintf("玩家%d选择提炼",order);
    server->textg->textbrowser->append(s);
    //-----------------------
    int gem = receiveMessageBuffer[1];
    int crystal = receiveMessageBuffer[2];
    for (int i=0; i<gem; i++)
    {
        (server->team[teamNumber])->lossStone(Gem);
        energyGem ++;
    }

    for (int i=0; i<crystal; i++)
    {
        (server->team[teamNumber])->lossStone(Crystal);
        energyCrystal++;
    }

    sendMessageBuffer[0] = EnergyChange;
    sendMessageBuffer[1] = gem;
    sendMessageBuffer[2] = crystal;
    BroadCast();//改变人物能量数量
}
void Player::fusion()
{
    //测试函数----------------
    QString s;
    s.sprintf("玩家%d选择合成",order);
    server->textg->textbrowser->append(s);
    //-----------------------
    int gem = receiveMessageBuffer[1];
    int crystal = receiveMessageBuffer[2];

    for (int i=0; i<gem; i++)
    {
        (server->team[teamNumber])->lossStone(Gem);
    }

    for (int i=0; i<crystal; i++)
    {
        (server->team[teamNumber])->lossStone(Crystal);
    }
    (server->team[teamNumber])->getGrail();
    (server->team[!teamNumber])->lossMorale(1);

    getCard(3);
}
//-----------感觉没有用-----------------------
void Player::attackAction()
{
    //if (receiveMessageBuffer[1] == 0)
    //{
         normalAttack();
    //}
    //else
    //{
        //?????????
    //}
}
//-----------------------------------------------
void Player::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else
    {
        //?????????
    }
}
void Player::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    //assert(this->card.find(cardUsed)!=this->card.end());
    //if(!server->players[attackTarget].canBeAttacked()) throw ActionIllegal();

    foldCard(&cardUsed,1,false);
    //暗灭无法应战，需要吗？
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

    //BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    if(server->players[attackTarget]->beAttacked(cardUsed,order,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(2);
        }
    }
    //Is there any one have a skill after his attack missed?
}
void Player::normalMagic()
{

    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
   // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();
    int cardname = cardlist.getName(cardUsed);
    switch (cardname) {
    case weak:
    case poison:
    {
        card.erase(cardUsed);
        cardNumber = this->card.size();
        server->players[magicTarget]->addStatus(cardUsed);

        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = -1;
        BroadCast();

        sendMessageBuffer[0] = StatusIncrease;
        sendMessageBuffer[1] = cardUsed;
        sendMessageBuffer[2] = magicTarget;
        BroadCast();//添加状态

        break;
    }
    case shield:
    {
        card.erase(cardUsed);
        cardNumber = this->card.size();
        server->players[magicTarget]->theShield = cardname;
        server->players[magicTarget]->addStatus(cardUsed);

        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = -1;
        BroadCast();

        sendMessageBuffer[0] = StatusIncrease;
        sendMessageBuffer[1] = cardUsed;
        sendMessageBuffer[2] = magicTarget;
        BroadCast();//添加状态

        break;
    }
    case missile:
    {
        foldCard(&cardUsed);
        server->players[magicTarget]->beMagicMissileAttack(cardUsed,2);
        break;
    }
    default:
        break;
    }
    /*
    if(cardname == weak || cardname == poison)
    {
        card.erase(cardUsed);
        cardNumber = this->card.size();
        server->players[magicTarget]->addStatus(cardname);
        BroadCast();//添加状态
    }
    else if (cardname == shield)
    {
        card.erase(cardUsed);
        cardNumber = this->card.size();
        theShield = cardname;
        server->players[magicTarget]->addStatus(cardname);
        BroadCast();//添加状态
    }
    else if(cardname == missile)
    {
        server->players[magicTarget]->beMagicMissileAttack(cardUsed,2);
    }
    */
}
void Player::headOn(int chainLength)
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];

    foldCard(&cardUsed,1,false);
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
    if(server->players[attackTarget]->beAttacked(order,cardUsed,chainLength,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
          (server->team[teamNumber])->getStone(Crystal);
          server->players[attackTarget]->countDamage(2);
        }
    }
}
void Player::beMagicMissileAttack(int cardUsed, int damage)
{
    //sendMessage(youMagicMissile,this->order,card);
    sendMessageBuffer[0] = AttackRespond;
    sendMessageBuffer[1] = cardUsed;
    sendMessage();
    //int receiveMessageBuffer[20];
    //actionType reaction = receive(receiveMessageBuffer);
    //测试----------------------------
    getmessage = false;
    //--------------------------------
    receive();
    int reaction = receiveMessageBuffer[0];
    if(reaction == AcceptAttack)
    {
        if(this->shieldExist())
        {
            this->destroySheild();
            return;
        }

        else
        {
            countDamage(damage);
        }
    }
    if(reaction == HeadOn)
    {
        int target = receiveMessageBuffer[1];
        int cardUsed1 = receiveMessageBuffer[2];
        foldCard(&cardUsed1);
        server->players[target]->beMagicMissileAttack(cardUsed1,damage+1);
        return ;
    }
    if(reaction == Light)
    {
        int cardUsed = receiveMessageBuffer[1];
        foldCard(&cardUsed);
        //BroadCast();//改变手牌数量
        return;
    }
}

bool Player::beAttacked(int attacker,int cardUsed,int chainLength,bool canBeAccept)
{
    //int receiveMessageBuffer[20];
    //sendMessage(BeAttack,canBeAccept);
    sendMessageBuffer[0] = AttackRespond;
    sendMessageBuffer[1] = cardUsed;
    //sendMessageBuffer[2] = attacker;
    sendMessageBuffer[2] = canBeAccept;
    sendMessage();
    //actionType reaction = receive(receiveMessageBuffer);
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
        foldCard(&cardUsed);
        //BroadCast();//改变手牌数量，展示圣光
        return false;
    }
    }
}
void Player::countDamage(int damage)
{
    if(cureExist())
    {
        int realDamage = damage - useCure(damage);
        if (realDamage > 0)
        takeDamage(realDamage);
    }
    else
    {
        takeDamage(damage);
    }
}
int Player::useCure(int damage)
{
    //sendMessage(youUseCure,this->order,damage);
    sendMessageBuffer[0] = CureRespond;
    sendMessageBuffer[1] = damage;
    sendMessage();
    //int receiveMessageBuffer[20];
    //actionType returnAmount = receive(receiveMessageBuffer);
    //测试----------------------------
    getmessage = false;
    //--------------------------------
    receive();
    int returnAmount = receiveMessageBuffer[0];
    sendMessageBuffer[0] = CureChange;
    sendMessageBuffer[1] = returnAmount;
    BroadCast();//改变治疗数量

    return returnAmount;
}
void Player::takeDamage(int damage)
{
    //BroadCast(TakeDamage,order,order);
    //server->dealCards(order,damage);
    getCard(damage);

    if(cardNumber > cardLimit)
    {
        Discards(cardNumber-cardLimit);
    }
}
void Player::Discards(int amount)
{
    //BroadCast(Discard,order,order,amount);
    //try...
    //int receiveMessageBuffer[20];
                                     // receive(receiveMessageBuffer);
    //for(int i = 0; i<amount; i++)
    //{
        //if(card.find(receiveMessageBuffer[i])==card.end()) throw ActionIllegal;
    //}
    //catch...
    //----测试------------
    server->textg->textbrowser->append("请选择牌号");
    getmessage = false;
    //--------------------------------
    receive();
    foldCard(receiveMessageBuffer,amount,false);
    (server->team[teamNumber])->lossMorale(amount);
}
