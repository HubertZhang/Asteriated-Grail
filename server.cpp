#include "server.h"
#include "cardpile.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "cardList.h"
using namespace std;

Server::Server(QObject *parent) :
    QObject(parent)
{
    players = new Player[PlayerNumber];
    srand(time(0));
}
void Server::gameInit()
{
     gamePile->initExtractPile();
     team[0] = new Team(RED);
     team[1] = new Team(BLUE);
     for(int i = 0;i < 6;i ++)
     {
         //allocate character;
         sendMessage();//Kind = 0;
     }
     //根据返回的character信息初始化Player[6];
}

void Server::gameCirculation()
{
    for(int i = 0;i <6 ;i++)
    {
        sendMessage();//Kind = 1;it's your turn;
        //Check for weak,poison,and some specail card(封印束缚，勇者挑衅等)
        for(int j = player[i]->statusnumber;j != 0;j --)
        {
            switch(CardList::getName(j))
            {
                case WEAK:
                {
                    sendMessage(WEAK);//Kind = 18;
                    if(returnKind == ACCEPT)
                    {

                    }
                }
                case POISON
            }
        }
        sendMessage();//Kind = 2;Special activity ?
    }
}

void Server::init()
{
    /*Arrenge Team*/
    {
        int temp[2][PlayerNumber/2];
        for (int i = 0; i<PlayerNumber;i++)
        {
            *(temp[0]+i) = i;
        }
        random_shuffle(temp[0],temp[0]+PlayerNumber);
        /*  number in team[0] means player is in red team,
         *  number in team[0] means player is in blue team,
         *  number in team[0][0] means player is the first player
         */
        for (int i = 0; i<PlayerNumber;i++)
        {
            team[0].player[i] = &players[temp[0][i]];
            team[1].player[i] = &players[temp[1][i]];
        }
    }
;
}
