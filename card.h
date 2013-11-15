#ifndef CARD_H
#define CARD_H

enum Kind{skill,blood,holy,magical,intone};
enum Name{groundAttack,waterAttack,fireAttack,windAttack,thunderAttack,darkAttack,poison,weak,missile,shield,holyLight};
enum Type{attack,magic};
enum Nature{ground,water,fire,wind,thunder,dark,light};
class Card
{
    friend class CardList;
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

#endif // CARD_H
//This is my branch
