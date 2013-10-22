#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
}
Player::~Player()
{

}
void Player::activate()
{
    return;
}
void Player::attack(int attackTarget, int card)
{
    putIntoDiscardPile(card);
    bool findTheCard = false;
    for(int i = 0;i < p[nowCharacter].cardNumber;i++)
    {
        if(card[i] == card)
        {
            findTheCard = true;
        }
        if(findTheCard)
        {
            p[nowCharacter].card[i] = p[nowCharacter].card[i+1];
        }
    }
    p[nowCharacter].cardNumber --;
    if(p[attackTarget].beAttacked(card,1))
    {
        if(p[attackTarget].cureNumber != 0)
        {
            int cureAmount = p[attackTarget].useCure(2);
            int realDamage = 2 - cureAmount;
            p[attackTarget].bearDamage(realDamage);
        }
        else
        {
            p[attackTarget].bearDamage(2);
        }
    }
}
bool Player::beAttacked(int card,int KindOfAttack)
{
    sendMessage(youAreAttacked,this->order,card);
    if(returnKind == noAccept) //returnKind: accept,noAccept,offset
    {
        if(KindOfAttack == 1)
        {
            if(team[(this->teamNumber + 1)%2].stone != 5)
            {
                team[(this->teamNumber + 1)%2].gem ++;
                team[(this->teamNumber + 1)%2].stone ++;
            }
        }
        else
        {
            if(team[(this->teamNumber + 1)%2].stone != 5)
            {
                team[(this->teamNumber + 1)%2].cystal ++;
                team[(this->teamNumber + 1)%2].stone ++;
            }
        }
        return true;
    }
    if(returnKind == accept)
    {
        this->acceptAttack(attackTarget,card);
        return false;
    }
    if(returnKind == offset)
    {
        return false;
    }
}
int Player::useCure(int damage)
{
    sendMessage(youUseCure,this->order,damage);
    int cureAmount = returnAmount;
    return cureAmount;
}
void Player::bearDamage(int damage)
{
    int damageCard[15];
    for(int i = 0;i < damage;i++)
    {
        damageCard[i] = getCard();
    }
    damageCard[damage] = -1;
    for(int i = 0;i < damage;i++)
    {
        sendMessage(youDamaged,damageCard[15]);
    }
    for(int i = 0;damageCard[i] != -1;i++)
    {
        this->card[this->cardNumber] = damageCard[i];
        this->cardNumber ++;
    }
    if(this->cardNumber > this->cardLimit)
    {
        this->discardCard(this->cardNumber - this->cardLimit);
    }
}
void Player::discardCard(int amount)
{
    sendMessage(youDiscardCard,this->order,amount);
    int findCardAmount = 0;
    for(int i = 0;i < this->cardNumber;i++)
    {
        for(int j = 0;j < amount;j++)
        {
            if(card[i] == returnCard[j])
            {
                findCardAmount ++;
            }
        }
        this->card[i] = this->card[i + findCardAmount];
        this->cardNumber = this->cardLimit;
        team[this->teamNumber].morale -= amount;
    }
}
void Player::acceptAttack(int attackTarget, int card)
{
    putIntoDiscardPile(card);
    bool findTheCard = false;
    for(int i = 0;i < p[nowCharacter].cardNumber;i++)
    {
        if(card[i] == card)
        {
            findTheCard = true;
        }
        if(findTheCard)
        {
            p[nowCharacter].card[i] = p[nowCharacter].card[i+1];
        }
    }
    p[nowCharacter].cardNumber --;
    if(p[attackTarget].beAttacked(card,0))
    {
        if(p[attackTarget].cureNumber != 0)
        {
            int cureAmount = p[attackTarget].useCure(2);
            int realDamage = 2 - cureAmount;
            p[attackTarget].bearDamage(realDamage);
        }
        else
        {
            p[attackTarget].bearDamage(2);
        }
    }
}
void Player::magic(int magicTarget,int card)
{
    putIntoDiscardPile(card);
    bool findTheCard = false;
    for(int i = 0;i < p[nowCharacter].cardNumber;i++)
    {
        if(card[i] == card)
        {
            findTheCard = true;
        }
        if(findTheCard)
        {
            p[nowCharacter].card[i] = p[nowCharacter].card[i+1];
        }
    }
    p[nowCharacter].cardNumber --;
    if(getKind(card) == weak || getKind(card) == poison)
    {
        p[magicTarget].status[statusnmber] == getKind(card);
        p[magicTarget].statusnumber ++;
    }
    if(getKind(card) == magicmissile)
    {
        p[magicTarget].beMagicMissileAttack(card,2);
    }
}
void Player::beMagicMissileAttack(int card, int damage)
{
    sendMessage(youMagicMissile,this->order,card);
    if(returnKind == noAccept)
    {
        if(this->cureNumber != 0)
        {
            int cureAmount = p[attackTarget].useCure(damage);
            int realDamage = damage - cureAmount;
            p[attackTarget].bearDamage(realDamage);
        }
        else
        {
            p[attackTarget].bearDamage(damage);
        }
    }
    if(returnKind == accept)
    {
        p[attackTarget].beMagicMissileAttack(card,damage + 1);
    }
    if(returnKind == offset)
    {
        return;
    }
}
