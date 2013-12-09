#include "elementalist.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
extern CardList cardlist;
/*
 *0.普通技：响应D 【元素吸收】：（对目标角色造成法术伤害时发动③）你+1【元素】。
 *1. 普通技：法术 【元素点燃】：（移除3点【元素】）对目标角色造成2点法术伤害③，额外+1【法术行动】；不能和【元素吸收】同时发动。
  2. 独有技：法术 【陨石】：对目标角色造成1点法术伤害③，额外+1【法术行动】；（若你额外弃1张地系牌【展示】①）本次伤害额外+1。
  3. 独有技：法术 【风刃】：对目标角色造成1点法术伤害③，额外+1【攻击行动】；（若你额外弃1张风系牌【展示】①）本次伤害额外+1。
  4. 独有技：法术 【火球】：对目标角色造成2点法术伤害③；（若你额外弃1张火系牌【展示】①）本次伤害额外+1。
  5. 独有技：法术 【雷击】：对目标角色造成1点法术伤害③，我方【战绩区】+1【宝石】；（若你额外弃1张雷系牌【展示】①）本次伤害额外+1。
  6. 独有技：法术 【冰冻】：对目标角色造成1点法术伤害③，并指定1名角色+1【治疗】；（若你额外弃1张水系牌【展示】①）本次伤害额外+1。
  7. 必杀技：法术 【月光】：【宝石】×1 对目标角色造成（X+1）点法术伤害③，X为你剩余的【能量】数。
 */
Elementalist::Elementalist(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    element = 0;
    thepoison = -1;
    server->textg->character[order]->setText("元素");
}

void Elementalist::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)//元素点燃
    {
         magicOne();
    }
    else if (receiveMessageBuffer[1] == 2)//陨石,风刃,火球,雷击,冰冻
    {
         magicTwo();
    }
    else if (receiveMessageBuffer[1] == 3)//月光
    {
        magicThree();
    }
}


void Elementalist::magicOne()
{
    int magicTarget = receiveMessageBuffer[2];

    int damage = 2;

    element = element -3;
    sendMessageBuffer[0] = SpecialChange;
    sendMessageBuffer[1] = -3;
    sendMessageBuffer[2] = 0;
    BroadCast();

    server->textg->textbrowser->append("你发动了元素点燃");

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    BroadCast();

    server->players[magicTarget]->countDamage(damage,Magic);

    sendMessageBuffer[0] = AdditionalAction;
    sendMessageBuffer[1] = 1;
    sendMessage();

    receive();

    if (receiveMessageBuffer[0] == 1)
    {
        magicAction();
    }
}

void Elementalist::magicTwo()
{
    int magicTarget = receiveMessageBuffer[2];
    int usenumber = receiveMessageBuffer[3];
    int cardUsed = receiveMessageBuffer[4];
    int damage = 1;

    foldCard(receiveMessageBuffer+4,usenumber);
    //展示攻击对象，攻击牌
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = usenumber;
    sendMessageBuffer[3] = magicTarget;
    for (int i=0; i<usenumber; i++)
    {
        sendMessageBuffer[i+4] = receiveMessageBuffer[4+i];
    }
    BroadCast();

    elementIncrease();

    int cardnature = cardlist.getNature(cardUsed);

    if (cardnature == ground)
    {
        server->textg->textbrowser->append("你发动了陨石");

        server->players[magicTarget]->countDamage(damage + usenumber -1,Magic);

        sendMessageBuffer[0] = AdditionalAction;
        sendMessageBuffer[1] = 1;
        sendMessage();

        receive();

        if (receiveMessageBuffer[0] == 1)
        {
            magicAction();
        }
    }
    else if (cardnature == wind)
    {
        server->textg->textbrowser->append("你发动了风刃");

        server->players[magicTarget]->countDamage(damage + usenumber -1,Magic);

        sendMessageBuffer[0] = AdditionalAction;
        sendMessageBuffer[1] = 0;
        sendMessage();

        receive();

        if (receiveMessageBuffer[0] == 0)
        {
            normalAttack();
        }
    }
    else if (cardnature == fire)
    {
        server->textg->textbrowser->append("你发动了火球");

        server->players[magicTarget]->countDamage(damage + usenumber,Magic);
    }
    else if (cardnature == thunder)
    {
        server->textg->textbrowser->append("你发动了雷击");

        server->players[magicTarget]->countDamage(damage + usenumber -1,Magic);
        server->team[teamNumber]->getStone(Gem);
    }
    else if (cardnature == water)
    {
        server->textg->textbrowser->append("你发动了冰冻");

        server->players[magicTarget]->countDamage(damage + usenumber -1,Magic);

        sendMessageBuffer[0] = AskRespond1;

        sendMessage();

        receive();

        int target = receiveMessageBuffer[0];
        server->players[target]->increaseCure(1);
    }
}

void Elementalist::magicThree()
{
    int magicTarget = receiveMessageBuffer[2];
    server->textg->textbrowser->append("你发动了月光");

    useEnergy(1,true);

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    BroadCast();

     elementIncrease();

    int damage = 1 + energyCrystal + energyGem;

    server->players[magicTarget]->countDamage(damage,Magic);
}

void Elementalist::elementIncrease()
{
    if (element < 3)
    {
        element++;
        sendMessageBuffer[0] = SpecialChange;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;
        BroadCast();

        QString s;
        s.sprintf("玩家%d有 %d元素",order,element);
        server->textg->textbrowser->append(s);
    }
}

void Elementalist::normalMagic()
{

    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    int cardname = cardlist.getName(cardUsed);
    switch (cardname) {
    case weak:
    case poison:
    {
        if (cardname == poison)
        {
            disconnect(server->players[magicTarget],SIGNAL(decreasestatus(int,int)),this,SLOT(skillone(int,int)));
            disconnect(server->players[magicTarget],SIGNAL(bepoison(int)),this,SLOT(skilltwo(int)));
            thepoison = cardUsed;
            isdiscard = 0;
            connect(server->players[magicTarget],SIGNAL(decreasestatus(int,int)),this,SLOT(skillone(int,int)));
            connect(server->players[magicTarget],SIGNAL(bepoison(int)),this,SLOT(skilltwo(int)));
        }

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
         BroadCast();

        bool missileCycle[6];
        for (int i=0; i<6; i++)
        {
            missileCycle[i] = false;
        }
        missileCycle[order] = true;

        connect(server->players[target],SIGNAL(bemissile(int)),this,SLOT(skillthree(int)));
        thedamage = 2;
        server->players[target]->beMagicMissileAttack(2,missileCycle);
        disconnect(server->players[target],SIGNAL(bemissile(int)),this,SLOT(skillthree(int)));
        break;
    }
    default:
        break;
    }
}

void Elementalist::skillone(int order,int card)
{
    if (card == thepoison)
    isdiscard ++;
}
void Elementalist::skilltwo(int card)
{
    if (card == thepoison && isdiscard == 1)
    elementIncrease();
}
void Elementalist::skillthree(int damage)
{
    if (thedamage == damage)
    elementIncrease();
}
void Elementalist::beMagicMissileAttack(int damage, bool *missileCycle)
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

        connect(server->players[target],SIGNAL(bemissile(int)),this,SLOT(skillthree(int)));
        thedamage = damage+1;
        server->players[target]->beMagicMissileAttack(damage+1,missileCycle);
        disconnect(server->players[target],SIGNAL(bemissile(int)),this,SLOT(skillthree(int)));
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










