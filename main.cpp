#include "mainwindow.h"
#include <QApplication>
#include<iostream>
#include"server.h"
#include"player.h"
#include"textgui.h"
#include<QThread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;
using std::vector;
using namespace std;

class game:public QThread
{
    Server *s;
  public:
    game(Server *a):s(a){}
    void run(){s->Game();}
};


int main(int argc, char *argv[])
{
    io_service iosev;
    QApplication a(argc, argv);
   //MainWindow w;
    int ports[6] = {13579,10001,10002,10003,10004,10005};
    Server* s = new Server;
    s->NetworkServer = new AGServer(iosev);
    s->NetworkServer->setup(ports);
    textGUI* h = new textGUI(0,s);
    s->init(h);
    h->tconnect();
    h->show();
    game hehe(s);
    hehe.run();
    // h.pp = new Player(/*&h,*/1,1,1);
   // w.show();
    return a.exec();
    //s->Game();

}
