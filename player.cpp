#include "player.h"
#include "cardpile.h"
#include "cardList.h"
//这一个cpp里面有大量的"."要被替换为"->"
//→ →我不是故意的
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
    for(int i = 0;i < this->cardNumber;i++)
    {
        if(card[i] == card)
        {
            findTheCard = true;
        }
        if(findTheCard)
        {
            this->card[i] = this->card[i+1];
        }
    }
    this->cardNumber --;
    if(p[attackTarget].beAttacked(card,1,this->order))
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
bool Player::beAttacked(int card,int KindOfAttack,int attacker)
{
    sendMessage(ATTACKED,this->order,attacker,card);//Kind = 3,you are attacked.this->order is the parameter
                                                    //to decide the people to receive this information,but this
                                                    //information will not be send out.
    if(returnKind == NOACCEPT) //returnKind: NOACCEPT,ACCEPT,OFFSET.
                               //NOACCEPT means he hit "取消"
                               //but the attack can be offseted by sheild.But not light.
    {
        if(this->sheildExist())
        {
            this->destroySheild();
            return false;
        }
        if(KindOfAttack == 1)
        {
            if(team[(this->teamNumber + 1)%2].stone != 5)
            {
                team[(this->teamNumber + 1)%2].gem ++;
                team[(this->teamNumber + 1)%2].stone ++;
            }
        }
        if(team[(this->teamNumber + 1)%2].stone != 5)
        {
            team[(this->teamNumber + 1)%2].cystal ++;
            team[(this->teamNumber + 1)%2].stone ++;
        }
        return true;
    }
    if(returnKind == ACCEPT)
    {
        this->acceptAttack(attackTarget,card);
        return false;
    }
    if(returnKind == OFFSET)
    {
        return false;
    }
}
int Player::useCure(int damage)
{
    sendMessage(ASKFORCURE,this->order,damage);
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
    sendMessage(EXTRACTCARD,damageCard[15]);//Kind = 11,EXTRACTCARD
    for(int i = 0;i < damage;i++)
    {
        this->card[this->cardNumber] = damageCard[i];
        this->cardNumber ++;
    }
    if(this->cardNumber > this->cardLimit)
    {
        this->discardCard(this->cardNumber - this->cardLimit);
    }
}
void Player::discardCard(int amount)//→ →我不是故意把弃牌算法写的这么低端的。。
                                    //→ →准确的说我就是故意的以后会用set之类的换掉吧
                                    //那就先这样吧Orz...原来的算法写错了QAQ
{
    sendMessage(DISCARDCARD,this->order,amount);//Kind = 13
    int temCardArray[10];
    int temNum = 0;
    for(int i = 0;i < this->cardNumber;i++)
    {
        for(int j = 0;j < amount;j++)
        {
            if(this->card[i] == returnCard[j])
            {
               break;
               if(j == amount - 1)
               {
                   temCardArray[temNum] = this->card[i];
                   temNum ++;
               }
            }
        }
    }
    for(int i = 0;i < this->cardLimit;i++)
    {
        this->card[i] = temCardArray[i];
    }
    this->cardNumber = this->cardLimit;
    team[this->teamNumber].morale -= amount;
    return;
}
void Player::acceptAttack(int attackTarget, int card)
{
    putIntoDiscardPile(card);
    bool findTheCard = false;
    for(int i = 0;i < this->cardNumber;i++)
    {
        if(card[i] == card)
        {
            findTheCard = true;
        }
        if(findTheCard)
        {
            this->card[i] = this->card[i+1];
        }
    }
    this->cardNumber --;
    if(p[attackTarget].beAttacked(card,0,this->order))
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
    return;
}
void Player::magic(int magicTarget,int card)
{
    putIntoDiscardPile(card);
    bool findTheCard = false;
    for(int i = 0;i < this->cardNumber;i++)
    {
        if(card[i] == card)
        {
            findTheCard = true;
        }
        if(findTheCard)
        {
            this->card[i] = this->card[i+1];
        }
    }
    this->cardNumber --;
    if(getKind(card) == weak || getKind(card) == poison)
    {
        p[magicTarget].status[statusnmber] == getKind(card);
        p[magicTarget].statusnumber ++;
    }
    if(getKind(card) == magicmissile)
    {
        p[magicTarget].massileAttack(card,2,this->order);
    }
}
void Player::massileAttack(int card, int damage,int attacker)
{
    sendMessage(MASSILE,this->order,card,attacker);
    if(returnKind == NOACCEPT)
    {
        if(this->sheildExist())
        {
            this->destroySheild();
            return;
        }
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
    if(returnKind == ACCEPT)
    {
        p[attackTarget].massileAttack(card,damage + 1);
    }
    if(returnKind == OFFSET)
    {
        return;
    }
}
bool Player::sheildExist()
{
    for(int i = 0;i < this->statusnumber;i++)
    {
        if(this->CardList::getName(status[i]) == SHEILD)
        {
            return true;
        }
    }
    return false;
}
bool Player::weakExist()
{
    for(int i = 0;i < this->statusnumber;i++)
    {
        if(this->CardList::getName(status[i]) == WEAK)
        {
            return true;
        }
    }
    return false;
}
bool Player::poisonExist()
{
    int poisonNum = 0;
    {
        for(int i = 0;i < this->statusnumber;i++)
        {
            if(this->CardList::getName(status[i]) == POISON)
            {
                poisonNum ++;
            }
        }
        return poisonNum;
    }
}
void Player::destroySheild()
{
    int findTheSheild = 0;
    for(int i = 0;i < this->statusnumber;i++)
    {
        if(this->CardList::getName(status[i]) == SHEILD)
        {
            findTheSheild ++;
        }
        this->status[i] = this->status[i + findTheSheild];
    }
    this->statusnumber --;
    return;
}
void Player::destroyWeak()
{
    int findTheWeak = 0;
    for(int i = 0;i < this->statusnumber;i++)
    {
        if(this->CardList::getName(status[i]) == WEAK)
        {
            findTheWeak ++;
        }
        this->status[i] = this->status[i + findTheWeak];
    }
    this->statusnumber --;
    return;
}
void Player::destroyPoison()
{
     int temStatusArray[10];
     int temNum = 0;
     for(int i = 0;i < this->statusnumber;i++)
     {
         if(CardList::getName(this->status[i]) == POISON)
         {
             continue;
         }
         temStatusArray[temNum] = this->status[i];
         temNum ++;
     }
     for(int i = 0;i < this->cardLimit;i++)
     {
         this->card[i] = temCardArray[i];
     }
     return;
}
void Player::addStatu(int card)
{
    this->statusnumber ++;
    this->status[this->statusnumber] = card;
}
void Player::buy()
{
    int cardArray[3];
    for(int i = 0;i < 3;i++)
    {
        cardArray = getCard();
    }
    if(team[this->teamNumber].stone < 4)
    {
        team[this->teamNumber].gem ++;
        team[this->teamNumber].crystal ++;
        team[this->teamNumber].stone += 2;
        sendMessage(EXTRACTCARD,cardArray[3]);//Kind = 14;
        return;
    }
    if(team[this->teamNumber].stone == 5)
    {
        sendMessage(EXTRACTCARD,cardArray[3]);//Kind = 14;
        return;
    }
    if(team[this->teamNumber.stone == 4])
    {
        sendMessage(GEMORCYSTAL);//Kind = 15;
        if(returnKind == CRYSTAL)
        {
            Team[this->teamNumber].crystal ++;
            Team[this->teamNumber].stone ++;
        }
        if(returnKind == GEM)
        {
            Team[this->teamNumber].gem ++;
            Team[this->teamNumber].stone ++;
        }
        sendMessage(EXTRACTCARD,cardArray[3]);//Kind = 14;
        return;
    }
}
void Player::extract()
{
    if(Team[this->teamNumber].stone == 1)
    {
        Team[this->teamNumber].stone --;
        if(Team[this->teamNumber].gem == 1)
        {
            Team[this->teamNumber].gem --;
            this->gem ++;
            return;
        }
        Team[this->teamNumber].cystal --;
        this->crystal ++;
        return;
    }
    sendMessage(EXTRACT);//return: int = the number of gem * 10 + the number of cystal
    Team[this->teamNumber].stone -= returnValue / 10 +returnValue % 10;
    Team[this->teamNumber].gem -= returnValue / 10;
    Team[this->teamNumber].crystal -= returnValue % 10;
    this->gem += returnValue / 10;
    this->crystal += returnValue % 10;
    return;
}
void Player::compose()
{
    if(Team[this->teamNumber].stone == 3)
    {
        Team[this->teamNumber].stone == 0;
        Team[this->teamNumber].gem == 0;
        Team[this->teamNumber].crystal == 0;
    }
    sendMessage(COMPOSE);//return: int = the number of gem * 10 + the number of cystal
    Team[this->teamNumber].stone -= 3;
    Team[this->teamNumber].gem -= returnValue / 10;
    Team[this->teamNumber].crystal -= returnValue % 10;
    for(int i = 0;i < 3;i++)
    {
        cardArray = getCard();
    }
    Team[this->teamNumber].grail ++;
    Team[(this->teamNumber + 1) % 2].morale --;
    sendMessage(EXTRACTCARD,cardArray[3]);//Kind = 14;
}
