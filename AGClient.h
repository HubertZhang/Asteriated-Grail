//
//  AGClient.h
//  Network
//
//  Created by Hubert on 13-11-20.
//  Copyright (c) 2013年 Hubert. All rights reserved.
//

#ifndef __Network__AGClient__
#define __Network__AGClient__

#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;
using std::vector;

#define clientPort 25555
//#define serverPort 25556
class AGClient {
    io_service &m_iosev;
    tcp::endpoint serverAddr;
    ip::tcp::acceptor m_acceptor;
    boost::shared_ptr<tcp::socket> psocket;
    //void acceptHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec);
    //void recieveHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec);
    void readHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec,size_t bytesArrived);
    //void conncetHandler(boost::shared_ptr<tcp::socket> psocket, vector<int> message, boost::system::error_code ec);
    void writeHandler(error_code ec, size_t bytes_transferred);
public:
    AGClient(io_service &iosev);
    int setup(char* addr,unsigned short int serverPort);
    void recieve();
    void send(vector<int> message);
    
    int len;
    vector<int> buf;
};

#endif /* defined(__Network__AGClient__) */
