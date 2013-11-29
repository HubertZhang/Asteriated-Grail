#ifndef AGCLIENT_H
#define AGCLIENT_H
#include <iostream>
#include <QtWidgets>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

#include <vector>

using std::vector;
class AGClient: public QTcpSocket
{
    Q_OBJECT
signals:
    void socketError();
    void idReceived(int id);
    void readFinished(std::vector<int> message);
private slots:
    void readMessage();
    void getId();
    void onDisconnected(){emit socketError();}
public:
    AGClient(QWidget *parent = 0);
    void setup(char* ip = NULL, int port = 10000);
    void sendMessage(vector<int> message);
    int id;
};

#endif // AGCLIENT_H
