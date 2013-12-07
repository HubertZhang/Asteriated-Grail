#include "server.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "player.h"
#include"blademaster.h"
#include"archer.h"
#include"sealer.h"
#include"assassin.h"
#include"saintess.h"
#include"magister.h"
#include"magicsword.h"
#include"holyspear.h"
#include"textgui.h"
#include"team.h"
#include"berserker.h"
#include <QApplication>
using namespace std;

void Server::messageReceived(int id, std::vector<int> message)
{
    for(int i = 0; i< message.size();i++)
    {
        players[id]->receiveMessageBuffer[i] = message[i];
    }
    players[id]->getmessage = true;
}

void Server::chooseCharacter(int id, std::vector<int> message)
{
    int a = message[0];
    playercharacter[id] = a;
    characterfinish[id] = true;
}

void Server::BroadCast()
{
    QString s = "*************************************************";
    textg->textbrowser->append(s);

    s = "游戏 begin";
    textg->textbrowser->append(s);
}

Server::Server(QObject *parent,int Number) :
    QObject(parent),PlayerNumber(Number)
{
    gamePile = new CardPile;
    srand(time(0));
    connectionBuilt = false;
    for (int i=0; i<6; i++)
    {
        characterfinish[i] = false;
    }
    connect(&networkServer,SIGNAL(connectionBuilt()),this,SLOT(connectionFinished()));
}

void Server::Game()
{
    for (int i=0; i<6; i++)
    {
        players[i]->characterConnect();
    }

    int gameround = 1;
    //sendMessageBuffer[0] =
    //BroadCast();
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
    case blademaster:
    {
        players[order] = new Blademaster(this,order,teamnumber,character);
        break;
    }
    case berserker:
    {
        players[order] = new Berserker(this,order,teamnumber,character);
        break;
    }
    case archer:
    {
        players[order] = new Archer(this,order,teamnumber,character);
        break;
    }
    case sealer:
    {
        players[order] = new Sealer(this,order,teamnumber,character);
        break;
    }
    case assassin:
    {
        players[order] = new Assassin(this,order,teamnumber,character);
        break;
    }
    case saintess:
    {
        players[order] = new Saintess(this,order,teamnumber,character);
        break;
    }
    case magister:
    {
        players[order] = new Magister(this,order,teamnumber,character);
        break;
    }
    case magicsword:
    {
        players[order] = new Magicsword(this,order,teamnumber,character);
        break;
    }
    case holyspear:
    {
        players[order] = new Holyspear(this,order,teamnumber,character);
        break;
    }
    default:
    {
        players[order] = new Player(this,order,teamnumber,character);
        break;
    }
    }
}
/*
void Server::init(textGUI *a)
{
    int sendMessageBuffer[6];

    while(1)
    {
         QCoreApplication::processEvents();
        if (connectionBuilt)
        break;
    }

    textg = a;
    team[0] = new Team(this,0);
    team[1] = new Team(this,1);
    //Allocate Order
        //int client[PlayerNumber]={0,1,2,3,4,5};
        //random_shuffle(client,client+PlayerNumber);

    //Arrange Team
        int *arrangeteam = new int[PlayerNumber];

       // for (int i=0; i<PlayerNumber/2; i++)
           // arrangeteam[i] = 0;
        //for (int i=0; i<PlayerNumber/2; i++)
           // arrangeteam[i+PlayerNumber/2] = 1;
       // random_shuffle(arrangeteam+1,arrangeteam+PlayerNumber);
        arrangeteam[0] = 0;
        arrangeteam[1] = 1;
        arrangeteam[2] = 0;
        arrangeteam[3] = 1;
        arrangeteam[4] = 0;
        arrangeteam[5] = 1;
   connect(&networkServer,SIGNAL(messageRecieved(int, std::vector<int>)),
                  this,SLOT(chooseCharacter(int, std::vector<int>)));
   //Broadcast Team
        int j=0;

        for (int i=0;i<PlayerNumber;i++)
        {
            if (arrangeteam[i] == 0)
            {
               sendMessageBuffer[j] = i;
               j++;
            }
        }

        for (int i=0; i<PlayerNumber; i++)
        {
            vector<int> tempMessage;
            tempMessage.push_back(0);
            tempMessage.push_back(i);
            for(int j=0; j<3; j++)
            {
                tempMessage.push_back(sendMessageBuffer[j]);
            }
            networkServer.sendMessage(i,tempMessage);
        }
        QCoreApplication::processEvents();

       // system("pause");

    //Choose Role
        int character[27];
        for (int i=0; i<18; i++)
        {
           character[i] = i/3 + 1;
        }
        //for (int i=0; i<3; i++)
        //{
        //    character[i+3] = 3;
        //}
        //character[0] = 6 ;
        //character[1] = 6 ;
        //character[2] = 6 ;
        //character[3] = 6 ;
        //character[4] = 6 ;
        //character[5] = 6 ;
        random_shuffle(character,character+18);

        for(int i=0; i<PlayerNumber;i++)
        {
            vector<int> tempMessage;
            tempMessage.push_back(1);
            for(int j=0; j<3; j++)
            {
                tempMessage.push_back(character[3*i]);
                tempMessage.push_back(character[3*i+1]);
                tempMessage.push_back(character[3*i+2]);
            }
            networkServer.sendMessage(i,tempMessage);
            //sendMessageBuffer[1] = character[3*i];
            //sendMessageBuffer[2] = character[3*i+1];
            //sendMessageBuffer[3] = character[3*i+2];
        }

        QCoreApplication::processEvents();
        //system("pause");
        while(1)
        {
           QCoreApplication::processEvents();
           if (characterfinish[0]&&characterfinish[1]&&characterfinish[2]
                   &&characterfinish[3]&&characterfinish[4]&&characterfinish[5])
           break;
        }
    //Allocate Character
        for (int i=0; i<PlayerNumber; i++)
        {
            allocateCharacter(i,playercharacter[i],arrangeteam[i]);
        }

        disconnect(&networkServer,SIGNAL(messageRecieved(int, std::vector<int>)),
                this,SLOT(chooseCharacter(int, std::vector<int>)));
        connect(&networkServer,SIGNAL(messageRecieved(int, std::vector<int>)),
                this,SLOT(messageReceived(int, std::vector<int>)));

     //Broadcast Character
        vector<int> tempMessage;
        tempMessage.push_back(2);
        for (int i=0; i<PlayerNumber;i++)
        {
            tempMessage.push_back(playercharacter[i]);
        }
        networkServer.sendMessage(-1,tempMessage);

    //Deal Cards
        for (int i = 0; i<PlayerNumber; i++)
        {
            players[i]->getCard(4);
        }

        delete []arrangeteam;
}
*/
void Server::init(textGUI *a)
{
    textg = a;
    team[0] = new Team(this,0);
    team[1] = new Team(this,1);

        int *arrangeteam = new int[PlayerNumber];

        for (int i=0; i<PlayerNumber/2; i++)
            arrangeteam[i] = 0;
        for (int i=0; i<PlayerNumber/2; i++)
            arrangeteam[i+PlayerNumber/2] = 1;
        random_shuffle(arrangeteam+1,arrangeteam+PlayerNumber);

        int character[6];

        character[0] = 8 ;
        character[1] = 8 ;
        character[2] = 8 ;
        character[3] = 8 ;
        character[4] = 8 ;
        character[5] = 8 ;

        for (int i=0; i<PlayerNumber; i++)
        {
            allocateCharacter(i,character[i],arrangeteam[i]);
        }

        for (int i = 0; i<PlayerNumber; i++)
        {
            players[i]->getCard(4);
        }

        delete []arrangeteam;
}

