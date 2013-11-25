//
//  AGServer.cpp
//  Network
//
//  Created by Hubert on 13-11-20.
//  Copyright (c) 2013年 Hubert. All rights reserved.
//

#include "AGServer.h"

AGServer::AGServer(io_service &iosev):m_iosev(iosev),m_acceptor(iosev,tcp::endpoint(tcp::v4(),serverPort),true)
{
    m_pacceptor = new tcp::acceptor*[6];
    for (int i = 0; i<6; i++) {
        m_pacceptor[i] = new tcp::acceptor(iosev);
        buf[i].resize(100);
    }
}

void AGServer::setup(int* listeningPort)
{
    psocket = boost::shared_ptr<tcp::socket>(new tcp::socket(m_iosev));
    boost::system::error_code ec;
    for (int i = 0; i<1; i++) {
        m_acceptor.accept(*psocket);
        clientAddr[i] = psocket->remote_endpoint();
        std::cout <<"New IP detected:" << clientAddr[i].address() << " " << clientAddr[i].port() << std::endl;
        int temp[1] ={listeningPort[i]};
        psocket->write_some(buffer(temp));
        boost::asio::ip::tcp::endpoint tempEP(tcp::v4(),listeningPort[i]);
        psocket->close();
        csocket[i] = boost::shared_ptr<tcp::socket>(new tcp::socket(m_iosev));
        m_pacceptor[i]->open(tempEP.protocol());
        m_pacceptor[i]->bind(tempEP);
        m_pacceptor[i]->listen(-1);
        m_pacceptor[i]->accept(*csocket[i]);
        std::cout<<"finished"<<std::endl;
    }
}

void AGServer::recieve(int i)
{
    std::cout << "Waiting for Data.\n";
    csocket[i]->async_read_some(buffer(buf[i]), boost::bind(&AGServer::readHandler,this,csocket[i],_1,_2));
}

void AGServer::readHandler(boost::shared_ptr<tcp::socket> psocket, boost::system::error_code ec,size_t bytesArrived)
{
    if (ec) {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    std::cout << bytesArrived << " Bytes received." << std::endl;
    emit receiveReady();
}

void AGServer::send(vector<int> message,int i)
{
    csocket[i]->async_write_some(buffer(message),
                              boost::bind(&AGServer::writeHandler, this, _1, _2));
}

void AGServer::writeHandler(error_code ec,
                            size_t bytes_transferred)
{
    if(ec)
        std::cout<< "发送失败!" << boost::system::system_error(ec).what() << std::endl;
    else
        std::cout<< bytes_transferred << "bytes 已发送" << std::endl;
}
