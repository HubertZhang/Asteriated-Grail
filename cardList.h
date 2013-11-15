#ifndef CARDLIST_H
#define CARDLIST_H

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
    int getSkillOne(int cardNum);
    int getSkillTwo(int cardNum);
};
//CardList cardlist;
#endif // CARDLIST_H
