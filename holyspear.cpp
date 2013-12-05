#include "holyspear.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
#include"player.h"
extern CardList cardlist;

/*
    普通技：被动 【神圣信仰】：你的【治疗】上限+1。
  1.普通技：法术 【辉耀】：（弃1张水系牌【展示】）所有人+1【治疗】，额外+1【攻击行动】。
  2.普通技：法术 【惩戒】：（弃1张法术牌【展示】）将其他角色的1【治疗】转移给你，额外+1【攻击行动】。
    普通技：被动 【圣击】：（攻击命中后发动②）你+1【治疗】。
  3.普通技：响应A 【天枪】：（主动攻击前发动①，移除你的2【治疗】）本次攻击对方不能应战；不能和【圣击】同时发动。
  4.普通技：响应A 【地枪】：（主动攻击命中后发动②，移除你的X【治疗】）本次攻击伤害额外+X，X最高为4；不能和【圣击】同时发动。
  5.必杀技：法术 【圣光祈愈】：【宝石】×1 无视你的治疗上限为你+2【治疗】，但你的【治疗】最高为5，额外+1【攻击行动】；本回合你不能再发动【天枪】。
*/

Holyspear::Holyspear(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    skill = 0;
    cureLimit++;
    server->textg->character[order]->setText("圣枪");
}

void Holyspear::normalAttack()
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

//------------天枪-------------------------------------------------------------
    if (cureNumber >= 2 && skill != 5)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 3;

        sendMessage();

        receive();
        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你响应了天枪");
            decreaseCure(2);
            canBeAccept = false;
            skill = 3;
        }
    }
//-------------------------------------------------------------------------------
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();//展示攻击对象，攻击牌

    emit attacked(order, attackTarget, damage);

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
            (server->team[teamNumber])->getStone(Gem);
//----------------地枪------------------------------------
            if (cureNumber >= 1)
            {
                sendMessageBuffer[0] = AskRespond;
                sendMessageBuffer[1] = 1;
                sendMessageBuffer[2] = 4;

                sendMessage();

                receive();
                if (receiveMessageBuffer[0])
                {
                    int usecurenumber = receiveMessageBuffer[1];
                    server->textg->textbrowser->append("你响应了地枪");
                    decreaseCure(usecurenumber);
                    damage = damage + usecurenumber;
                }
                else if (skill != 3)
                {
                    server->textg->textbrowser->append("你发动了圣击");
                    increaseCure(1);
                }
            }
//----------------圣击----------------------------------------
            else if (skill != 3)
            {
                server->textg->textbrowser->append("你发动了圣击");
                increaseCure(1);
            }
//------------------------------------------------------------
            server->players[attackTarget]->countDamage(damage,Attack);
        }
    }

    skill = 0;
}

void Holyspear::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)//辉耀
    {
        magicOne();
    }
    else if (receiveMessageBuffer[1] == 2)//惩戒
    {
        magicTwo();
    }
    else if (receiveMessageBuffer[1] == 5)//圣光祈愈
    {
        magicThree();
    }
}


void Holyspear::magicOne()
{
     server->textg->textbrowser->append("你发动了辉耀");

     int cardUsed = receiveMessageBuffer[2];
     foldCard(&cardUsed);

     sendMessageBuffer[0] = DrawPicture;
     sendMessageBuffer[1] = 6;
     sendMessageBuffer[2] = 1;
     int i=0;
     for (i=0; i<6; i++)
     {
         sendMessageBuffer[3+i] = i;
     }
     sendMessageBuffer[9] = cardUsed;
     BroadCast();

     for (int j=0 ;j<6; j++)
     {
         server->players[j]->increaseCure(1);
     }

     sendMessageBuffer[0] = AdditionalAction;
     sendMessageBuffer[1] = 0;
     sendMessage();

     receive();

     if (receiveMessageBuffer[0])
     {
        normalAttack();
     }
}


void Holyspear::magicTwo()
{
     server->textg->textbrowser->append("你发动了惩戒");

     int magicTarget = receiveMessageBuffer[2];
     int cardUsed = receiveMessageBuffer[3];
     foldCard(&cardUsed);

     sendMessageBuffer[0] = AttackHappen;
     sendMessageBuffer[1] = magicTarget;
     sendMessageBuffer[2] = cardUsed;
     BroadCast();//展示攻击对象，攻击牌

     server->players[magicTarget]->decreaseCure(1);
     increaseCure(1);

     sendMessageBuffer[0] = AdditionalAction;
     sendMessageBuffer[1] = 0;
     sendMessage();

     receive();

     if (receiveMessageBuffer[0])
     {
        normalAttack();
     }
}


void Holyspear::magicThree()
{
     skill = 5;
     server->textg->textbrowser->append("你发动了圣光祈愈");
     //??????
     sendMessageBuffer[0] = DrawPicture;
     sendMessageBuffer[1] = 0;
     sendMessageBuffer[2] = 0;
     BroadCast();

     useEnergy(1,true);

     if (cureNumber <=3)
         increaseCure(2,false);
     else
         increaseCure(5-cureNumber,false);

     sendMessageBuffer[0] = AdditionalAction;
     sendMessageBuffer[1] = 0;
     sendMessage();

     receive();

     if (receiveMessageBuffer[0])
     {
        normalAttack();
     }
     skill = 0;
}


void Holyspear::headOn(int chainLength)
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
    BroadCast();

    emit miss(order);
    emit attacked(order, attackTarget, damage);

    if(server->players[attackTarget]->beAttacked(order,cardUsed,chainLength,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
          (server->team[teamNumber])->getStone(Crystal);
          server->textg->textbrowser->append("你发动了圣击");
          increaseCure(1);
          server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
}

