#include "server.h"
#include <cardpile.h>
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
    int current_player = 0;
    while(1)
    {
        try
        {
            playerturn(current_player);
            current_player++;
            if(current_player==6)
                current_player=0;
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
        extern CardPile* getCardPile = new CardPile();
        extern CardPile* discardCardPile = new CardPile();
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
    for (int i = 0; i<PlayerNumber; i++) deal_cards(i,4);
}

void Server::shuffle_cards()
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

void Server::deal_cards(int id, int number)
{
    /*  Use what to indentify players?*/
    NumberOfLeftCards = NextCard-EndOfPile;
    if (NumberOfLeftCards<number)
    {
        int temp = NumberOfLeftCards;
        deal_cards(id,temp);
        shuffle_cards();
        deal_cards(id,number-temp);
    }
    else
    {
        for(int i = 0; i<number; i++)
        {
            players[id].get_card(*NextCard);
            NextCard++;
        }
    }
}
