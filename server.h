/*The work that the server should do is to handle the signals that come from outside.
 *
 *
 */
#ifndef SERVER_H
#define SERVER_H
#include "card.h"
#include "player.h"
#include "cardpile.h"
#include "team.h"
#include <QObject>
#include <cstring>
#include <exception>

class GameTerminate
{

};

class Server : public QObject
{
    Q_OBJECT
private:
    CardPile* gamePile;
    Player* player[6];
    Team* team[2];

public:
    void gameCirculation();
    void gameInit();
};

#endif // SERVER_H
