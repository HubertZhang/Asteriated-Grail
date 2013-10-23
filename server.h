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
private:
    int* Pile;
    int* DiscardPile;
    int* NextCard;                          //current card
    int* EndOfPile;                         //
    int* EndOfDiscardPile;                  //place the discard (point to an empty slot)

public:

};

#endif // SERVER_H
