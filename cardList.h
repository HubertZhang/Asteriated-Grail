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
    int getName(int cardNum);
    int getType(int cardNum);
    int getKind(int cardNum);
    int getSkillOne(int cardNum);
    int getSkillTwo(int cardNum);
};

#endif // CARDLIST_H
