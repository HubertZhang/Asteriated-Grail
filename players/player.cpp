#include "player.h"
#include "server.h"
#include"cardList.h"
#include <assert.h>
#include"cardpile.h"
#include"textgui.h"
#include<QString>
#ifdef Q_OS_WIN
#include<Windows.h>
#else
#include<unistd.h>
#define Sleep(a) usleep(1000*a)
#endif

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
            while (s[i] != ',' && s[i] != '\0')
            {
                k++;
                i++;
            }
            int ss = 0;
            for (int t=0; t<k; t++)
            {
                int sum=1;
                for (int m=0; m<k-t-1; m++)
                {
                    sum = sum *10;
                }
                ss = ss + int((s[i+t-k]-'0')*sum);
            }
            receiveMessageBuffer[j] = ss;
            j++;
        }
        if (s[i]=='\0')
        {
            break;
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
    vector<int> tempMessage;

    switch(sendMessageBuffer[0])
    {
    /*
    case TurnBegin:
    {
        QString s;
        s = "---------------------------------------";
        server->textg->textbrowser->append(s);
        s.sprintf("player%d's action:",order);
        server->textg->textbrowser->append(s);
        break;
    }
    */
    case BeforeAction://Kind 6
    {
        QString s;
        s = "特殊行动阶段,请选择:";
        server->textg->textbrowser->append(s);
        s = "0:什么也不做, 1:启动, 2:提炼(宝石,水晶),3:购买, 3:合成(宝石,水晶)";
        server->textg->textbrowser->append(s);

        tempMessage.push_back(6);
        break;
    }
    case ActionType://Kind 7
    {
        QString s;
        s = "行动阶段,请选择:";
        server->textg->textbrowser->append(s);
        s = "0:攻击(目标,卡牌), 1:魔法(技,目标,卡牌)";
        server->textg->textbrowser->append(s);

        tempMessage.push_back(7);
        break;
    }
    case GetCard://Kind 13
    {
        QString s;
        s.sprintf("玩家%d 获得%d张牌 ",order,sendMessageBuffer[1]);
        server->textg->textbrowser->append(s);

        tempMessage.push_back(13);
        tempMessage.push_back(sendMessageBuffer[1]);
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
            tempMessage.push_back(sendMessageBuffer[2+i]);
        }
        break;
    }
    case WeakRespond://Kind 4
    {
        server->textg->textbrowser->append("你被虚弱,请选择:");
        server->textg->textbrowser->append("0:跳过回合,1:摸三张牌");

        tempMessage.push_back(4);
        break;
    }
    case AttackRespond://Kind 11
    {
        QString s;
        s.sprintf("玩家%d 请选择:",order);
        server->textg->textbrowser->append(s);
        server->textg->textbrowser->append("0:承受攻击,1:应战(对象,卡牌),2:圣光");

        tempMessage.push_back(11);
        tempMessage.push_back(sendMessageBuffer[1]);
        tempMessage.push_back(sendMessageBuffer[2]);
        tempMessage.push_back(sendMessageBuffer[3]);
        break;
    }
    case CureRespond://Kind 12
    {
        server->textg->textbrowser->append("请选择使用治疗数:");

        tempMessage.push_back(12);
        tempMessage.push_back(sendMessageBuffer[1]);
        break;
    }
    case AskRespond://Kind 15
    {
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
            QString s;
            s.sprintf("你是否use %d技",sendMessageBuffer[i+2]);
            server->textg->textbrowser->append(s);
        }
        server->textg->textbrowser->append("选择技能号");

        tempMessage.push_back(15);
        tempMessage.push_back(sendMessageBuffer[1]);
        int i;
        for (i=0; i<sendMessageBuffer[1]; i++)
        {
           tempMessage.push_back(sendMessageBuffer[i+2]);
        }

        if (character == 22 && sendMessageBuffer[2] == 2)
        {
            tempMessage.push_back(sendMessageBuffer[i+2]);
        }
        break;
    }
    case SpecialAsk://Kind 18
    {
        tempMessage.push_back(18);
        tempMessage.push_back(sendMessageBuffer[1]);
        if (sendMessageBuffer[1] == 2)
        {
          server->textg->textbrowser->append("五系封印!");
          server->textg->textbrowser->append("0:跳过回合,1:摸三张牌");
        }
        else if (sendMessageBuffer[1] == 4)
        {
            server->textg->textbrowser->append("威力赐福");
            server->textg->textbrowser->append("0:不使用,1:使用");
        }
        else if (sendMessageBuffer[1] == 5)
        {
            server->textg->textbrowser->append("迅捷赐福");
            server->textg->textbrowser->append("0:不使用,1:使用");
        }
        break;
    }
    case FoldCard://Kind 17
    {
        QString s;
        s.sprintf("请选择弃牌牌号(%d)",sendMessageBuffer[1]);
        server->textg->textbrowser->append(s);

        tempMessage.push_back(17);
        tempMessage.push_back(sendMessageBuffer[1]);
        break;
    }
    case AdditionalAction://Kind 19
    {
        QString s;
        s = "附加行动阶段:";
        server->textg->textbrowser->append(s);
        if (sendMessageBuffer[1] == 0)
        {
            s = "0:攻击(目标,卡牌)";
            server->textg->textbrowser->append(s);
        }
        else
        {
            s =  "1:魔法(技,目标,卡牌)";
            server->textg->textbrowser->append(s);
        }

        tempMessage.push_back(19);
        tempMessage.push_back(sendMessageBuffer[1]);
        break;
    }
    case MissileRespond://Kind 20
    {
        QString s;
        s.sprintf("玩家%d 被魔弹攻击,请选择:",order);
        server->textg->textbrowser->append(s);
        server->textg->textbrowser->append("0:承受攻击,1:应战(卡牌),2:圣光");

        tempMessage.push_back(20);
        break;
    }
    case AskRespond1:
    {
        tempMessage.push_back(21);
        break;
    }
    case SpecialChange2:
    {
        tempMessage.push_back(22);
        for (int i=0; i<sendMessageBuffer[1]+1; i++)
        {
            tempMessage.push_back(sendMessageBuffer[i+1]);
        }
        break;
    }
    case FoldOneCard://Kind 14
    {
        QString s;
        s.sprintf("玩家%d 弃一张牌 ",order);
        server->textg->textbrowser->append(s);

        tempMessage.push_back(14);
        tempMessage.push_back(sendMessageBuffer[1]);
        tempMessage.push_back(sendMessageBuffer[2]);
        break;
    }
    default:
        break;
    }
    /*
    while(sendMessageBuffer[i]!=-1){
        tempMessage.push_back(sendMessageBuffer[i]);
        sendMessageBuffer[i] = -1;
        i++;
    }
    */
    server->networkServer.sendMessage(order,tempMessage);
}
void Player::BroadCast()
{
    vector<int> tempMessage;

    switch(sendMessageBuffer[0])
    {
    case TurnBegin://Kind 3
    {
     QString s;
     s = "----------------------------";
     server->textg->textbrowser->append(s);
     s.sprintf("玩家%d:",order);
     server->textg->textbrowser->append(s);

     tempMessage.push_back(3);
     tempMessage.push_back(order);
     break;
    }
    case Show://Kind 10
    {
        QString s;
        s.sprintf("玩家%d 使用卡牌: ",order);
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
            QString s1 = cardlist.getQName(sendMessageBuffer[i+2]);
            s = s + s1;
        }
        server->textg->textbrowser->append(s);

        //for (int i=0; i<sendMessageBuffer[1]; i++)
        //{
        //   emit fold(order,sendMessageBuffer[i+2]);
        //}

        tempMessage.push_back(10);
        tempMessage.push_back(order);
        tempMessage.push_back(0);
        tempMessage.push_back(sendMessageBuffer[1]);
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
            tempMessage.push_back(sendMessageBuffer[i+2]);
        }
        break;
    }
    case StatusIncrease://Kind 5
    {
        QString s;
        s.sprintf("玩家%d添加",order);
        s = s + cardlist.getQName(sendMessageBuffer[1]);
        server->textg->textbrowser->append(s);

        int j=0;
        for (; j<statusnumber; j++)
        {
            QString s1 = cardlist.getQName(status[j]);
            //s.sprintf("%d",status[i]);
            //server->textg->textbrowser->append("2222");
            server->textg->playerstatus[order][j]->setText(s1);
        }
        for (;j<6;j++)
        server->textg->playerstatus[order][j]->clear();

        tempMessage.push_back(5);
        tempMessage.push_back(order);
        tempMessage.push_back(sendMessageBuffer[1]);
        tempMessage.push_back(1);
        break;
    }
    case StatusDecrease://Kind 5
    {
        QString s;
        s.sprintf("玩家%d‘s",order,sendMessageBuffer[1]);
        s = s + cardlist.getQName(sendMessageBuffer[1]);
        s = s + "移除";
        server->textg->textbrowser->append(s);

        emit decreasestatus(order,sendMessageBuffer[1]);

        int i=0;
        for (i=0; i<statusnumber; i++)
        {
            QString s1 = cardlist.getQName(status[i]);
            //s.sprintf("%d",status[i]);
            server->textg->playerstatus[order][i]->setText(s1);
        }
        for (;i<6;i++)
        server->textg->playerstatus[order][i]->clear();

        tempMessage.push_back(5);
        tempMessage.push_back(order);
        tempMessage.push_back(sendMessageBuffer[1]);
        tempMessage.push_back(0);
        break;
    }
    case CardChange://Kind 9
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

     tempMessage.push_back(9);
     tempMessage.push_back(order);
     tempMessage.push_back(sendMessageBuffer[1]);
     for (int i=0; i<6; i++)
     {
         tempMessage.push_back(0);
     }
     break;
    }
    case EnergyChange://Kind 9
    {
        QString s;
        //s.sprintf("玩家%d 获得了%d个宝石,%d个水晶",order,sendMessageBuffer[1],sendMessageBuffer[2]);
        //server->textg->textbrowser->append(s);
        s.sprintf("%d",energyGem);
        server->textg->playergem[order]->setText(s);
        s.sprintf("%d",energyCrystal);
        server->textg->playercrystal[order]->setText(s);

        tempMessage.push_back(9);
        tempMessage.push_back(order);
        tempMessage.push_back(0);
        tempMessage.push_back(0);
        tempMessage.push_back(0);
        tempMessage.push_back(sendMessageBuffer[1]);
        tempMessage.push_back(sendMessageBuffer[2]);
        tempMessage.push_back(0);
        tempMessage.push_back(0);
        break;
    }
    case SpecialChange:
    {
        QString s;
        s.sprintf("玩家%d增加%d黄魂,%d蓝魂",order,sendMessageBuffer[1],sendMessageBuffer[2]);
        server->textg->textbrowser->append(s);

        tempMessage.push_back(9);
        tempMessage.push_back(order);
        for (int i=0; i<5; i++)
        {
            tempMessage.push_back(0);
        }
        tempMessage.push_back(sendMessageBuffer[1]);
        tempMessage.push_back(sendMessageBuffer[2]);
        break;
    }
    case AttackHappen://Kind 10
    {
        QString s;
        s.sprintf("玩家%d->",order);
        QString s1;
        s1.sprintf("玩家%d:",sendMessageBuffer[1]);
        s = s + s1;
        s = s + cardlist.getQName(sendMessageBuffer[2]);
        server->textg->textbrowser->append(s);

        //for (int i=0; i<sendMessageBuffer[1]; i++)
       // {
       //  emit fold(order,sendMessageBuffer[2]);
        //}

        tempMessage.push_back(10);
        tempMessage.push_back(order);
        tempMessage.push_back(1);
        tempMessage.push_back(1);
        tempMessage.push_back(sendMessageBuffer[1]);
        tempMessage.push_back(sendMessageBuffer[2]);

        break;
    }
    case DrawPicture://Kind 10
    {
        QString s;
        s.sprintf("玩家%d->",order);
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
          QString s1;
          s1.sprintf("玩家%d ",sendMessageBuffer[i+3]);
          s = s + s1;
        }
        for (int i=0; i<sendMessageBuffer[2];i++)
        {
        s = s + cardlist.getQName(sendMessageBuffer[i+3+sendMessageBuffer[1]]);
        }
        //server->textg->textbrowser->append(s);

       // for (int i=0; i<sendMessageBuffer[2]; i++)
       // {
       //   emit fold(order,sendMessageBuffer[i+3+sendMessageBuffer[1]]);
       // }

        tempMessage.push_back(10);
        tempMessage.push_back(order);
        tempMessage.push_back(sendMessageBuffer[1]);
        tempMessage.push_back(sendMessageBuffer[2]);
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
           tempMessage.push_back(sendMessageBuffer[i+3]);
        }
        for (int i=0; i<sendMessageBuffer[2];i++)
        {
           tempMessage.push_back(sendMessageBuffer[i+3+sendMessageBuffer[1]]);
        }
        break;
    }
    case CureChange://Kind 9
    {
        QString s;
        s.sprintf("%d",cureNumber);
        server->textg->playercure[order]->setText(s);

        tempMessage.push_back(9);
        tempMessage.push_back(order);
        for (int i=0; i<2; i++)
        {
            tempMessage.push_back(0);
        }
        tempMessage.push_back(sendMessageBuffer[1]);
        for (int i=0; i<4; i++)
        {
            tempMessage.push_back(0);
        }
        break;
    }
    case Activated://Kind 16
    {
        QString s;
        s.sprintf("玩家%d 启动: ",order);
        server->textg->textbrowser->append(s);

        tempMessage.push_back(16);
        tempMessage.push_back(order);
        tempMessage.push_back(sendMessageBuffer[1]);
        break;
    }
    case CardLimitChange://Kind 9
    {
        QString s;
        s.sprintf("玩家%d 手牌上限变为%d ",order,cardLimit);
        server->textg->textbrowser->append(s);

        tempMessage.push_back(9);
        tempMessage.push_back(order);
        tempMessage.push_back(0);
        tempMessage.push_back(sendMessageBuffer[1]);
        for (int i=0; i<5; i++)
        {
            tempMessage.push_back(0);
        }
        break;
    }
    default:
     break;
    }

    server->networkServer.sendMessage(-1,tempMessage);

    if (sendMessageBuffer[0] == Show)
    {
        //Sleep(1500);
        for (int i=0; i<sendMessageBuffer[1]; i++)
        {
           emit fold(order,sendMessageBuffer[i+2]);
        }
    }
    else if (sendMessageBuffer[0] == AttackHappen)
    {
        //Sleep(1500);
        emit fold(order,sendMessageBuffer[2]);
    }
    else if (sendMessageBuffer[0] == DrawPicture)
    {
        //Sleep(1500);
        for (int i=0; i<sendMessageBuffer[2]; i++)
        {
          emit fold(order,sendMessageBuffer[i+3+sendMessageBuffer[1]]);
        }
    }
}
void Player::receive()
{
    getmessage = false;
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
    //BroadCast();
}
//----------------------------------------------------------

