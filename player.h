#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT
public:
    int order;//座位编号
    int cardLimit;//手牌上限
    int cardNumber;//手牌数量
    int cureLimit;//治疗上限
    int cureNumber;//治疗数量
    int card[15];//手牌内容
    int teamNumber;//队伍编号
    int status[10];//状态栏
private:
    virtual void activate();//启动
    virtual void attack(int attackTarget,int card);
    virtual bool beAttacked(int card,int kindOfAttack);
    virtual int useCure(int damage);
    virtual void bearDamage(int damage);
    virtual void magic();
    virtual void acceptAttack(int attackTarget,int card);
    virtual void discardCard();
    virtual void skillOne();
    virtual void skillTwo();
    virtual void skillThree();

};
#endif // PLAYER_H
