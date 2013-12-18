#include "paladin.h"
#include"server.h"
extern CardList cardlist;

/*
 * 普通技：被动 【智慧法典】：你的【能量】上限+1；（你每次承受法术伤害时⑥，若该伤害>3）你+2【宝石】。
   普通技：响应D 【法术反弹】：（你每次承受法术伤害时⑥，若该伤害仅为1点，则可以弃X张同系牌【展示】（X>1））对目标角色造成（X-1）点法术伤害③，并对自己造成X点法术伤害③
   1.必杀技：法术 【魔道法典】：【宝石】×1 （弃X张不同系的牌【展示】（X>1））对目标角色与自己各造成（X-1）点法术伤害③。
   2.必杀技：法术 【圣洁法典】：【宝石】×1 （弃X张不同系的牌【展示】（X>2））最多（X-2）名角色各+2【治疗】，并对自己造成（X-1）点法术伤害③。
*/

Paladin::Paladin(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    stonelimit++;
    server->textg->character[order]->setText("xianzhe");
}

void Paladin::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 2)//魔道法典
    {
         magicOne();
         emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 3)//圣洁法典
    {
         magicTwo();
         emit finishaction(order, Magic);
    }
}

void Paladin::magicOne()
{
    server->textg->textbrowser->append("你发动了魔道法典");
    int MagicTarget = receiveMessageBuffer[2];
    int cardnumber = receiveMessageBuffer[3];
    int damage =cardnumber - 1;

    foldCard(receiveMessageBuffer+4,cardnumber);

    useEnergy(1,true);

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[2] = cardnumber;
    sendMessageBuffer[3] = MagicTarget;
    for (int i=0; i<cardnumber; i++)
    {
        sendMessageBuffer[i+4] = receiveMessageBuffer[i+4];
    }
    BroadCast();

    server->players[MagicTarget]->countDamage(damage,Magic);
    countDamage(damage,Magic);
}

void Paladin::magicTwo()
{
    server->textg->textbrowser->append("你发动了圣洁法典");
    int targetnumber = receiveMessageBuffer[2];
    int cardnumber = receiveMessageBuffer[3+targetnumber];
    int damage =cardnumber - 1;

    foldCard(receiveMessageBuffer+targetnumber+4,cardnumber);

    useEnergy(1,true);

    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = targetnumber;
    sendMessageBuffer[2] = cardnumber;
    for (int i=0; i<targetnumber; i++)
    {
        sendMessageBuffer[i+3] = receiveMessageBuffer[i+4];
    }
    for (int i=0; i<cardnumber; i++)
    {
        sendMessageBuffer[targetnumber+3+i] = receiveMessageBuffer[targetnumber+4];
    }
    BroadCast();

    for (int i=0; i<targetnumber; i++)
    {
        server->players[receiveMessageBuffer[i+3]]->increaseCure(2);
    }

    countDamage(damage,Magic);
}

void Paladin::takeDamage(int damage, int kind)
{
    getCard(damage);

    if(cardNumber > cardLimit)
    {
        Discards(cardNumber-cardLimit,kind);
    }

    if (kind == Magic && damage > 3)
    {
        if (energyCrystal + energyGem + 2 <= stonelimit)
        {
           energyGem = energyGem + 2;

           sendMessageBuffer[0] = EnergyChange;
           sendMessageBuffer[1] = 2;
           sendMessageBuffer[2] = 0;
           BroadCast();//改变人物能量数量
        }
        else if (energyCrystal + energyGem + 1 == stonelimit)
        {
           energyGem = energyGem + 1;

           sendMessageBuffer[0] = EnergyChange;
           sendMessageBuffer[1] = 1;
           sendMessageBuffer[2] = 0;
           BroadCast();//改变人物能量数量
        }

    }
    else if (kind == Magic && damage == 1)
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
               server->textg->textbrowser->append("你发动了法术反弹");
               int MagicTarget = receiveMessageBuffer[0];
               int cardnumber = receiveMessageBuffer[1];
               int damage =cardnumber - 1;

               foldCard(receiveMessageBuffer+2,cardnumber);

               sendMessageBuffer[0] = DrawPicture;
               sendMessageBuffer[1] = 1;
               sendMessageBuffer[2] = cardnumber;
               sendMessageBuffer[3] = MagicTarget;
               for (int i=0; i<cardnumber; i++)
               {
                   sendMessageBuffer[i+4] = receiveMessageBuffer[i+2];
               }
               BroadCast();


               server->players[MagicTarget]->countDamage(damage,Magic);
               countDamage(damage+1,Magic);
           }
        }
    }
}
