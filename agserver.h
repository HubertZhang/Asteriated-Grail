//
//  AGServe.h
//  Test1
//
//  Created by Hubert on 13-11-20.
//
//

#ifndef __AGServer_h__
#define __AGServer_h__
#include <iostream>
#include <QtWidgets>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

#include <vector>

using std::vector;

class AGSocket: public QTcpSocket
{
    Q_OBJECT
    friend class AGServer;
public:
    explicit AGSocket(QObject *parent = 0);
    int ID(){return id;}
signals:
    void socketError();
    void readFinished(int id,std::vector<int> message);
public slots:
    void readMessage();
    void onDisconnected(){emit socketError();}
private:
    int id;
};

class AGServer: public QTcpServer
{
    Q_OBJECT
    
signals:
    void serverError();
    void messageRecieved(int id, std::vector<int> message);
    void connectionBuilt();
public slots:
    void onDisconnected(){emit serverError();}
    void readFinished(int id, std::vector<int> message){emit messageRecieved(id, message);}
public:
    explicit AGServer(QWidget *parent = 0,int playerNum = 6,int port = 10000);
    void sendMessage(int id, std::vector<int> message);
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    const int playerNum;
    QList<AGSocket*> socketList;
    QList<int> socketDescriptorList;
};

#endif /* defined(__AGServer_h__) */
