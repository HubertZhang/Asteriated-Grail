#ifndef CHATSERVER_H
#define CHATSERVER_H
#include <iostream>
#include <QtWidgets>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

class AGChatSocket: public QTcpSocket
{
    Q_OBJECT
    friend class AGChatServer;
public:
    explicit AGChatSocket(QObject *parent = 0);
    int ID(){return id;}
signals:
    void socketError();
    void readFinished(int id,QString message);
public slots:
    void setup();
    void readMessage();
    void onDisconnected(){emit socketError();}
private:
    int id;
};

class AGChatServer: public QTcpServer
{
    Q_OBJECT

signals:
    void serverError();
    void messageRecieved(int id, QString message);
    void connectionBuilt();
public slots:
    void onDisconnected(){emit serverError();}
    void readFinished(int id, QString message){sendMessage(id, message);}
public:
    explicit AGChatServer(QWidget *parent = 0,int playerNum = 6,int port = 10001);
    void sendMessage(int id, QString message);
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    const int playerNum;
    QList<AGChatSocket*> socketList;
    QList<int> socketDescriptorList;
};

#endif // CHATSERVER_H
