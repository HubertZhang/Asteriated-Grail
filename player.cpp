#include "player.h"
#include "cardpile.h"
#include "cardList.h"
#include "team.h"
//这一个cpp里面有大量的"."要被替换为"->"
//→ →我不是故意的
Player::Player(QObject *parent) :
    QObject(parent)
{

}
Player* p[6];
Team* team[2];
Player::~Player()
{

}
void Player::activate()
{
    return;
}
void Player::attack(int attackTarget,int card,bool canBeAccept)//canBeAccept = 0:此攻击不可应战
{
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
    if(p[attackTarget]->beAttacked(card,1,this->order,canBeAccept))
    {
        if(p[attackTarget]->cureNumber != 0)
        {
            int cureAmount = p[attackTarget]->useCure(2);
            int realDamage = 2 - cureAmount;
            if(realDamage != 0)
            {
                p[attackTarget]->bearDamage(realDamage,ATTACK);
            }
        }
        else
        {
            p[attackTarget]->bearDamage(2,ATTACK);
        }
    }
}
bool Player::beAttacked(int card,int KindOfAttack,int attacker,bool canBeAccept)
{
    sendMessage(ATTACKED,attacker,card,canBeAccept);//Kind = 3,you are attacked.
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
            if(team[(this->teamNumber + 1)%2]->stone != 5)
            {
                team[(this->teamNumber + 1)%2]->gem ++;
                team[(this->teamNumber + 1)%2]->stone ++;
            }
        }
        if(KindOfAttack == 0)
        {
            if(team[(this->teamNumber + 1)%2]->stone != 5)
            {
                team[(this->teamNumber + 1)%2]->crystal ++;
                team[(this->teamNumber + 1)%2]->stone ++;
            }
        }
        return true;
    }
    if(returnKind == ACCEPT)
    {
        this->acceptAttack(attackTarget,card,canBeAccept);
        return false;
    }
    if(returnKind == OFFSET)//light
    {
        return false;
    }
}
int Player::useCure(int damage)
{
    sendMessage(ASKFORCURE,damage);//Kind = 10;
    int cureAmount = returnAmount;
    return cureAmount;
}
void Player::bearDamage(int damage,int KindOfDamage)//The kind of Damage:Magic or Attack
{
    int damageCard[15];
    for(int i = 0;i < damage;i++)
    {
        damageCard[i] = CardPile::getCard();
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
    team[this->teamNumber]->morale -= amount;
    return;
}
void Player::acceptAttack(int attackTarget,int card,bool canBeAccept)
{
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
    if(p[attackTarget]->beAttacked(card,0,this->order,canBeAccept))
    {
        if(p[attackTarget]->cureNumber != 0)
        {
            int cureAmount = p[attackTarget]->useCure(2);
            int realDamage = 2 - cureAmount;
            if(realDamage != 0)
            {
                p[attackTarget]->bearDamage(realDamage,ATTACK);
            }
        }
        else
        {
            p[attackTarget]->bearDamage(2,ATTACK);
        }
    }
    return;
}
void Player::magic(int magicTarget,int card)
{
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
    if(CardList::getKind(card) == WEAK || CardList::getKind(card) == POISON)
    {
        p[magicTarget]->status[statusnmber] == CardList::getKind(card);
        p[magicTarget]->statusnumber ++;
    }
    if(getKind(card) == MASSILE)
    {
        p[magicTarget]->massileAttack(card,2,this->order);
    }
}
void Player::massileAttack(int card, int damage,int attacker)
{
    sendMessage(MASSILE,card,attacker);//Kind = 20;
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
            if(realDamage != 0)
            {
                p[attackTarget]->bearDamage(realDamage,MAGIC);
            }
        }
        else
        {
            p[attackTarget]->bearDamage(damage,MAGIC);
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
int Player::poisonExist()
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
void Player::destroySheild()//已经有盾的时候才可以调用
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
void Player::destroyWeak()//已经有虚弱的时候才可以调用
{
    int findTheWeak = 0;
    for(int i = 0;i < this->statusnumber;i++)
    {
        if(CardList::getName(this->status[i]) == WEAK)
        {
            findTheWeak ++;
        }
        this->status[i] = this->status[i + findTheWeak];
    }
    this->statusnumber --;
    return;
}
void Player::destroyPoison()//若有多个poison,destroy 排在最后的poison.
{
     for(int i = this->statusnumber;i > 0;i --)
     {
         if(CardList::getName(this->status[i - 1]) == POISON)
         {
             for(int j = (i - 1);j < this->statusnumber - 1; j++)
             {
                 this->status[j] = this->status[j + 1];
             }
             break;
         }
     }
     this->statusnumber --;
     return;
}
void Player::addStatu(int card)
{
    this->status[this->statusnumber] = card;
    this->statusnumber ++;
}
void Player::buy()
{
    int cardArray[3];
    for(int i = 0;i < 3;i++)
    {
        cardArray[i] = CardPile::getCard();
    }
    if(team[this->teamNumber]->stone < 4)
    {
        team[this->teamNumber]->gem ++;
        team[this->teamNumber]->crystal ++;
        team[this->teamNumber]->stone += 2;
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
            team[this->teamNumber]->crystal ++;
            team[this->teamNumber]->stone ++;
        }
        if(returnKind == GEM)
        {
            team[this->teamNumber]->gem ++;
            team[this->teamNumber]->stone ++;
        }
        sendMessage(EXTRACTCARD,cardArray[3]);//Kind = 14;
        return;
    }
}
void Player::extract()
{
    if(team[this->teamNumber]->stone == 1)
    {
        team[this->teamNumber]->stone --;
        if(team[this->teamNumber]->gem == 1)
        {
            team[this->teamNumber]->gem --;
            this->gem ++;
            return;
        }
        team[this->teamNumber]->cystal --;
        this->crystal ++;
        return;
    }
    sendMessage(EXTRACT);//return: int = the number of gem * 10 + the number of cystal
    team[this->teamNumber]->stone -= returnValue / 10 +returnValue % 10;
    team[this->teamNumber]->gem -= returnValue / 10;
    team[this->teamNumber]->crystal -= returnValue % 10;
    this->gem += returnValue / 10;
    this->crystal += returnValue % 10;
    return;
}
void Player::compose()
{
    if(team[this->teamNumber]->stone == 3)
    {
        team[this->teamNumber]->stone == 0;
        team[this->teamNumber]->gem == 0;
        team[this->teamNumber]->crystal == 0;
    }
    sendMessage(COMPOSE);//return: int = the number of gem * 10 + the number of cystal
    team[this->teamNumber]->stone -= 3;
    team[this->teamNumber]->gem -= returnValue / 10;
    team[this->teamNumber]->crystal -= returnValue % 10;
    for(int i = 0;i < 3;i++)
    {
        cardArray = CardList::getCard();
    }
    team[this->teamNumber]->grail ++;
    team[(this->teamNumber + 1) % 2]->morale --;
    sendMessage(EXTRACTCARD,cardArray[3]);//Kind = 14;
}
void Player::weakRespond()
{
    destroyWeak();
    int cardArray[3];
    for(int i = 0;i < 3;i ++)
    {
        cardArray[i] = CardPile::getCard();
    }
    sendMessage(EXTRACTCARD,cardArray[3]);//Kind = 14;
    if(this->cardNumber < 4)
    {
        return;
    }
    this->discardCard(this->cardNumber - 3);
}
void Player::poisonRespond()
{
    destroyPoison();
    if(this->cureNumber != 0)
    {
        sendMessage(ASKFORCURE,1);//Kind = 10;
        if(returnValue = 1)
        {
            return;
        }
        bearDamage(1,MAGIC);
    }
}
