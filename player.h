#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QObject *parent = 0);
    void get_card(int);
signals:

public slots:
    void GameStart();

};

struct Team
{
    int grail;
    int morale;
    Player* player[3];
    Team();
};
#endif // PLAYER_H
