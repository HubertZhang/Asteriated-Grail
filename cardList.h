#ifndef CARDLIST_H
#define CARDLIST_H
#include "card.h"

class CardList
{
private:
    Card cardList[150];
public:
    CardList();
    ~CardList();
    void initCardList();
    void getName(int cardNum);
    void getType(int cardNum);
    void getKind(int cardNum);
    void getSkillOne(int cardNum);
    void getSkillTwo(int cardNum);
};

#endif // CARDLIST_H
