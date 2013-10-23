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
         //allocate Team
         sendMessage();//Kind = 0;
     }
     for(int i = 0;i < 6;i ++)
     {
         //allocate character;
         sendMessage();//Kind = 19;
     }
     //根据返回的character信息初始化Player[6];
     for(int i = 0;i < 6; i ++)
     {
         int initCard[4];
         for(int j = 0;j < 4;j++)
         {
             initCard[j] = CardPile.getCard();
         }
         sendMessage(EXTRACTCARD,initCard[4]);//Kind = 14;
     }
}

void Server::gameCirculation()
{
    for(int i = 0;i <6 ;i++)
    {
    NextOne:
        sendMessage();//Kind = 1;it's your turn;
        //Check for weak,poison,and some specail card(封印束缚，勇者挑衅等)
        for(int j = player[i]->statusnumber;j != 0;j --)
        {
            switch(CardList::getName(player[i]->status[j - 1]))
            {
                case WEAK:
                {
                    sendMessage(WEAK);//Kind = 18;
                    if(returnKind == ACCEPT)
                    {
                        player[i]->weakRespond();
                        break;
                    }
                    if(returnKind == NOACCEPT)
                    {
                        player[i]->destroyWeak();
                        i ++;
                        if(i == 5)
                        {
                            i = 0;
                        }
                        goto NextOne;
                    }
                }
                case POISON:
                {
                    player[i]->poisonRespond();
                    break;
                }
                case FIVEBOUND:
                {

                }
                case PROVOKE:
                {

                }
                default:
                break;
            }
        }
        player[i]->activate();//Activate or specialActivity
        //wait for information;
        switch(returnInformation)
        {
            case ATTACK:
            {
                player[i]->attack(attackTarget,card,canBeAccept);
            }
            case MAGIC:
            {
                player[i]->magic(magicTarget,card);
            }
            case SPECIALMAGIC:
            {
                //special Magic
            }
        }
    }
}

