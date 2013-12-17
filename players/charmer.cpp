#include "charmer.h"
#include"server.h"
extern CardList cardlist;

/*
 *1.普通技：法术 【灵符 - 雷鸣】：（弃1张雷系牌【展示】）对任意2名角色各造成1点法术伤害③。
  2.普通技：法术 【灵符 - 风行】：（弃1张风系牌【展示】）指定2名角色各弃1张牌。
    普通技：响应B 【念咒】：你每次发动【灵符】，可将自己1张手牌面朝下放置在你的角色旁，作为【妖力】。
    普通技：响应A 【百鬼夜行】：（主动攻击命中后发动②，移除1个【妖力】）对目标角色造成1点法术伤害③；若【妖力】为火系牌，则可以选择【展示】，改为指定2名角色，对除他们以外的其他所有角色各造成1点法术伤害③。
    必杀技：响应B 【灵力崩解】：【能量】×1 （和【灵符 - 雷鸣】或【百鬼夜行】同时发动）你的本次【灵符 - 雷鸣】或【百鬼夜行】每次造成的伤害额外+1。
*【妖力】为灵符师专有盖牌，上限为2；若【妖力】达到上限，则不能发动【念咒】。你可以随时查看自己的盖牌，但不能替换它，只能通过百鬼夜行使用掉。
*/

Charmer::Charmer(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    magicalnumber = 0;
    server->textg->character[order]->setText("灵符");
}

void Charmer::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)//灵符 - 雷鸣
    {
         magicOne();
         emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 2)//灵符 - 风行
    {
         magicTwo();
         emit finishaction(order, Magic);
    }
}

void Charmer::magicOne()
{
    server->textg->textbrowser->append("你发动了灵符雷鸣");

    int magicTarget[2];
    magicTarget[0] = receiveMessageBuffer[2];
    magicTarget[1] = receiveMessageBuffer[3];
    int cardUsed = receiveMessageBuffer[4];
    int damage = 1;

    foldCard(&cardUsed);
//----------灵力崩解-----------------
    if ( (energyGem+energyCrystal) >= 1)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 2;

        sendMessage();

        receive();

        if (receiveMessageBuffer[0])
        {
            server->textg->textbrowser->append("你发动了灵力崩解");
            useEnergy(1);
            damage++;
        }
    }
//----------------------------------------
    //展示攻击对象，攻击牌
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 2;
    sendMessageBuffer[2] = 1;
    for (int i=0; i<2; i++)
    {
        sendMessageBuffer[i+3] = receiveMessageBuffer[2+i];
    }
    sendMessageBuffer[5] = cardUsed;
    BroadCast();

//------念咒---------------
    if (magicalnumber != 2)
    {
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;

        sendMessage();

        receive();

        if (receiveMessageBuffer[0])
        {
            receive();
           if (receiveMessageBuffer[0]!= -1)
           {
               card.erase(receiveMessageBuffer[0]);
               cardNumber = card.size();

               sendMessageBuffer[0] = CardChange;
               sendMessageBuffer[1] = -1;
               BroadCast();


               sendMessageBuffer[0] = SpecialChange;
               sendMessageBuffer[1] = 1;
               sendMessageBuffer[2] = 0;
               BroadCast();

               server->textg->textbrowser->append("你发动了念咒");
               magicalnumber++;
               magicalterm.insert(receiveMessageBuffer[0]);
           }
        }
    }
//--------------------------
    if ((magicTarget[0]+6-order)%6 >= (magicTarget[1]+6-order)%6)
    {
        int temp = magicTarget[0];
        magicTarget[0] = magicTarget[1];
        magicTarget[1] = temp;
    }

    server->players[magicTarget[0]]->countDamage(damage,Magic);
    server->players[magicTarget[1]]->countDamage(damage,Magic);
}

