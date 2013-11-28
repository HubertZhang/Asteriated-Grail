//
//  AGServe.cpp
//  Test1
//
//  Created by Hubert on 13-11-20.
//
//
#include "AGClient.h"

AGClient::AGClient(QWidget *parent):QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
}

void AGClient::readMessage()
{
    QDataStream socketIn(this);
    socketIn.setVersion(QDataStream::Qt_5_0);
    while (1) {
        std::cout <<this->bytesAvailable()<< "Signal1\n";
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
        emit readFinished(temp);
    }
}

void AGClient::setup(char* ip, int port)
{
    abort();
    QString ipAddr(ip);
    QHostAddress serverAddr(ipAddr);
    connectToHost(serverAddr, port);
}
void AGClient::sendMessage(vector<int> message)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (int)(message.size());
    
    for (int i = 0; i<message.size(); i++) {
        out << message[i];
    }
    write(block);
    qDebug() << "Client data sending...\nSize is: " << block.size()<< " Address is " << peerAddress().toString() << ":" << peerPort() << endl;
}
