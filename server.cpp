#include "server.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "player.h"
#include"textgui.h"
#include"team.h"
#include <QApplication>
using namespace std;

void Server::BroadCast()
{
    QString s = "*************************************************";
    textg->textbrowser->append(s);

    s = "游戏 begin";
    textg->textbrowser->append(s);
}

void Server::sendMessage()
{

}

Server::Server(QObject *parent,int Number) :
    QObject(parent),PlayerNumber(Number)
{
    //创建队伍
    /*
    team[0] = new Team(this,0);
    team[1] = new Team(this,1);
    */
    gamePile = new CardPile;
    //init();
    srand(time(0));
}


void Server::Game()
{
    int gameround = 1;
    BroadCast();
    try
    {
    while(1)
    {
        //BroadCast();
        for(int i = 0; i<PlayerNumber; i++)
        {
            players[i]->start();
            //QCoreApplication::processEvents();
        }
         QCoreApplication::processEvents();
        gameround++;
        if (gameround == 10) break;
    }
    }
    catch(...)//GameTerminate a
    {
        //...
    }
    //BroadCast();

    for (int i=0; i<PlayerNumber; i++)
    {
        delete []players[i];
    }

}

void Server::allocateCharacter(int order,int character,int teamnumber)
{
    switch(character)
    {
        default:
        {
           players[order] = new Player(this,order,teamnumber,character);
        }
    }
}

void Server::init(textGUI *a)
{
    textg = a;
    team[0] = new Team(this,0);
    team[1] = new Team(this,1);
    /*Allocate Order*/
        //int client[PlayerNumber]={0,1,2,3,4,5};
        //random_shuffle(client,client+PlayerNumber);

    /*Arrange Team*/
        int *arrangeteam = new int[PlayerNumber];

        for (int i=0; i<PlayerNumber/2; i++)
            arrangeteam[i] = 0;
        for (int i=0; i<PlayerNumber/2; i++)
            arrangeteam[i+PlayerNumber/2] = 1;

        random_shuffle(arrangeteam+1,arrangeteam+PlayerNumber);
        /*  number 0 in temp[i] means player[i] is in red team,
         *  number 1 in temp[i] means player[i] is in blue team,
         *  player[0] is the first player
         */

   /*Broadcast Team*/
        /*
        int sendMessageBuffer[20];
        int j=0;
        for (int i=0;i<PlayerNumber;i++)
        {
            if (arrangeteam[i] == 1)
            {
               sendMessageBuffer[2+j] = i;
               j++;
            }
        }
        for (int i=0; i<PlayerNumber; i++)
        {
        sendMessageBuffer[0] = ArrangeTeam;
        sendMessageBuffer[1] = arrangeteam[i];
        sendMessage();//向玩家发送
        }
       */

    /*Choose Role*/

        int character[31];
        for (int i=0; i<31; i++)
        {
            character[i] = i+1;
        }        
        random_shuffle(character,character+31);
        /*
        for(int i=0; i<PlayerNumber;i++)
        {
            sendMessageBuffer[0] = ArrangeCharacter;
            sendMessageBuffer[1] = character[3*i];
            sendMessageBuffer[2] = character[3*i+1];
            sendMessageBuffer[3] = character[3*i+2];
            sendMessage();
        }
        //receive();
        */

    /*Allocate Character*/
        for (int i=0; i<PlayerNumber; i++)
        {
            allocateCharacter(i,character[i],arrangeteam[i]/*,client[i]*/);
        }

    /*Broadcast Character*/
        /*
        sendMessageBuffer[0] = BroadCastCharacter;
        for (int i=0; i<PlayerNumber;i++)
        {
            sendMessageBuffer[i+1] = character[i];
        }
        for(int i=0; i<PlayerNumber;i++)
        {
            sendMessage();
        }
        */
    /*Deal Cards*/
        for (int i = 0; i<PlayerNumber; i++)
        {
            players[i]->getCard(4);
            /*(2)
            for (int j=0; j<4; j++)
            {
                players[i]->card.insert(gamePile->getCard());
            }
            players[i]->cardNumber = card.size();
            sendMessage();
            */
        }

        delete []arrangeteam;
}

