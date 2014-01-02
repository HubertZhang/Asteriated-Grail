#include "psychic.h"
#include"server.h"
extern CardList cardlist;

/*普通技：法术 【同生共死】：（摸2张牌【强制】，将【同生共死】放置于目标角色面前）
                          在【普通形态】下，你和他手牌上限各-2。
                          在【流血形态】下，你和他手牌上限各+1。
  普通技：启动 【血之哀伤】：（对自己造成2点法术伤害③）转移【同生共死】的目标或移除【同生共死】。
  普通技：被动D 【流血】【持续】：（当你因承受伤害而导致我方士气减少时强制发动【强制】）
                            【横置】转为【流血形态】，你+1【治疗】。
                            此形态下在你的每次回合开始时对自己造成1点法术伤害③。
                              （自身手牌<3时强制触发【强制】）
                            【重置】脱离【流血形态】。
  普通技：法术 【逆流】：（仅【流血形态】下发动）你弃2张牌，你+1【治疗】。
  独有技：法术 【血之悲鸣】：（仅【流血形态】下发动）对目标角色和自己各造成1~3点法术伤害③，数值由你决定。
  必杀技：法术 【血之诅咒】：【宝石】×1 对目标角色造成2点法术伤害③，你弃3张牌。
*/

Psychic::Psychic(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
    specialTarget = -1;
    Blood = false;
    server->textg->character[order]->setText("巫女");
}

void Psychic::start()
{
    sendMessageBuffer[0] = TurnBegin;
    BroadCast(); //回合开始

    try{
        if (Blood)
        {
            server->textg->textbrowser->append("流血形态伤害:");
            countDamage(1,Magic);
        }
    handleStatus();
    beforeAction();

    int i = 1;
    while(i--)
    {
        try{
        sendMessageBuffer[0] = ActionType;
        sendMessage();

        receive();
        int returnAcction = receiveMessageBuffer[0];
        if (returnAcction == Attack)
        {
            normalAttack();
        }
        else if (returnAcction == Magic)
        {
            magicAction();
        }
        }
        catch(ActionIllegal)
        {
            i++;//Do the While Loop again
        }
    }
    }
    catch(int)
    {

    }
    end();
}

void Psychic::BroadCast()
{
    Player::BroadCast();
    if (sendMessageBuffer[0] == CardChange && cardNumber<3)
    {
        if (Blood)
        {
            server->textg->textbrowser->append("转为普通形态:");
            Blood = false;
            if (specialTarget != -1)
            {
               server->players[specialTarget]->changeCardLimit2(-3);
               changeCardLimit2(-3);
            }
        }
    }
}

void Psychic::Discards(int amount, int kind)
{
    sendMessageBuffer[0] = FoldCard;
    sendMessageBuffer[1] = amount;
    sendMessage();

    receive();

    foldCard(receiveMessageBuffer,amount);

    (server->team[teamNumber])->lossMorale(amount);

    if (amount > 0 && (kind == Attack || kind == Magic))
    {
        if (!Blood)
        {
            server->textg->textbrowser->append("转为流血形态:");
            increaseCure(1);
            Blood = true;
            if (specialTarget != -1)
            {
            server->players[specialTarget]->changeCardLimit2(3);
            changeCardLimit2(3);
            }
        }
    }
}

void Psychic::activate()
{
    if (receiveMessageBuffer[1] == 1)
    {
        server->textg->textbrowser->append("请选择目标");
        receive();
        if (receiveMessageBuffer[0] != -1 && receiveMessageBuffer[0] != specialTarget)
        {
            countDamage(2,Magic);
            if (Blood)
            {
                server->players[specialTarget]->changeCardLimit2(-1);
                specialTarget = receiveMessageBuffer[0];
                server->players[specialTarget]->changeCardLimit2(1);
            }
            else
            {
                server->players[specialTarget]->changeCardLimit2(2);
                specialTarget = receiveMessageBuffer[0];
                server->players[specialTarget]->changeCardLimit2(-2);
            }
        }
    }
    else if (receiveMessageBuffer[1] == 2)
    {
        countDamage(2,Magic);
        if (Blood)
        {
            server->players[specialTarget]->changeCardLimit2(-1);
            changeCardLimit2(-1);
            specialTarget = -1;
        }
        else
        {
            server->players[specialTarget]->changeCardLimit2(2);
            changeCardLimit2(2);
            specialTarget = -1;
        }
    }
}

void Psychic::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)//同生共死
    {
         magicOne();
         emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 2)//逆流
    {
         magicTwo();
         emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 3)//血之悲鸣
    {
        magicThree();
        emit finishaction(order, Magic);
    }
    else if (receiveMessageBuffer[1] == 4)//血之诅咒
    {
        magicFour();
        emit finishaction(order, Magic);
    }
}

void Psychic::magicOne()
{
    server->textg->textbrowser->append("同生共死");
    int magicTarget = receiveMessageBuffer[2];

    takeDamage(2,2);
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    BroadCast();

    specialTarget = magicTarget;  
    connect(server->players[specialTarget],SIGNAL(changecardlimit(int)),this,SLOT(skillone(int)));

    if (Blood)
    {
        server->players[specialTarget]->changeCardLimit2(1);
        changeCardLimit2(1);
    }
    else
    {
        server->players[specialTarget]->changeCardLimit2(-2);
        changeCardLimit2(-2);
    }
}
void Psychic::magicTwo()
{  
    server->textg->textbrowser->append("逆流");

    int number = receiveMessageBuffer[2];

    foldCard(receiveMessageBuffer+3,number);

    increaseCure(1);
}
void Psychic::magicThree()
{    
    server->textg->textbrowser->append("血之悲鸣:");

    int magicTarget = receiveMessageBuffer[2];
    int cardUsed = receiveMessageBuffer[3];

    server->textg->textbrowser->append("请选择伤害数:");
    receive();

    int damage = receiveMessageBuffer[0];

    foldCard(&cardUsed);
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = magicTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();

    if ((6-server->playerOrder+order)%6 > (6-server->playerOrder+magicTarget)%6)
    {
        server->players[magicTarget]->countDamage(damage,Magic);
        countDamage(damage,Magic);
    }
    else
    {
        countDamage(damage,Magic);
        server->players[magicTarget]->countDamage(damage,Magic);
    }
}
void Psychic::magicFour()
{
    server->textg->textbrowser->append("血之哀伤:");

    int magicTarget = receiveMessageBuffer[2];
    int number = receiveMessageBuffer[3];
    sendMessageBuffer[0] = DrawPicture;
    sendMessageBuffer[1] = 1;
    sendMessageBuffer[3] = magicTarget;
    sendMessageBuffer[2] = 0;
    BroadCast();

    useEnergy(1,true);
    foldCard(receiveMessageBuffer+4,number);

    server->players[magicTarget]->countDamage(2,Magic);
}

void Psychic::skillone(int target)
{
    if (target == specialTarget)
    {
    if (Blood)
    {
        server->players[specialTarget]->changeCardLimit2(1);
    }
    else
    {
        server->players[specialTarget]->changeCardLimit2(-2);
    }
    }
}
