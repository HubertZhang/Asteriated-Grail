#ifndef CHARMER_H
#define CHARMER_H
#include"player.h"
#include <set>
using std::set;

class Charmer : public Player
{
    int magicalnumber;
    set<int> magicalterm;
    void magicOne();
    void magicTwo();
public:
    Charmer(Server *server, int order, int teamNumber, int character);   
    void normalAttack();
    void magicAction();
};

#endif // CHARMER_H
