#ifndef CARDLIST_H
#define CARDLIST_H
#include<QString>
class Card;
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
