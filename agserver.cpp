//
//  AGServe.cpp
//  Test1
//
//  Created by Hubert on 13-11-20.
//
//

#include "agserver.h"

AGSocket::AGSocket(QObject *parent) : QTcpSocket(parent)
{
    this->id = -1;
}

void AGSocket::readMessage()
{
    QDataStream socketIn(this);
    socketIn.setVersion(QDataStream::Qt_5_0);
    while (1) {
        qDebug() <<this->bytesAvailable()<< "Signal1\n";
        int messageLength= 0;
        if (bytesAvailable() < (int)sizeof(int)) return;
        socketIn >> messageLength;
        
        if (bytesAvailable() < (int)sizeof(int)*messageLength) return;
        std::vector<int> temp;
        for (int i = 0; i<messageLength; i++) {
            int tempMessage;
            socketIn >>tempMessage;
            temp.push_back(tempMessage);
        }
        emit readFinished(id, temp);
    }
}

AGServer::AGServer(QWidget *parent,int playerNum,int port):playerNum(playerNum),QTcpServer(parent)
{
    this->socketList.clear();
    listen(QHostAddress::Any,port);
}

void AGServer::incomingConnection(qintptr socketDescriptor)
{
    if(socketList.size()>=playerNum) return;
    AGSocket* tempSocket = new AGSocket;
    tempSocket->setSocketDescriptor(socketDescriptor);
    
    socketList.append(tempSocket);
    socketDescriptorList.append(socketDescriptorList);
    tempSocket->id = socketList.size()-1;
    connect(tempSocket,SIGNAL(disconnected()),tempSocket,SLOT(onDisconnected()));
    connect(tempSocket,SIGNAL(socketError()),this,SLOT(onDisconnected()));
    connect(tempSocket,SIGNAL(disconnected()),tempSocket,SLOT(deleteLater()));
    connect(tempSocket,SIGNAL(readyRead()),tempSocket,SLOT(readMessage()));
    connect(tempSocket,SIGNAL(readFinished(int,std::vector<int>)),this,SLOT(readFinished(int,std::vector<int>)));
    qDebug()<< socketList.size() <<"th Client connected, ip is" << tempSocket->peerAddress().toString().toStdString() << endl;
}

void AGServer::sendMessage(int id,vector<int> message)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (int)(message.size());
    
    for (int i = 0; i<message.size(); i++) {
        out << message[i];
    }
    QTcpSocket *tempSocket;
    if (id==-1) {
        for (int i = 0; i<socketList.size(); i++) {
            tempSocket = socketList.at(i);
            tempSocket->write(block);
        }
    }
    else
    {
        tempSocket = socketList.at(id);
        tempSocket->write(block);
    }
    qDebug() << "Server data sending...\nSize is: " << block.size() << "\n";
}
