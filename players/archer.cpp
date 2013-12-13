#include "archer.h"
#include"server.h"
extern CardList cardlist;

/*
  普通技：被动A 【闪电箭】：你的雷系攻击对方无法应战。
  1,普通技：响应A 【贯穿射击】：（主动攻击未命中时发动②，弃1张法术牌【展示】）对你所攻击的目标造成2点法术伤害③。
  2,独有技：法术 【闪光陷阱】：对目标角色造成2点法术伤害③。
  3,独有技：响应A 【精准射击】：此攻击强制命中，但本次攻击伤害-1。
  4,必杀技：法术 【狙击】：【能量】×1 目标角色手牌直接补到5张【强制】，额外+1【攻击行动】。
*/


Archer::Archer(/*QObject *parent*/Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    skill = 0;
    target = -1;
    server->textg->character[order]->setText("弓女");
}

void Archer::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    //assert(this->card.find(cardUsed)!=this->card.end());
    //if(!server->players[attackTarget].canBeAttacked()) throw ActionIllegal();

    connect(server->players[attackTarget],SIGNAL(miss(int)),this,SLOT(skillone(int)));

    int damage = 2;
    target = attackTarget;
    foldCard(&cardUsed);
    //暗灭无法应战，需要吗？
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

//----------闪电箭------------------------------------
    if (cardlist.getName(cardUsed) == thunderAttack)
        canBeAccept = false;
//----------精准射击--------------------------------------
    if (cardlist.getSkillOne(cardUsed) == 32 ||cardlist.getSkillTwo(cardUsed) == 32)
    {
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
            server->textg->textbrowser->append("你响应了精准射击");
            skill = 3;
        }

    }
//-----------------------------------------------------   
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();//展示攻击对象，攻击牌

    if (skill == 3)
    {
        damage = damage -1;
        emit attacked(order, attackTarget, damage);
        (server->team[teamNumber])->getStone(Gem);
        server->players[attackTarget]->countDamage(damage,Attack);
    }
    else if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
//---------------------贯穿射击-----------------------------------------
            sendMessageBuffer[0] = AskRespond;
            sendMessageBuffer[1] = 1;
            sendMessageBuffer[2] = 0;

            sendMessage();

            receive();

            int temp = receiveMessageBuffer[0];
            if (temp != 0)
            {
                 receive();

                 if (receiveMessageBuffer[0] != -1)
                 {
                     sendMessageBuffer[0] = AttackHappen;
                     sendMessageBuffer[1] = target;
                     sendMessageBuffer[2] = receiveMessageBuffer[0];
                     //BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
                     BroadCast();

                     server->textg->textbrowser->append("你响应了贯穿射击");
                     foldCard(&receiveMessageBuffer[0],1,true);//弃牌
                     server->players[target]->countDamage(2,Magic);
                 }
            }
        }
        else
        {
           emit attacked(order, attackTarget, damage);
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }

    disconnect(server->players[attackTarget],SIGNAL(miss(int)),this,SLOT(skillone(int)));
    target = -1;

    emit finishaction(order, Attack);
}

void Archer::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 2)
    {
        server->textg->textbrowser->append("你发动了闪光陷阱");
        magicOne();//闪光陷阱 
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 4)
    {
        server->textg->textbrowser->append("你发动了狙击");
        magicTwo();//狙击
    }
}

void  Archer::magicOne()
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];
    int damage = 2;

    foldCard(&cardUsed);
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
   // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    server->players[magicTarget]->countDamage(damage,Magic);
}


void  Archer::magicTwo()
{
    int magicTarget = receiveMessageBuffer[2];

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    //sendMessageBuffer[2] = cardUsed;
   // BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    useEnergy(1);

    if (server->players[magicTarget]->cardNumber < 5)
    {
         server->players[magicTarget]->getCard(5-server->players[magicTarget]->cardNumber);
    }
    if (server->players[magicTarget]->cardNumber > server->players[magicTarget]->cardLimit)
    {
        Discards(server->players[magicTarget]->cardLimit - server->players[magicTarget]->cardNumber,2);
    }


    emit finishaction(order, Magic);

    sendMessageBuffer[0] = AdditionalAction;
    sendMessageBuffer[1] = 0;
    sendMessage();

    receive();

    if (receiveMessageBuffer[0] == 0)
    {
        normalAttack();
    }

}

void Archer::end()
{
    skill = 0;
}

void Archer::skillone(int order)
{
    if (order == target)
    {
        //---------------------贯穿射击-----------------------------------------
         sendMessageBuffer[0] = AskRespond;
         sendMessageBuffer[1] = 1;
         sendMessageBuffer[2] = 0;

         sendMessage();

         receive();
         int temp = receiveMessageBuffer[0];

         if (temp != 0)
         {
              receive();

              if (receiveMessageBuffer[0] != -1)
              {
                  sendMessageBuffer[0] = AttackHappen;
                  sendMessageBuffer[1] = target;
                  sendMessageBuffer[2] = receiveMessageBuffer[0];
                  //BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
                  BroadCast();

                  server->textg->textbrowser->append("你响应了贯穿射击");
                  foldCard(&receiveMessageBuffer[0],1,true);//弃牌
                  server->players[target]->countDamage(2,Magic);
              }
         }
         target = -1;
    }
}

void Archer::headOn(int chainLength)
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
    //----------闪电箭------------------------------------
        if (cardlist.getName(cardUsed) == thunderAttack)
            canBeAccept = false;

    //------------------精准射击------------------------------------------------
    if (cardlist.getSkillOne(cardUsed) == 11 ||cardlist.getSkillTwo(cardUsed) == 11)
    {
        if (server->players[attackTarget]->shieldExist())
        {
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
            server->textg->textbrowser->append("你响应了精准射击");
            skill = 3;
        }
        }
    }

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    //BroadCast(AttackHappen,order,attackTarget,cardUsed);//展示攻击对象，攻击牌
    BroadCast();

    emit miss(order);
    if(skill == 3)
    {
        damage = damage -1;
        emit attacked(order, attackTarget, damage);
        (server->team[teamNumber])->getStone(Crystal);
        server->players[attackTarget]->countDamage(damage,Attack);
    }
    else if(server->players[attackTarget]->beAttacked(order,cardUsed,chainLength,canBeAccept))
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

    skill = 0;
}


