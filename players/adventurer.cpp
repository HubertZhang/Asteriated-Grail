#include "adventurer.h"
#include"server.h"
extern CardList cardlist;

/*
1,普通技：响应A 【欺诈】：（弃2张同系牌【展示】）视为一次除暗以外的任意系的主动攻击，该系由你决定；

或（弃3张同系牌【展示】）视为一次暗系的主动攻击。
普通技：被动A 【强运】：（每发动一次【欺诈】）你+1【水晶】。
普通技：被动C 【地下法则】：你执行【购买】时，改为【战绩区】+2【宝石】。
2,普通技：被动C 【冒险者天堂】：你执行【提炼】时，可将提炼出的【能量】交给任意一名队友。
3,必杀技：法术 【偷天换日】【回合限定】：【能量】×1 将对方【战绩区】的1【宝石】转移到我方【战绩区】，
  或将我方【战绩区】的【水晶】全部替换为【宝石】；额外+1【攻击行动】或【法术行动】。
*/

Adventurer::Adventurer(Server* server,int order,int teamNumber,int character) :
    Player(server,order,teamNumber,character)
{
}

void Adventurer::magicAction()
{
    if (receiveMessageBuffer[1] == 0)
    {
         normalMagic();
    }
    else if (receiveMessageBuffer[1] == 1)
    {
        magicOne();
    }
}

void Adventurer::refine()
{

    QString s;
    s.sprintf("玩家%d选择提炼",order);
    server->textg->textbrowser->append(s);

    int gem = receiveMessageBuffer[1];
    int crystal = receiveMessageBuffer[2];
    int target = receiveMessageBuffer[3];
    for (int i=0; i<gem; i++)
    {
        (server->team[teamNumber])->lossStone(Gem);
       // energyGem ++;
    }

    for (int i=0; i<crystal; i++)
    {
        (server->team[teamNumber])->lossStone(Crystal);
       // energyCrystal++;
    }
    server->players[target]->energyGem =server->players[target]->energyGem + gem;
    server->players[target]->energyGem =server->players[target]->energyGem + crystal;

    server->players[target]->sendMessageBuffer[0] = EnergyChange;
    server->players[target]->sendMessageBuffer[1] = gem;
    server->players[target]->sendMessageBuffer[2] = crystal;
    server->players[target]->BroadCast();//改变人物能量数量
}

void Adventurer::magicOne()
{
    if (receiveMessageBuffer[2] == 0)
    {
        (server->team[teamNumber])->getStone(Gem);
        (server->team[1-teamNumber])->lossStone(Gem);
    }
    else if (receiveMessageBuffer[2] == 1)
    {
        int crystalnumber = server->team[teamNumber]->crystal;
        for (int i=0; i<crystalnumber; i++)
        {
             (server->team[teamNumber])->lossStone(Crystal);
             (server->team[teamNumber])->getStone(Gem);
        }
    }

    useEnergy(1);
}

void Adventurer::normalAttack()
{
    if (receiveMessageBuffer[1] == 6)
    {
        attackOne();
        return;
    }

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

    emit attacked(order, attackTarget, damage);
    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }
}

void Adventurer::attackOne()
{
    int attackTarget = receiveMessageBuffer[2];
    int usenumber = receiveMessageBuffer[3];
    int cardUsed = receiveMessageBuffer[4];

    int damage = 2;
}
