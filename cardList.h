#ifndef CARDLIST_H
#define CARDLIST_H
#include "card.h"
#include "cardNameSpace.h"
class CardList
{
    Q_OBJECT
<<<<<<< HEAD
    friend class Player;
=======
    friend class player;
>>>>>>> 2b8d22289801164e8c34f1f9235efe33d45e59e4
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
