//
//  AGClient.cpp
//  Network
//
//  Created by Hubert on 13-11-20.
//  Copyright (c) 2013年 Hubert. All rights reserved.
//

#include "AGClient.h"

AGClient::AGClient(io_service &iosev):m_iosev(iosev),m_acceptor(iosev)
{
    buf.resize(100);
}

int AGClient::setup(char* addr,unsigned short int serverPort)
{
    serverAddr = tcp::endpoint(ip::address::from_string(addr),serverPort);
    psocket = boost::shared_ptr<tcp::socket>(new tcp::socket(m_iosev));
    boost::system::error_code ec;
    psocket->connect(serverAddr, ec);
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return 1;
    }
    size_t tempLen = psocket->read_some(buffer(buf), ec);
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return 2;
    }
    std::cerr << tempLen << " Byte recieved, port is "<<buf[0] << std::endl;
    serverAddr.port(buf[0]);
    psocket->close();
    psocket->connect(serverAddr, ec);
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return 1;
    }
    return 0;
}

void AGClient::recieve()
{
    std::cout << "Waiting for Data.\n";
    psocket->async_read_some(buffer(buf), boost::bind(&AGClient::readHandler,this,psocket,_1,_2));
}

void AGClient::readHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec,size_t bytesArrived)
{
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    std::cout << bytesArrived << std::endl;
    len = bytesArrived/sizeof(int);
}

void AGClient::send(vector<int> message)
{
    psocket->async_write_some(buffer(message),
                              boost::bind(&AGClient::writeHandler, this, _1, _2));
}

void AGClient::writeHandler(error_code ec,
                            size_t bytes_transferred)
{
    if(ec)
        std::cout<< "发送失败!" << boost::system::system_error(ec).what() << std::endl;
    else
        std::cout<< bytes_transferred << "bytes 已发送" << std::endl;
}
