#ifndef SERVER_H
#define SERVER_H
#include "card.h"
#include "player.h"
#include <QObject>
#include <cstring>
#include <exception>
#define CARD_NUMBER 150

struct Team
{
    int grail[2];
    int morale[2];
    Player* player[3];
};

class Server : public QObject
{
    Q_OBJECT
    static int PlayerNumber;
    Card card[CARD_NUMBER];
    Player* players;
    Team team[2];
    void shuffle_cards();
    void deal_cards(int id,int number);
    void init();
    void playerturn(int t);

    int* Pile;
    int* DiscardPile;
    int* BeginOfPile;                       //current card
    int* EndOfPile;                         //
    int* BeginOfDiscardPile;                //
    int* EndOfDiscardPile;                  //place the discard (point to an empty slot)

public:
    explicit Server(QObject *parent = 0, int Number=4);

    void Game();
signals:

public slots:

};

#endif // SERVER_H
