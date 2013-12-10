#include "chatserver.h"

AGChatSocket::AGChatSocket(QObject *parent) : QTcpSocket(parent)
{
    this->id = -1;
}
void AGChatSocket::setup()
{
    QDataStream socketIn(this);
    socketIn.setVersion(QDataStream::Qt_5_0);
    while (1)
    {
        qDebug() <<this->bytesAvailable()<< "Signal1\n";
        if (bytesAvailable() < (int)sizeof(int)) continue;
        socketIn >> id;
        qDebug() <<id<< " ID received\n";
        disconnect(this,SIGNAL(readyRead()),this,SLOT(setup()));
        connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
        return;
    }
}
void AGChatSocket::readMessage()
{
    QDataStream socketIn(this);
    socketIn.setVersion(QDataStream::Qt_5_0);
    while (1)
    {
        qDebug() <<this->bytesAvailable()<< "Signal2\n";
        quint16 messageLength= 0;
        if (bytesAvailable() < sizeof(quint16)) return;
        socketIn >> messageLength;

        if (bytesAvailable() < messageLength) return;
        QString temp;
        socketIn >> temp;
        qDebug() << id << ": message received: " << temp << endl;
        emit readFinished(id, temp);
    }

}

AGChatServer::AGChatServer(QWidget *parent,int playerNum,int port):playerNum(playerNum),QTcpServer(parent)
{
    this->socketList.clear();
    listen(QHostAddress::Any,port);
}

void AGChatServer::incomingConnection(qintptr socketDescriptor)
{
    if(socketList.size()>=playerNum) return;
    AGChatSocket* tempSocket = new AGChatSocket;
    tempSocket->setSocketDescriptor(socketDescriptor);

    socketList.append(tempSocket);
    socketDescriptorList.append(socketDescriptorList);

    connect(tempSocket,SIGNAL(disconnected()),tempSocket,SLOT(onDisconnected()));
    connect(tempSocket,SIGNAL(socketError()),this,SLOT(onDisconnected()));
    connect(tempSocket,SIGNAL(disconnected()),tempSocket,SLOT(deleteLater()));
    connect(tempSocket,SIGNAL(readyRead()),tempSocket,SLOT(setup()));
    connect(tempSocket,SIGNAL(readFinished(int,QString)),this,SLOT(readFinished(int,QString)));
    qDebug()<< socketList.size() <<"th Client connected, ip is" << tempSocket->peerAddress().toString() << endl;
}

void AGChatServer::sendMessage(int id,QString message)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out<<(quint16) 0;
    out<<id;
    out<<message;
    out.device()->seek(0);
    out<<(quint16)(block.size() - sizeof(quint16));
    QTcpSocket *tempSocket;
        for (int i = 0; i<socketList.size(); i++) {
            tempSocket = socketList.at(i);
            tempSocket->write(block);
        }
    qDebug() << "Server data sending...\nSize is: " << block.size() << "\n";
}
