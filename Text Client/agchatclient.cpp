#include "agchatclient.h"

AGChatClient::AGChatClient(QWidget *parent):QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(this,SIGNAL(connected()),this,SLOT(setup()));
}

void AGChatClient::readMessage()
{
    QDataStream socketIn(this);
    socketIn.setVersion(QDataStream::Qt_5_0);
    while (1) {
        std::cout <<this->bytesAvailable()<< "Signal1\n";
        quint16 messageLength= 0;
        if (bytesAvailable() < (int)sizeof(quint16)) return;
        socketIn >> messageLength;

        if (bytesAvailable() < messageLength) return;
        int senderId;
        socketIn >> senderId;
        QString temp;
        socketIn >> temp;
        emit readFinished(senderId,temp);
    }
}

void AGChatClient::setup(int id,char* ip, int port)
{
    abort();
    QString ipAddr(ip);
    QHostAddress serverAddr(ipAddr);
    connectToHost(serverAddr, port);
    this->id = id;
}
void AGChatClient::setup()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << id;
    write(block);
    std::cout << id<< " Writed." << std::endl;
}

void AGChatClient::sendMessage(QString message)
{
    qDebug() << message << endl;
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16)(block.size()-sizeof(quint16));
    write(block);
    qDebug() << "Client data sending...\nSize is: " << block.size()<< " Address is " << peerAddress().toString() << ":" << peerPort() << endl;
}
