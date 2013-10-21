#ifndef CARD_H
#define CARD_H

#define WIND 0
#define WATER 1
#define FIRE 2
#define GROUND 3
#define THUNDER 4
#define DARK 5
#define WEAK 6
#define POISON 7
#define SHEILD 8
#define BOMB 9
#define LIGHT 10
#define MAGIC 0
#define ATTACK 1
#define SKILL 0
#define BLOOD 1
#define INTONE 2
#define HOLY 3
#define PURPLE 4
struct paintCard
{
    int cnumber;
    int cstatus;
    int ctype;
    int ckind;
    char* cname;
    int* cskillone;
    int* cskilltwo;
};//This struct is used to send to local to help it paint the card.This struct include all the information needed to
  //paint a special card.

class Card
{
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
