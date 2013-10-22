#include "player.h"
#include "server.h"
#include <assert.h>
Player::Player()
{
    cardLimit = 6;
    cardNumber = card.size();
    cureLimit = 2;
    cureNumber = 0;
    energyGem = 0;
    energyCrystal = 0;
}

void Player::start()
{
    activate();
    int messageBuffer[20];
    int i = 1;
    while(i--)
    {
        try{
        actionType returnAcction = receive(messageBuffer);
        switch (returnAcction)
        {
        case (Attack):
        {
            int target = messageBuffer[0];
            int cardUsed = messageBuffer[1];
            this->attack(target,cardUsed,1);
            break;
        }
        case (Magic):
        {
    //        int target = messageBuffer[0];
    //        int cardUsed = messageBuffer[1];
    //        this->magic(target,cardUsed);
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
        }
        case (Refine):
        {
            int gem = messageBuffer[0];
            int crystal = messageBuffer[1];
            this->refine(gem, crystal);
            break;
        }
        }
        }
        catch(ActionIllegal)
        {
            i++;//Do the While Loop again
        }
    }
    BroadCast(TurnEnd,order,order);
}

void Player::activate()
{
    BroadCast(TurnBegin,order,order);
    return;
}
void Player::attack(int attackTarget, int cardUsed,int chainLength)
{
    assert(this->card.find(cardUsed)!=this->card.end());
    if(!server->players[attackTarget].canBeAttacked()) throw ActionIllegal();
    this->card.erase(cardUsed);
    cardNumber = this->card.size();
    foldCard(cardUsed);
    BroadCast(AttackHappen,order,attackTarget,cardUsed);

    if(server->players[attackTarget].beAttacked(cardUsed,chainLength))
    {
        if(server->players[attackTarget].cureNumber != 0)
        {
            int cureAmount = server->players[attackTarget].useCure(2);
            int realDamage = 2 - cureAmount;
            server->players[attackTarget].takeDamage(realDamage);
        }
        else
        {
            server->players[attackTarget].takeDamage(2);
        }
    }
    //Is there any one have a skill after his attack missed?
}

void Player::foldCard(int idOfCard)
{
    server->fold(idOfCard);
}

void Player::getCard(int idOfCard)
{
    card.insert(idOfCard);
    cardNumber = card.size();
}

bool Player::beAttacked(int cardUsed,int chainLength)
{
    int messageBuffer[20];
    reactionType reaction;// = receive(messageBuffer);
    switch(reaction) //returnKind: accept,noAccept,offset
    {
    case AcceptAttack:
    {
        //if(status[])//If there is a shield...
        if(chainLength == 1)
        {
            (server->team[!teamNumber]).getStone(Gem);
        }
        else
        {
            (server->team[!teamNumber]).getStone(Crystal);
        }
        return true;
    }
    case HeadOn:
    {
        int attackTarget = messageBuffer[0];
        int cardUsed1 = messageBuffer[1];
        //if(cardList[cardUsed1].type()!=cardList[cardUsed].type()) throw
        this->attack(attackTarget,cardUsed1,chainLength+1);//there should also be a catch!
        return false;
    }
//    case Light:
//    {
//        return false;
//    }
    }
}

//int Player::useCure(int damage)
//{
//    sendMessage(youUseCure,this->order,damage);
//    int cureAmount = returnAmount;
//    return cureAmount;
//}

void Player::takeDamage(int damage)
{
    //int damageCard[15];
    BroadCast(TakeDamage,order,order);
    server->dealCards(order,damage);

    if(cardNumber > cardLimit)
    {
        Discards(cardNumber-cardLimit);
    }
}
void Player::Discards(int amount)
{
    BroadCast(Discard,order,order,amount);
    //try...
    int messageBuffer[20];
    receive(messageBuffer);
    for(int i = 0; i<amount; i++)
    {
        //if(card.find(messageBuffer[i])==card.end()) throw ActionIllegal;
    }
    //catch...

    for(int i = 0; i<amount; i++)
    {
        card.erase(messageBuffer[i]);
    }
    cardNumber = card.size();
    thisTeam->lossMorale(amount);
}

//void Player::acceptAttack(int attackTarget, int card)
//{
//    foldCard(card);
//    bool findTheCard = false;
//    for(int i = 0;i < p[nowCharacter].cardNumber;i++)
//    {
//        if(card[i] == card)
//        {
//            findTheCard = true;
//        }
//        if(findTheCard)
//        {
//            p[nowCharacter].card[i] = p[nowCharacter].card[i+1];
//        }
//    }
//    p[nowCharacter].cardNumber --;
//    if(p[attackTarget].beAttacked(card,0))
//    {
//        if(p[attackTarget].cureNumber != 0)
//        {
//            int cureAmount = p[attackTarget].useCure(2);
//            int realDamage = 2 - cureAmount;
//            p[attackTarget].bearDamage(realDamage);
//        }
//        else
//        {
//            p[attackTarget].bearDamage(2);
//        }
//    }
//}

//void Player::magic(int magicTarget,int card)
//{
//    foldCard(card);
//    bool findTheCard = false;
//    for(int i = 0;i < p[nowCharacter].cardNumber;i++)
//    {
//        if(card[i] == card)
//        {
//            findTheCard = true;
//        }
//        if(findTheCard)
//        {
//            p[nowCharacter].card[i] = p[nowCharacter].card[i+1];
//        }
//    }
//    p[nowCharacter].cardNumber --;
//    if(getKind(card) == weak || getKind(card) == poison)
//    {
//        p[magicTarget].status[statusnmber] == getKind(card);
//        p[magicTarget].statusnumber ++;
//    }
//    if(getKind(card) == magicmissile)
//    {
//        p[magicTarget].beMagicMissileAttack(card,2);
//    }
//}
//void Player::beMagicMissileAttack(int card, int damage)
//{
//    sendMessage(youMagicMissile,this->order,card);
//    if(returnKind == noAccept)
//    {
//        if(this->cureNumber != 0)
//        {
//            int cureAmount = p[attackTarget].useCure(damage);
//            int realDamage = damage - cureAmount;
//            p[attackTarget].bearDamage(realDamage);
//        }
//        else
//        {
//            p[attackTarget].bearDamage(damage);
//        }
//    }
//    if(returnKind == accept)
//    {
//        p[attackTarget].beMagicMissileAttack(card,damage + 1);
//    }
//    if(returnKind == offset)
//    {
//        return;
//    }
//}
