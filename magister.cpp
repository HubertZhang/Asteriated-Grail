#include "magister.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
#include"player.h"
extern CardList cardlist;
/* 1.普通技：法术 【魔爆冲击】：（弃1张法术牌【展示】）指定2名对手各弃一张法术牌【展示】，若其中有人不如此做，则对他造成2点法术伤害③。只要有一名对手不如此做，我方【战绩区】+1【宝石】。
   2.普通技：被动B 【魔弹掌握】：你主动使用【魔弹】时可以选择逆向传递。
   .普通技：被动B 【魔弹融合】：你的地系或火系牌可以当【魔弹】使用。
   3.必杀技：法术 【毁灭风暴】：【宝石】×1 对任意2名对手各造成2点法术伤害③。
*/

Magister::Magister(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    server->textg->character[order]->setText("魔导");
}

void Magister::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)//魔爆冲击
    {
         magicOne();
    }
    else if (receiveMessageBuffer[1] == 2)//魔弹掌握
    {
         magicTwo();
    }
    else if (receiveMessageBuffer[1] == 3)//毁灭风暴
    {
        magicThree();
    }
}

void Magister::magicOne()//魔爆冲击
{
    int magicTarget[2];
    magicTarget[0] = receiveMessageBuffer[2];
    magicTarget[1] = receiveMessageBuffer[3];
    int cardUsed = receiveMessageBuffer[4];
    int damage = 2;

    foldCard(&cardUsed);
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

    if ((magicTarget[0]+order)%6 >= (magicTarget[1]+order)%6)
    {
        int temp = magicTarget[0];
        magicTarget[0] = magicTarget[1];
        magicTarget[1] = temp;
    }

    bool allfold = true;
    for (int i=0; i<2; i++)
    {
        server->players[magicTarget[i]]->sendMessageBuffer[0] = FoldOneCard;
        server->players[magicTarget[i]]->sendMessageBuffer[1] = 1;
        server->players[magicTarget[i]]->sendMessageBuffer[2] = 1;

        server->players[magicTarget[i]]->sendMessage();
        server->players[i]->receive();

        int card = server->players[i]->receiveMessageBuffer[0];
        if (card == 150)
        {
            allfold = false;
            server->players[magicTarget[i]]->countDamage(damage,Magic);
        }
        else
        {
            server->players[magicTarget[i]]->foldCard(&card,1,true);
        }
    }
    if (!allfold)
    {
        (server->team[teamNumber])->getStone(Gem);
    }
}

void Magister::magicTwo()//魔弹掌握
{
    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    foldCard(&cardUsed);

    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    bool missileCycle[6];
    for (int i=0; i<6; i++)
    {
        missileCycle[i] = false;
    }
    missileCycle[order] = true;

    server->players[magicTarget]->beMagicMissileAttack(2,missileCycle);
}

void Magister::magicThree()
{
    server->textg->textbrowser->append("你发动了毁灭风暴");
    int magicTarget1 = receiveMessageBuffer[2];
    int magicTarget2 = receiveMessageBuffer[3];
    int damage = 2;

    useEnergy(1,true);
    //展示攻击对象，攻击牌
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 2;
    sendMessageBuffer[2] = 0;
    for (int i=0; i<2; i++)
    {
        sendMessageBuffer[i+3] = receiveMessageBuffer[2+i];
    }
    BroadCast();

    if ((magicTarget1+order)%6 >= (magicTarget2+order)%6)
    {
        int temp = magicTarget1;
        magicTarget1 = magicTarget2;
        magicTarget2 = temp;
    }

    server->players[magicTarget1]->countDamage(damage,Magic);
    server->players[magicTarget2]->countDamage(damage,Magic);
}


