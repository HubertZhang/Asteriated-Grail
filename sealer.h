#ifndef Sealer_H
#define Sealer_H
#include"player.h"
#include <set>
using std::set;

class Sealer : public Player
{
    Q_OBJECT

    int number;
    set<int> sealcard[6];
    void magicOne();//五行封印
    void magicTwo();//五系束缚
    void magicThree();//封印破碎
public:
    Sealer(Server* server,int order,int teamNumber,int character);
    void magicAction();
    void characterConnect();

public slots:
    void skillone(int order, int, int w);
    void skilltwo(int order,int);
    void skillthree(int order,int);
};

#endif // Sealer_H
