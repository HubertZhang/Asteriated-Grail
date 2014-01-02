/*The work that the server should do is to handle the signals that come from outside.
 *
 *
 */
#ifndef SERVER_H
#define SERVER_H
#include "player.h"
#include "cardpile.h"
#include "team.h"
#include "agserver.h"
#include "chatserver.h"
#include "textgui.h"
#include "team.h"

#include <cstring>
#include <exception>

#include <QObject>
#define CARD_NUMBER 150
enum messageType1{ArrangeTeam,ArrangeCharacter,BroadCastCharacter};
enum character{normal,blademaster,berserker,archer,sealer,assassin,saintess,angel,magister,magicsword,holyspear,
              elementalist,adventurer,necromancer,arbiter,oracle,prayer,paladin,charmer,swordking,
               flighter,brave,soulwarlock,psychic};
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
    bool connectionBuilt;
public:
    bool characterfinish[6];
    int playercharacter[6];
    int playerOrder;
    Team* team[2];
    Player* players[6];
    CardPile* gamePile;
    explicit Server(QObject *parent = 0, int Number=6);
    void Game();
    AGServer networkServer;
    AGChatServer chatServer;
//--------------测试---------------------------
    void init(textGUI*);
    textGUI *textg;
    void BroadCast();
    void sendMessage();

signals:

public slots:
    void messageReceived(int id,std::vector<int> message);
    void chooseCharacter(int id,std::vector<int> message);
    void connectionFinished(){connectionBuilt = true;}
//--------------------------------------------
};

#endif // SERVER_H
