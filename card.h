#ifndef CARD_H
#define CARD_H
#include "cardList.h"

class Card
{
    Q_OBJECT
    friend class CardList;
    int cnumber;             //each card will ba allocated with a specific number.
                             //This number is
    int cname;               //name of this card
                             //(There are ten names of card.Wind,water,fire,ground,thunder,dark.weak,poison,sheild,bomb,light)
    int ctype;               //type of this card(There are two types of card. Magic and attack)
    int ckind;               //kind of this card
                             //(There are five kind of card.Green(Skill),Red(blood),Orange(intone),Blue(holy),Purple(magical))
    int cnature;
    int skillone;
    int skilltwo;
    /*How to set skills......*/
public:
    Card();
    paintCard getCardInformation(int cardNumber);
};

#endif // CARD_H
