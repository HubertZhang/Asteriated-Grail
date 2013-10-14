#ifndef CARD_H
#define CARD_H

class Card
{
    int status;             //status of this card
                            //(There are ten status of card.Wind,water,fire,ground,thunder,dark.weak,poison,sheild,light)
    int type;               //type of this card(There are two types of card. Magic and attack)
    int kind;               //kind of this card
                            //(There are five kind of card.Green(Skill),Red(blood),Orange(intone),Blue(holy),Purple(magical))
    char* name;             //name of this card
    /*How to set skills......*/
public:
    Card();
};

#endif // CARD_H
