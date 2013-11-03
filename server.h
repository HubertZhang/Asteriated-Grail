/*The work that the server should do is to handle the signals that come from outside.
 *
 *
 */
#ifndef SERVER_H
#define SERVER_H
#include "player.h"
#include "card.h"
#include <QObject>

#define CARD_NUMBER 150

class Server : public QObject
{
    Q_OBJECT
    const int PlayerNumber;

    Card card[CARD_NUMBER];



    void init();

    void shuffleCards();
    int* Pile;
    int* DiscardPile;
    int* NextCard;                          //current card
    int* EndOfPile;                         //
    int* EndOfDiscardPile;                  //place the discard (point to an empty slot)

public:
    Team* team;
    Player* players;
    int NumberOfLeftCards;

    explicit Server(QObject *parent = 0, int Number=4);

    void Game();  

    void dealCards(int id,int number);
    void fold(int idOfCard);
signals:

public slots:

};

#endif // SERVER_H
