#include "mainwindow.h"
#include <QApplication>
#include<iostream>
#include"server.h"
#include"player.h"
#include"textgui.h"
#include<QThread>
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

    QApplication a(argc, argv);

    Server* s = new Server;
    textGUI* h = new textGUI(0,s);
    s->init(h);
  //h->tconnect();
    h->show();
    game hehe(s);
    hehe.run();
    return a.exec();
}
