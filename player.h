#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT
protected:
    int order;//座位编号
    int cardLimit;//手牌上限
    int cardNumber;//手牌数量
    int cureLimit;//治疗上限
    int cureNumber;//治疗数量
    int card[15];//手牌内容
    int teamNumber;//队伍编号
    int status[10];//状态栏
    int statusnumber;//状态烂数量
    int crystal;//水晶数量
    int gem;//宝石数量
    int stonelimit;//能量上限
public:
    bool sheildExist();
    bool weakExist();
    int poisonExist();
    void destroySheild();
    void destroyWeak();
    void destroyPoison();
    void addStatu(int card);
    void buy();
    void compose();
    void extract();
    virtual void activate();//启动
    virtual void attack(int attackTarget,int card);
    virtual bool beAttacked(int card,int kindOfAttack,int attacker);
    virtual int useCure(int damage);
    virtual void bearDamage(int damage);
    virtual void magic(int magicTarget,int card);
    virtual void acceptAttack(int attackTarget,int card);
    virtual void discardCard();
    virtual void massileAttack(int card,int damage,int attacker);
    virtual void specialSkill();
};
#endif // PLAYER_H
