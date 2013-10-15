/*The work that the server should do is to handle the signals that come from outside.
 *
 *
 */
#ifndef SERVER_H
#define SERVER_H
#include "card.h"
#include "player.h"
#include <QObject>
#include <cstring>
#include <exception>
#define CARD_NUMBER 150

class GameTerminate
{

};

class Server : public QObject
{
    Q_OBJECT
    const int PlayerNumber;

    Card card[CARD_NUMBER];

    Player* players;
    Team team[2];
    void shuffle_cards();
    void deal_cards(int id,int number);
    void init();
    void playerturn(int t);

    int* Pile;
    int* DiscardPile;
    int* NextCard;                          //current card
    int* EndOfPile;                         //
    int* EndOfDiscardPile;                  //place the discard (point to an empty slot)

public:
    explicit Server(QObject *parent = 0, int Number=4);
    int NumberOfLeftCards;
    void Game();
signals:

public slots:

};

#endif // SERVER_H
