#include "magicsword.h"
#include"server.h"
extern CardList cardlist;

/*
 *1.普通技：响应C 【修罗连斩】【回合限定】：（【攻击行动】结束时发动）额外+1火系【攻击行动】。
  2.普通技：启动 【暗影凝聚】：（对自己造成1点法术伤害③）【横置】持续到你的下个行动阶段开始，你都处于【暗影形态】，脱离【暗影形态】时【重置】。
    普通技：被动A 【暗影之力】：（仅【暗影形态】下发动）你发动的所有攻击伤害额外+1。
    普通技：被动 【暗影抗拒】：在你的行动阶段你始终不能使用法术牌。
  3.普通技：法术 【暗影流星】：（仅【暗影形态】下发动，弃2张法术牌【展示】）对目标角色造成2点法术伤害③。
  4.必杀技：响应A 【黑暗震颤】【回合限定】：【宝石】×1 （主动攻击前发动①）本次攻击对手不能应战，若命中②，你的手牌直接补到上限【强制】。

*/

Magicsword::Magicsword(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    activation = 0;
    combo = false;
    server->textg->character[order]->setText("魔剑");
}

void Magicsword::normalAttack()
{
    int skill = 0;

    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;
//----暗影之力------------------
    if (activation == 1)
     damage++;
//-----------------------------
    foldCard(&cardUsed);

    //暗灭无法应战，需要吗？
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

//-----黑暗震颤------------------
    if (energyGem >= 1)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 1;

        sendMessage();

        receive();
        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你响应了黑暗震颤");
            useEnergy(1,true);
            canBeAccept = false;
            skill = 3;
        }
    }
//------------------------------

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    emit attacked(order, attackTarget, damage);
    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
            if (skill == 3)
            {
                getCard(cardLimit - cardNumber);
            }
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }

    if (!combo)
    {
      sendMessageBuffer[0] = AskRespond;
      sendMessageBuffer[1] = 1;
      sendMessageBuffer[2] = 0;
      sendMessage();
      receive();//是否发动技能

      int temp = receiveMessageBuffer[0];

      if (temp != 0)
      {
         receive();
         if (receiveMessageBuffer[0] != -1)
         {
             receiveMessageBuffer[2] = receiveMessageBuffer[1];
             receiveMessageBuffer[1] = receiveMessageBuffer[0];
             server->textg->textbrowser->append("你发动了修罗连斩");
             combo = true;
             normalAttack();
         }
      }
    }

}

void Magicsword::end()
{
    combo = false;
}

void Magicsword::headOn(int chainLength)
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    int damage = 2;

    //----暗影之力------------------
        if (activation == 1)
         damage++;
    //-----------------------------
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
    emit attacked(order, attackTarget, damage);

    if(server->players[attackTarget]->beAttacked(order,cardUsed,chainLength,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
          (server->team[teamNumber])->getStone(Crystal);
          server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
}

void Magicsword::activate()
{

    activation =1;

    sendMessageBuffer[0] = Activated;
    sendMessageBuffer[1] = 1;
    BroadCast();

    countDamage(1,Magic);
}

bool Magicsword::canActivate()
{
    return !((cardLimit - cardNumber) < 3 && activation == 1 &&
                (energyCrystal+energyGem==3 || server->team[teamNumber]->stone==0));
}

void Magicsword::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 2)
    {
         server->textg->textbrowser->append("你发动了暗影流星");
         magicOne();
    }

}

void Magicsword::magicOne()
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed[2];
    cardUsed[0] = receiveMessageBuffer[3];
    cardUsed[1] = receiveMessageBuffer[4];
    int damage = 2;

    foldCard(cardUsed,2);
   //展示攻击对象，攻击牌
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = 2;
    for (int i=0; i<3; i++)
    {
        sendMessageBuffer[i+3] = receiveMessageBuffer[2+i];
    }
    BroadCast();

    server->players[magicTarget]->countDamage(damage,Magic);
}

void Magicsword::beforeAction()
{
    if (activation == 1)
    {
        activation = 0;

        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 0;
        BroadCast();
    }

    if (!canActivate())
    {
        return;
    }

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