void Charmer::magicTwo()
{
      server->textg->textbrowser->append("你发动了灵符风行");

      int magicTarget[2];
      magicTarget[0] = receiveMessageBuffer[2];
      magicTarget[1] = receiveMessageBuffer[3];
      int cardUsed = receiveMessageBuffer[4];
      foldCard(&cardUsed);

      sendMessageBuffer[0] = DrawPicture;
      sendMessageBuffer[1] = 2;
      sendMessageBuffer[2] = 1;
      for (int i=0; i<2; i++)
      {
          sendMessageBuffer[i+3] = receiveMessageBuffer[2+i];
      }
      sendMessageBuffer[5] = cardUsed;
      BroadCast();
 //------念咒------------------------------------------
      if (magicalnumber != 2)
      {
          sendMessageBuffer[0] = AskRespond;
          sendMessageBuffer[1] = 1;
          sendMessageBuffer[2] = 0;

          sendMessage();

          receive();

          if (receiveMessageBuffer[0])
          {
              receive();
             if (receiveMessageBuffer[0]!= -1)
             {
                 card.erase(receiveMessageBuffer[0]);
                 cardNumber = card.size();

                 sendMessageBuffer[0] = CardChange;
                 sendMessageBuffer[1] = -1;
                 BroadCast();

                 sendMessageBuffer[0] = SpecialChange;
                 sendMessageBuffer[1] = 1;
                 sendMessageBuffer[2] = 0;
                 BroadCast();

                 server->textg->textbrowser->append("你发动了念咒");
                 magicalnumber++;
                 magicalterm.insert(receiveMessageBuffer[0]);
             }
          }
      }
//-----------------------------------------------------

      if ((magicTarget[0]+6-order)%6 >= (magicTarget[1]+6-order)%6)
      {
          int temp = magicTarget[0];
          magicTarget[0] = magicTarget[1];
          magicTarget[1] = temp;
      }

      for (int i=0; i<2; i++)
      {
          if (server->players[magicTarget[i]]->cardNumber != 0)
          {
              server->players[magicTarget[i]]->sendMessageBuffer[0] = FoldOneCard;
              server->players[magicTarget[i]]->sendMessageBuffer[1] = 0;
              server->players[magicTarget[i]]->sendMessageBuffer[2] = 0;

              server->players[magicTarget[i]]->sendMessage();
              server->players[magicTarget[i]]->receive();

              int card = server->players[magicTarget[i]]->receiveMessageBuffer[0];

              server->players[magicTarget[i]]->foldCard(&card);
          }
      }
}

void Charmer::normalAttack()
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

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
            if (magicalnumber >= 1)
            {
                sendMessageBuffer[0] = AskRespond;
                sendMessageBuffer[1] = 1;
                sendMessageBuffer[2] = 1;

                sendMessage();

                receive();

                if (receiveMessageBuffer[0])
                {
                    receive();

                    if (receiveMessageBuffer[0] != -1)
                    {
                        server->textg->textbrowser->append("你发动了百鬼夜行");
                        int magicdamage = 1;
                        if (receiveMessageBuffer[0] == 0)
                        {
                            int cardUsed = receiveMessageBuffer[1];
                            int magicTarget = receiveMessageBuffer[2];

                            magicalnumber--;
                            magicalterm.erase(cardUsed);
                            server->gamePile->putIntoPile(cardUsed);

                            sendMessageBuffer[0] = SpecialChange;
                            sendMessageBuffer[1] = -1;
                            sendMessageBuffer[2] = 0;
                            BroadCast();

                            sendMessageBuffer[0] = DrawPicture;
                            sendMessageBuffer[1] = 1;
                            sendMessageBuffer[3] = magicTarget;
                            sendMessageBuffer[2] = 0;
                            BroadCast();

                            if ( (energyGem+energyCrystal) >= 1)
                            {
                                sendMessageBuffer[0] = AskRespond;
                                sendMessageBuffer[1] = 1;
                                sendMessageBuffer[2] = 2;

                                sendMessage();

                                receive();

                                if (receiveMessageBuffer[0])
                                {
                                   server->textg->textbrowser->append("你发动了灵力崩解");
                                   magicdamage++;
                                   useEnergy(1);
                                }
                            }

                            server->players[magicTarget]->countDamage(magicdamage,Magic);
                        }
                        else
                        {
                            int cardUsed = receiveMessageBuffer[1];
                            int magicTarget1 = receiveMessageBuffer[2];
                            int magicTarget2 = receiveMessageBuffer[3];

                            magicalnumber--;
                            magicalterm.erase(cardUsed);
                            server->gamePile->putIntoPile(cardUsed);

                            sendMessageBuffer[0] = DrawPicture;
                            sendMessageBuffer[1] = 4;
                            sendMessageBuffer[2] = 1;
                            int j = 0;
                            for (int i=0; i<6; i++)
                            {
                                if (i != magicTarget1 && i != magicTarget2)
                                sendMessageBuffer[j++] = i;
                            }
                            sendMessageBuffer[7] = cardUsed;
                            BroadCast();

                            if ( (energyGem+energyCrystal) >= 1)
                            {
                                sendMessageBuffer[0] = AskRespond;
                                sendMessageBuffer[1] = 1;
                                sendMessageBuffer[2] = 2;

                                sendMessage();

                                receive();

                                if (receiveMessageBuffer[0])
                                {
                                   server->textg->textbrowser->append("你发动了灵力崩解");
                                   magicdamage++;
                                   useEnergy(1);
                                }
                            }

                            for (int i=order; i<6+order; i++)
                            {
                                if ((i%6) != magicTarget1 && (i%6) != magicTarget2)
                                {
                                    server->players[i%6]->countDamage(magicdamage,Magic);
                                }
                            }

                        }
                    }
                }
            }

            emit attacked(order, attackTarget, damage);
            (server->team[teamNumber])->getStone(Gem);
            server->players[attackTarget]->countDamage(damage,Attack);
        }
    }

    emit finishaction(order, Attack);
}
