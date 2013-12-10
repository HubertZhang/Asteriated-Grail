#ifndef CARDLIST_H
#define CARDLIST_H
#include<QString>

enum Kind{skill,blood,holy,magical,intone};
enum Name{groundAttack,waterAttack,fireAttack,windAttack,thunderAttack,darkAttack,poison,weak,missile,shield,holyLight};
enum Type{attack,magic};
enum Nature{ground,water,fire,wind,thunder,dark,light};
struct Card
{
                             //each card will ba allocated with a specific number.                             //This number is
    int cname;               //name of this card
                             //(There are ten names of card.Wind,water,fire,ground,thunder,dark.weak,poison,sheild,bomb,light)
    int ctype;               //type of this card(There are two types of card. Magic and attack)
    int ckind;               //kind of this card
                             //(There are five kind of card.Green(Skill),Red(blood),Orange(intone),Blue(holy),Purple(magical))
    int cnature;
    int cskillone;
    int cskilltwo;
    /*How to set skills......*/
    public:
    /*
    Card(int name,int type,int kind,int nature, int skillone, int skilltwo):{}
    int getName();
    int getType();
    int getKind();
    int getNature();
    int getSkillOne();
    int getSkillTwo();
    */
};

class CardList
{
private:
    Card *cardList;
    void initCardList();
public:
    CardList();
    ~CardList(){}
    int getName(int cardNum);
    int getType(int cardNum);
    int getKind(int cardNum);
    int getNature(int cardNum);
    int getSkillOne(int cardNum);
    int getSkillTwo(int cardNum);
    //测试-------------------------
    QString getQName(int cardNum);
    //----------------------------
};
//CardList cardlist;
#endif // CARDLIST_H
