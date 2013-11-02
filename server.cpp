#include "server.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

Server::Server(QObject *parent,int Number) :
    QObject(parent),PlayerNumber(Number)
{
    players = new Player[PlayerNumber];
    srand(time(0));
}

void Server::Game()
{
    init();
    while(1)
    {
        for(int i = 0; i<PlayerNumber/2; i++)
        try
        {
            team[0].player[i]->start();
            team[1].player[i]->start();
        }
        catch(GameTerminate a)
        {
            //...
        }
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
            players[i].order = i;
        }
        random_shuffle(temp[0],temp[0]+PlayerNumber);
        /*  number in team[0] means player is in red team,
         *  number in team[1] means player is in blue team,
         *  number in team[0][0] means player is the first player
         */
        for (int i = 0; i<PlayerNumber;i++)
        {
            team[0].player[i] = &players[temp[0][i]];
            team[0].player[i]->teamNumber = 0;
            team[1].player[i] = &players[temp[1][i]];
            team[1].player[i]->teamNumber = 1;
        }
    }


    /*Choose Role*/

    /*init Piles*/
    Pile = new int[CARD_NUMBER];
    DiscardPile = new int[CARD_NUMBER];
    EndOfPile = Pile+CARD_NUMBER;
    EndOfDiscardPile = DiscardPile;
    NextCard = Pile;
    for(int i = 0; i<CARD_NUMBER; i++) Pile[i] = i;
    random_shuffle(Pile,EndOfPile);

    /*Deal Cards*/
    for (int i = 0; i<PlayerNumber; i++) dealCards(i,4);
}

void Server::shuffleCards()
{
    /*  How to store cards?
     *  How to know whether this card is used or still in Pile(Pai Ku)
     *  How to only shuffle the cards in the discard pile(Qi Pai Dui)
     */
    int* temp = Pile;
    Pile = DiscardPile;
    DiscardPile = temp;

    NextCard = Pile;
    EndOfPile = EndOfDiscardPile;
    EndOfDiscardPile = DiscardPile;
    random_shuffle(Pile,EndOfPile);
}

void Server::dealCards(int id, int number)
{
    /*  Use what to indentify players?*/
    NumberOfLeftCards = NextCard-EndOfPile;
    if (NumberOfLeftCards<number)
    {
        int temp = NumberOfLeftCards;
        dealCards(id,temp);
        shuffleCards();
        dealCards(id,number-temp);
    }
    else
    {
        for(int i = 0; i<number; i++)
        {
            players[id].getCard(*NextCard);
            NextCard++;
        }
    }
}

void Server::fold(int idOfCard)
{
    *EndOfDiscardPile = idOfCard;
    EndOfDiscardPile++;
}
