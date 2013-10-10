#ifndef SERVER_H
#define SERVER_H
#include "card.h"
#include "player.h"
#include <QObject>
#include <cstring>
#include <exception>
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
    Card card[];
    Player* players;
    Team team[2];
    void shuffle_cards();
    void deal_cards(int id,int number);
    void init();
    void playerturn(int t);
public:
    explicit Server(QObject *parent = 0, int Number=4);

    void Game();
signals:

public slots:

};

#endif // SERVER_H
