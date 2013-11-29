#ifndef AGCHATCLIENT_H
#define AGCHATCLIENT_H

#include <iostream>
#include <QtWidgets>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

class AGChatClient: public QTcpSocket
{
    Q_OBJECT
signals:
    void socketError();

    void readFinished(int id,QString message);
private slots:
    void readMessage();
    void setup();
    void onDisconnected(){emit socketError();}
public:
    AGChatClient(QWidget *parent = 0);
    void setup(int id,char* ip = NULL, int port = 10001);
    void sendMessage(QString message);
    int id;
};

#endif // AGCHATCLIENT_H
