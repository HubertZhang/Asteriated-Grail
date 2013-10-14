#include "server.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

Server::Server(QObject *parent,int Number) :
    QObject(parent)
{
    this->PlayerNumber = Number;
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
        catch
        {
            //...
        }
    }
}

void Server::init()
{
    /*init Piles*/
    Pile = new int[CARD_NUMBER];
    DiscardPile = new int[CARD_NUMBER];
    BeginOfPile = Pile;
    EndOfPile = Pile+CARD_NUMBER;
    BeginOfDiscardPile = DiscardPile;
    EndOfDiscardPile = DiscardPile;
    for(int i = 0; i<CARD_NUMBER; i++) DiscardPile[i]=Pile[i] = i;

    /*Arrenge Team*/
    srand(time(0));
    int team[2][PlayerNumber/2];
    for (int i = 0; i<PlayerNumber;i++)
    {
        *(team[0]+i) = i;
    }
    random_shuffle(team[0],team[0]+PlayerNumber);
/*  number in team[0] means player is in red team,
 *  number in team[0] means player is in blue team,
 *  number in team[0][0] means player is the first player
 */
    /*Choose Role*/

    /*Shuffle Cards*/
    shuffle_cards();
    /*Deal Cards*/
    for (int i = 0; i<PlayerNumber; i++) deal_cards(i,4);
}

void Server::shuffle_cards()
{
    /*  How to store cards?
     *  How to know whether this card is used or still in Pile(Pai Ku)
     *  How to only shuffle the cards in the discard pile(Qi Pai Dui)
     */
    random_shuffle(BeginOfDiscardPile,EndOfDiscardPile);
    int*
}

void Server::deal_cards(int id, int number)
{
    /*  Use what to indentify players?*/
}
