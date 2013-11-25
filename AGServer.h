//
//  AGServer.h
//  Network
//
//  Created by Hubert on 13-11-20.
//  Copyright (c) 2013年 Hubert. All rights reserved.
//

#ifndef __Network__AGServer__
#define __Network__AGServer__
#include <QObject>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <vector>
using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;
using std::vector;

//#define clientPort 25555
#define serverPort 25556
class AGServer :public QObject
{
    Q_OBJECT
private:
    io_service &m_iosev;
    tcp::endpoint clientAddr[6];
    ip::tcp::acceptor m_acceptor;
    ip::tcp::acceptor** m_pacceptor;
    boost::shared_ptr<tcp::socket> psocket;
    boost::shared_ptr<tcp::socket> csocket[6];

    void readHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec,size_t bytesArrived);
    void writeHandler(error_code ec, size_t bytes_transferred);
public:
    AGServer(io_service &iosev);

    void setup(int* listeningPort);
    void recieve(int i);
    void send(vector<int> message,int i);

    vector<int> buf[6];
signals:
    void receiveReady();
};

#endif /* defined(__Network__AGServer__) */