Player::Player(/*QObject *parent = 0,*/ Server* p,int order1,int teamnumber,int character)
    :/*QObject(parent),*/ server(p),order(order1),teamNumber(teamnumber),character(character)
{
    cardLimit = 6;
 // cardNumber = card.size();
    cureLimit = 2;
    cureNumber = 0;
    energyGem = 0;
    energyCrystal = 0;
    stonelimit = 3;
    theShield = 0;
    statusnumber = 0;
    activation = 1;
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
    server->textg->character[order]->setText("");

 //--------------------------
}

void Player::start()
{
    sendMessageBuffer[0] = TurnBegin;
    BroadCast(); //回合开始

    try{
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
void Player::handleStatus()
{
    int w = -1;
    int f = -1;
    int o = 0;
    //Check for weak,poison,and some specail card(封印束缚，勇者挑衅等)
    for (int j = statusnumber; j != 0; j--)
    {
       int cardname = cardlist.getName(status[j-1]);
       switch(cardname)
       {
           case weak:
           {
               w = status[j-1];
               o = j-1;
               break;
           }
           case poison:
           {
               poisonRespond(status[j - 1],j-1);
               break;
           }

           case -1://FIVEBOUND:
           {
               f = 150;
                break;
           }
       }
     }

    if (f == 150)
       {

           // server->gamePile->putIntoPile(card);
           sendMessageBuffer[0] = SpecialAsk;
           sendMessageBuffer[1] = 2;
           sendMessage();
           system("pause");
           receive();
           int returnKind = receiveMessageBuffer[0];
           if(returnKind == Accept)//ACCEPT
           {
               emit beweak(order,Accept,w);
           }
           else if(returnKind == NoAccept)
           {
               emit beweak(order,NoAccept,w);
               throw(1);
              //跳至下一个玩家
           }
       }
    else if (w != -1)
       {
           weakRespond(w,o);
       }

            /*
            case PROVOKE:
            {
                break;
            }
            */
}
void Player::beforeAction()
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
    sendMessageBuffer[1] = order;
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
        card.erase(idOfCard[i]);
        server->gamePile->putIntoPile(idOfCard[i]);
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

    sendMessageBuffer[0] = StatusIncrease;
    sendMessageBuffer[1] = cardUsed;
    BroadCast();
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
    for (int i=0; i<statusnumber; i++)
    {
        if (status[i] == theShield)
        {
            destroyStatus(theShield,i);
            server->gamePile->putIntoPile(theShield);
            break;
        }
    }
    theShield = 0;
}
void Player::destroyStatus(int card, int order)
{

    for (int i = order; i < statusnumber-1;  i++)
    {
        status[i] = status[i+1];
    }

    status[statusnumber-1] = -1;
    statusnumber--;

    sendMessageBuffer[0] = StatusDecrease;
    sendMessageBuffer[1] = card;
    BroadCast();//消去虚弱或中毒或圣盾

    return;
}
void Player::destroyStatus(int cardname)
{
    for (int i=0; i<statusnumber; i++)
    {
        if (status[i] == cardname)
        {
            destroyStatus(cardname,i);
            break;
        }
    }
}
void Player::weakRespond(int card,int order)
{
    destroyStatus(card,order);
    server->gamePile->putIntoPile(card);
    sendMessageBuffer[0] = WeakRespond;
    sendMessage();
    receive();
    int returnKind = receiveMessageBuffer[0];
    if(returnKind == Accept)//ACCEPT
    {
        takeDamage(3,2);
    }
    if(returnKind == NoAccept)
    {
        throw(1);
        //跳至下一个玩家
    }
}
void Player::poisonRespond(int card,int theorder)
{
    destroyStatus(card,theorder);
    server->gamePile->putIntoPile(card);
    emit bepoison(card);
    countDamage(1,Magic);
}
void Player::increaseCure(int amount,bool limit)
{
    if (limit)
    {
        if (cureLimit < cureNumber + amount)
        {
            if (cureLimit > cureNumber)
            {
              sendMessageBuffer[0] = CureChange;
              sendMessageBuffer[1] = cureLimit-cureNumber;
              BroadCast();//改变治疗数量
              cureNumber = cureLimit;
            }
        }
        else
        {
            cureNumber = cureNumber + amount;
            sendMessageBuffer[0] = CureChange;
            sendMessageBuffer[1] = amount;
            BroadCast();//改变治疗数量
        }
    }
    else
    {
        cureNumber = cureNumber + amount;
        sendMessageBuffer[0] = CureChange;
        sendMessageBuffer[1] = amount;
        BroadCast();//改变治疗数量
    }
}
void Player::decreaseCure(int amount)
{
    cureNumber = cureNumber - amount;
    sendMessageBuffer[0] = CureChange;
    sendMessageBuffer[1] = -amount;
    BroadCast();//改变治疗数量
}
void Player::changeCardLimit1(int amount)
{
    sendMessageBuffer[0] = CardLimitChange;
    sendMessageBuffer[1] = amount - cardLimit;
    BroadCast();
    cardLimit = amount;

    emit changecardlimit(order);
    if (cardLimit < cardNumber)
    {
         Discards(cardNumber-cardLimit,2);
    }
}
void Player::changeCardLimit2(int amount)
{
    cardLimit = cardLimit + amount;
    sendMessageBuffer[0] = CardLimitChange;
    sendMessageBuffer[1] = amount;
    BroadCast();

    if (cardLimit < cardNumber)
    {
         Discards(cardNumber-cardLimit,2);
    }
}
void Player::useEnergy(int number,bool gem)
{
    if (gem == true)
    {
        energyGem = energyGem - number;
        sendMessageBuffer[0] = EnergyChange;
        sendMessageBuffer[1] = -number;
        sendMessageBuffer[2] = 0;
        BroadCast();//改变人物能量数量
    }
    else
    {
        if (energyCrystal >= number)
        {
            energyCrystal = energyCrystal - number;
            sendMessageBuffer[0] = EnergyChange;
            sendMessageBuffer[1] = 0;
            sendMessageBuffer[2] = -number;
            BroadCast();//改变人物能量数量
        }
        else
        {
            int t = energyCrystal;
            energyCrystal = 0;
            energyGem = t - number + energyGem;
            sendMessageBuffer[0] = EnergyChange;
            sendMessageBuffer[1] = t - number;
            sendMessageBuffer[2] = -t;
            BroadCast();//改变人物能量数量
        }
    }
}

void Player::purchase()
{
    //测试函数----------------
    QString s;
    s.sprintf("玩家%d选择购买",order);
    //s = s + "选择购买";
    server->textg->textbrowser->append(s);
    //-----------------------
    int gem = receiveMessageBuffer[1];
    int crystal = receiveMessageBuffer[2];
    for (int i=0; i<gem; i++)
    {
         (server->team[teamNumber])->getStone(Gem);
    }

    for (int i=0; i<crystal; i++)
    {
        (server->team[teamNumber])->getStone(Crystal);
    }

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

bool Player::canActivate()
{
    return (!((cardLimit - cardNumber) < 3 && activation == 1
            &&(energyCrystal+energyGem==stonelimit || server->team[teamNumber]->stone==0)));
}
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
    int damage = 2;
    foldCard(&cardUsed);
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

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
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
void Player::normalMagic()
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    int cardname = cardlist.getName(cardUsed);
    switch (cardname) {
    case weak:
    case poison:
    {
        card.erase(cardUsed);
        cardNumber = this->card.size();

        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = -1;
        BroadCast();

        sendMessageBuffer[0] = AttackHappen;
        sendMessageBuffer[1] = magicTarget;
        sendMessageBuffer[2] = cardUsed;
       // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
        BroadCast();

        server->players[magicTarget]->addStatus(cardUsed);
        break;
    }
    case shield:
    {
        card.erase(cardUsed);
        cardNumber = this->card.size();
        server->players[magicTarget]->theShield = cardUsed;

        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = -1;
        BroadCast();

        sendMessageBuffer[0] = AttackHappen;
        sendMessageBuffer[1] = magicTarget;
        sendMessageBuffer[2] = cardUsed;
       // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
        BroadCast();

        server->players[magicTarget]->addStatus(cardUsed);
        break;
    }
    case missile:
    {
        foldCard(&cardUsed);
        int target;

        for (int i=1; i<6; i++)
        {
            if (server->players[(order+i)%6]->teamNumber!= teamNumber)
            {
                target = (order+i)%6;
                break;
            }
        }

        sendMessageBuffer[0] = AttackHappen;
        sendMessageBuffer[1] = target;
        sendMessageBuffer[2] = cardUsed;
        // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
         BroadCast();

        bool missileCycle[6];
        for (int i=0; i<6; i++)
        {
            missileCycle[i] = false;
        }
        missileCycle[order] = true;

        server->players[target]->beMagicMissileAttack(2,missileCycle);
        break;
    }
    default:
        break;
    }
    emit finishaction(order, Magic);
}
void Player::headOn(int chainLength)
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
void Player::beMagicMissileAttack(int damage, bool *missileCycle)
{
    if ((damage -1)%5 == 0)
    {
        for(int i=0; i<6; i++)
        {
            missileCycle[i] = false;
        }
    }
    //sendMessage(youMagicMissile,this->order,card);
    sendMessageBuffer[0] = MissileRespond;
    sendMessage();

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
            emit bemissile(damage);
            countDamage(damage,Magic);
        }
    }
    if(reaction == HeadOn)
    {

        int cardUsed = receiveMessageBuffer[1];
        foldCard(&cardUsed);

        int target;
        for (int i=1; i<6; i++)
        {
            if (server->players[(order+i)%6]->teamNumber!= teamNumber &&
                    missileCycle[(order+i)%6] == false)
            {
                target = (order+i)%6;
                missileCycle[order] = true;
                break;
            }
        }

        sendMessageBuffer[0] = AttackHappen;
        sendMessageBuffer[1] = target;
        sendMessageBuffer[2] = cardUsed;
       // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
        BroadCast();

        server->players[target]->beMagicMissileAttack(damage+1,missileCycle);
        return ;
    }
    if(reaction == Light)
    {
        int cardUsed = receiveMessageBuffer[1];
        foldCard(&cardUsed,1,true);
        //BroadCast();//改变手牌数量
        return;
    }
}

