#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTcpSocket>
#include "team.h"
#include "termination.h"
enum messageType{TurnBegin,AttackHappen,TakeDamage,Discard,TurnEnd};
enum actionType{Attack,Magic,Purchase,Refine,Fusion};
enum reactionType{AcceptAttack,HeadOn};

class Server;
class Player : public QObject
{
    Q_OBJECT
private:
    Server* server;
    Team* thisTeam;
    QTcpSocket client;
    set<int> card;//手牌内容
public:
    int order;//座位编号
    int cardLimit;//手牌上限
    int cardNumber;//手牌数量
    int cureLimit;//治疗上限
    int cureNumber;//治疗数量
    int teamNumber;//队伍编号
    int status[10];//状态栏
    int statusnumber;//状态烂数量
    int energyGem;
    int energyCrystal;

    void start();
    void BroadCast(messageType a,int origin,int target,...);
    void sendMessage(messageType a,int,int,int* =NULL);
    void foldCard(int idOfCard);
    void getCard(int idOfCard);//
    actionType receive(int*);

    virtual void activate();//启动


    virtual void purchase();
    virtual void refine(int gem,int crystal);
    virtual void fusion();

    virtual void attack(int attackTarget, int cardUsed, int chainLength);
    virtual bool canBeAttacked();
    virtual bool beAttacked(int cardUsed, int chainLength);

    virtual int useCure(int damage);
    virtual void takeDamage(int damage);
    //virtual void magic(int magicTarget,int card);
    //virtual void acceptAttack(int attackTarget,int card);
    virtual void Discards(int amount);
    //virtual void beMagicMissileAttack(int card,int damage);
    //virtual void skillOne();
    //virtual void skillTwo();
    //virtual void skillThree();

    Player();
};
#endif // PLAYER_H
