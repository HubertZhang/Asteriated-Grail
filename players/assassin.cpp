#include "assassin.h"
#include"server.h"
extern CardList cardlist;

/*
   普通技：被动D 【反噬】：（承受攻击伤害时发动⑥）攻击你的对手摸1张牌【强制】。
   1,普通技：响应D 【水影】：（任何人对你造成伤害时发动③）弃X张水系牌【展示】。
   2,必杀技：启动 【潜行】：【宝石】×1 【横置】持续到你的下个行动阶段开始，你的手牌上限-1；
   你不能成为主动攻击的目标；你的主动攻击对方无法应战且伤害额外+X，X为你剩余的【能量】数。【潜行】的效果结束时【重置】。
 */
Assassin::Assassin(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    Attacker = -1;
    activation = 0;
    server->textg->character[order]->setText("暗杀");
}

void Assassin::characterConnect()
{
    for (int i=0; i<6; i++)
    {
    connect(server->players[i],SIGNAL(attacked(int,int,int&)),this,SLOT(skillone(int,int,int&)));
    }
}

void Assassin::takeDamage(int damage,int kind)
{
    getCard(damage);

    if(cardNumber > cardLimit)
    {
        Discards(cardNumber-cardLimit,kind);
    }

    //--------------反噬----------------------
    if (kind == Attack)
    {
       server->textg->textbrowser->append("你使用了反噬");
       server->players[Attacker]->takeDamage(1,2);
    }
    //----------------------------------------
    Attacker = -1;

}

void Assassin::countDamage(int damage,int kind)
{
//-----------水影-----------------------------
        sendMessageBuffer[0] = AskRespond;
        sendMessageBuffer[1] = 1;
        sendMessageBuffer[2] = 0;

        sendMessage();

        receive();
        if (receiveMessageBuffer[0])
        {
            receive();
            if (receiveMessageBuffer[0] != -1)
            {
               server->textg->textbrowser->append("你响应了水影");
               foldCard(receiveMessageBuffer+1,receiveMessageBuffer[0],true);
            }
        }
//-------------------------------------------

    if(cureExist())
    {
        int realDamage = damage - useCure(damage);
        if (realDamage > 0)
        takeDamage(realDamage,kind);
    }
    else
    {
        takeDamage(damage,kind);
    }
}

void Assassin::normalAttack()
{
    int attackTarget = receiveMessageBuffer[1];
    int cardUsed = receiveMessageBuffer[2];
    //assert(this->card.find(cardUsed)!=this->card.end());
    //if(!server->players[attackTarget].canBeAttacked()) throw ActionIllegal();

    int damage = 2;
    foldCard(&cardUsed);

    //暗灭无法应战，需要吗？
    bool canBeAccept;
    if (cardlist.getName(cardUsed) == darkAttack)
        canBeAccept = false;
    else
        canBeAccept = true;

//----------潜行------------------------------------
    if (activation == 1)
    {
        server->textg->textbrowser->append("你在潜下攻击");
        damage = damage + energyCrystal + energyGem;
        canBeAccept = false;
    }
//--------------------------------------------------
    sendMessageBuffer[0] = AttackHappen;
    sendMessageBuffer[1] = attackTarget;
    sendMessageBuffer[2] = cardUsed;
    BroadCast();//展示攻击对象，攻击牌

    if(server->players[attackTarget]->beAttacked(order,cardUsed,1,canBeAccept))
    {
        if(server->players[attackTarget]->shieldExist())//If there is a shield...
        {
            server->players[attackTarget]->destroySheild();
        }
        else
        {
            emit attacked(order, attackTarget, damage);
           (server->team[teamNumber])->getStone(Gem);
           server->players[attackTarget]->countDamage(damage,Attack);
        }
    }

    emit finishaction(order, Attack);
}
void Assassin::activate()
{
    activation = 1;
    changeCardLimit(-1);

    sendMessageBuffer[0] = Activated;
    sendMessageBuffer[1] = 1;
    BroadCast();

    useEnergy(1,true);

}

bool Assassin::canActivate()
{
    return !((cardLimit - cardNumber) < 3 && (activation == 1 || energyGem==0) &&
                (energyCrystal+energyGem==stonelimit || server->team[teamNumber]->stone==0));
}

void Assassin::beforeAction()
{
    if (activation == 1)
    {
        activation = 0;
        changeCardLimit(1);

        sendMessageBuffer[0] = Activated;
        sendMessageBuffer[1] = 0;
        BroadCast();
    }

    if (!canActivate())
    {
        return;
    }

    //int receiveMessageBuffer[20];
    sendMessageBuffer[0] = BeforeAction;
    sendMessage();
    //actionType returnAcction = receive(receiveMessageBuffer);
    //测试----------------------------
    getmessage = false;
    //--------------------------------
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

void Assassin::skillone(int attackplayer, int target, int &damage)
{
    if (target == order)
    {
        Attacker = attackplayer;
    }
}