bool Player::beAttacked(int attacker,int cardUsed,int chainLength,bool canBeAccept)
{
    sendMessageBuffer[0] = AttackRespond;
    sendMessageBuffer[1] = cardUsed;
    sendMessageBuffer[3] = attacker;
    sendMessageBuffer[2] = canBeAccept;
    sendMessage();

    receive();
    int reaction =receiveMessageBuffer[0];
    switch(reaction)
    {
    case AcceptAttack:
    {
        return true;
    }
    case HeadOn:
    {
        this->headOn(chainLength+1);
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
    return true;
}
void Player::countDamage(int damage,int kind)
{
    if(cureExist())
    {
        int realDamage = damage - useCure(damage);
        if (realDamage > 0)
        actualDamage(realDamage,kind);
    }
    else
    {
        actualDamage(damage,kind);
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
    decreaseCure(returnAmount);

    return returnAmount;
}
void Player::actualDamage(int damage,int kind)
{
    if (damage > 0)
    {
        emit bedamage2(order,damage,kind);
        if (damage > 0)
        {
        emit bedamage1(order,damage,kind);
        if (damage > 0)
        {
            takeDamage(damage,kind);
        }
        }
    }
}
void Player::takeDamage(int damage,int kind)
{
    if (damage > 0)
    {
      getCard(damage);

      if(cardNumber > cardLimit)
      {
          Discards(cardNumber-cardLimit,kind);
      }
    }
}
void Player::Discards(int amount,int kind)
{
    sendMessageBuffer[0] = FoldCard;
    sendMessageBuffer[1] = amount;
    sendMessage();

    receive();

    foldCard(receiveMessageBuffer,amount);

    emit moraleloss3(teamNumber, amount, kind);
   (server->team[teamNumber])->lossMorale(amount);
}
