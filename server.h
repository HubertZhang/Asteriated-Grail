/*The work that the server should do is to handle the signals that come from outside.
 *
 *
 */
#ifndef SERVER_H
#define SERVER_H
#include "player.h"
#include "card.h"
#include <QObject>
#include "cardpile.h"
#include "team.h"
#include <cstring>
#include <exception>
#include "AGServer.h"
#define CARD_NUMBER 150
enum messageType1{ArrangeTeam,ArrangeCharacter,BroadCastCharacter};
enum character{normal,blademaster};
class textGUI;
class Server : public QObject
{
    Q_OBJECT
    const int PlayerNumber;
//    Card card[CARD_NUMBER];
    //void init(){}
    void allocateCharacter(int order,int character,int teamnumber);
    //void BroadCast();
    //void sendMessage();
public:
    Team* team[2];
    Player* players[6];
    CardPile* gamePile;
    explicit Server(QObject *parent = 0, int Number=6);
   // void Game();
    int sendMessageBuffer[20];

//--------------测试---------------------------
    void init(textGUI*);
    textGUI *textg;
    void BroadCast();
    void sendMessage();
    io_service* iosev;
    AGServer* NetworkServer;
public slots:
    void getMessage();
    void receiveReady();
signals:

public slots:
void Game();
//--------------------------------------------
};

#endif // SERVER_H
