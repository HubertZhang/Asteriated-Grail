#include "player.h"
#include "server.h"
#include"cardList.h"
#include <assert.h>
#include"cardpile.h"

//extern CardList cardlist;
CardList cardlist;

void Player::BroadCast()
{

}

void Player::sendMessage()
{

}

actionType Player::receive(int* a)
{
      return Attack;
}


Player::Player(Server* p,int order,int teamnumber,int character)
    :server(p),order(order),teamNumber(teamnumber),character(character)
{
    cardLimit = 6;
    cardNumber = card.size();
    cureLimit = 2;
    cureNumber = 0;
    energyGem = 0;
    energyCrystal = 0;
    stonelimit = 3;
    theShield = 0;
    statusnumber = 0;
    for (int i=0; i<20; i++)
    {
        receiveMessageBuffer[i]=-1;
    }
    for (int i=0; i<20; i++)
    {
        sendMessageBuffer[i]=-1;
    }
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

        actionType returnAcction = receive(receiveMessageBuffer);
        if (returnAcction == Attack)
        {
            attack();
        }
        else if (returnAcction == Magic)
        {
            magic();
        }

        }
        catch(ActionIllegal)
        {
            i++;//Do the While Loop again
        }
    }
    }
    catch(...)
    {

    }

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
                weakRespond(cardname,j-1);
                break;
            }
            case poison:
            {
                poisonRespond(cardname,j-1);
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
    actionType returnAcction = receive(receiveMessageBuffer);
    switch (returnAcction)
    {
    case (Activate):
    {
        activate();
        break;
    }
    case (Purchase):
    {
        this->purchase();
        break;
    }
    case (Fusion):
    {
        this->fusion();
        break;
    }
    case (Refine):
    {
        this->refine();
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

void Player::foldCard(int* idOfCard,int amount,bool canBeSee)
{
    for (int i=0; i<amount; i++)
    {
        this->card.erase(*(idOfCard+i));
        server->gamePile->putIntoPile(*(idOfCard+i));
    }
    cardNumber = this->card.size();

    if (canBeSee)
    {
        sendMessageBuffer[0] = CardChange;
        sendMessageBuffer[1] = -amount;
        BroadCast();

        sendMessageBuffer[0] = Show;
        for (int i=0; i<amount; i++)
        {
            sendMessageBuffer[i+1] = idOfCard[i];
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
        sendMessageBuffer[i+1] = cardname;
    }
    cardNumber = card.size();
    sendMessageBuffer[0] = GetCard;
    sendMessage();

    sendMessageBuffer[0] = CardChange;
    sendMessageBuffer[1] = amount;
    BroadCast();//改变手牌数量
}
void Player::addStatus (int cardUsed)
{
    status[statusnumber] = cardUsed;
    statusnumber ++;
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
    actionType returnKind = receive(receiveMessageBuffer);
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
    (server->team[teamNumber]).getStone(Gem);
    (server->team[teamNumber]).getStone(Crystal);
    getCard(3);
    return;
}
void Player::refine()
{
    int gem = receiveMessageBuffer[0];
    int crystal = receiveMessageBuffer[1];
    for (int i=0; i<gem; i++)
    {
        (server->team[teamNumber]).lossStone(Gem);
        energyGem ++;
    }

    for (int i=0; i<crystal; i++)
    {
        (server->team[teamNumber]).lossStone(Crystal);
        energyCrystal++;
    }

    sendMessageBuffer[0] = EnergyChange;
    sendMessageBuffer[1] = gem;
    sendMessageBuffer[2] = crystal;
    BroadCast();//改变人物能量数量
}
void Player::fusion()
{
    int gem = receiveMessageBuffer[0];
    int crystal = receiveMessageBuffer[1];

    for (int i=0; i<gem; i++)
    {
        (server->team[teamNumber]).lossStone(Gem);
    }

    for (int i=0; i<crystal; i++)
    {
        (server->team[teamNumber]).lossStone(Crystal);
    }
    (server->team[teamNumber]).getGrail();
    (server->team[!teamNumber]).lossMorale(1);

    getCard(3);
}

void Player::attack()
{
    if (receiveMessageBuffer[0] == 0)
    {
         normalAttack();
    }
    else
    {
        //?????????
    }
}
void Player::magic()
{
    if (receiveMessageBuffer[0] == 0)
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

    if(server->players[attackTarget]->beAttacked(cardUsed,order,1,canBeAccept))
    {
        (server->team[teamNumber]).getStone(Gem);
        server->players[attackTarget]->countDamage(2);
    }
    //Is there any one have a skill after his attack missed?
}
void Player::normalMagic()
{

    int magicTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];

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
        server->players[magicTarget]->addStatus(cardname);

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
        theShield = cardname;
        server->players[magicTarget]->addStatus(cardname);

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
    int attackTarget = receiveMessageBuffer[0];
    int cardUsed = receiveMessageBuffer[1];

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
    if(server->players[attackTarget]->beAttacked(order,cardUsed,chainLength,canBeAccept))
    {
        (server->team[teamNumber]).getStone(Crystal);
        server->players[attackTarget]->countDamage(2);
    }
}
void Player::beMagicMissileAttack(int cardUsed, int damage)
{
    foldCard(&cardUsed);
    //sendMessage(youMagicMissile,this->order,card);
    sendMessageBuffer[0] = AttackRespond;
    sendMessageBuffer[1] = cardUsed;
    sendMessage();
    //int receiveMessageBuffer[20];
    actionType reaction = receive(receiveMessageBuffer);

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
        int target = receiveMessageBuffer[0];
        int cardUsed1 = receiveMessageBuffer[1];
        server->players[target]->beMagicMissileAttack(cardUsed1,damage+1);
        return ;
    }
    if(reaction == Light)
    {
        int cardUsed = receiveMessageBuffer[0];
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
    sendMessage();
    actionType reaction = receive(receiveMessageBuffer);
    switch(reaction) //returnKind: accept,noAccept,offset
    {
    case AcceptAttack:
    {
        if(this->shieldExist())//If there is a shield...
        {
            this->destroySheild();
            return false;
        }
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
        int cardUsed = receiveMessageBuffer[0];
        foldCard(&cardUsed);
        BroadCast();//改变手牌数量，展示圣光
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
    actionType returnAmount = receive(receiveMessageBuffer);

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
    receive(receiveMessageBuffer);
    //for(int i = 0; i<amount; i++)
    //{
        //if(card.find(receiveMessageBuffer[i])==card.end()) throw ActionIllegal;
    //}
    //catch...
    foldCard(receiveMessageBuffer,amount,false);
    (server->team[teamNumber]).lossMorale(amount);
}
