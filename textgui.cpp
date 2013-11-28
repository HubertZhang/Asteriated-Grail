#include "textgui.h"
#include"player.h"
#include"server.h"
#include"team.h"
#include<QString>
//Server s;
textGUI::textGUI(QWidget *parent,Server *a) :
    QWidget(parent),server(a)
{
    pushbutton = new QPushButton("确定",this);
   // begin = new QPushButton("begin",this);
    textbrowser = new QTextBrowser(this);
    lineEdit= new QLineEdit(this);
    pushbutton->move(810,615);
    pushbutton->resize(50,30);
    textbrowser->move(510,130);
    textbrowser->resize(350,450);
    lineEdit->move(510,615);
    lineEdit->resize(300,30);
   // begin->resize(50,30);
   // begin->move(810,650);
//-----------队伍-------------------------
    for (int i=0; i<2; i++)
    {
        QString s;
        s.sprintf("team %d",i);
        team[i][0] = new QLabel(s,this);
        team[i][0] ->resize(80,30);
        team[i][1] = new QLabel("星杯",this);
        team[i][1] ->resize(40,30);
        team[i][2] = new QLabel("士气",this);
        team[i][2] ->resize(40,30);
        team[i][3] = new QLabel("宝石",this);
        team[i][3] ->resize(40,30);
        team[i][4] = new QLabel("水晶",this);
        team[i][4] ->resize(40,30);
    }

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<4; j++)
        {
            teamstatus[i][j] = new QTextBrowser(this);
            //teamstatus[i][j]->setText("1");
            teamstatus[i][j] ->resize(40,25);
            teamstatus[i][j] ->move(100+900*i+90*j,40);
        }
    }

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<5; j++)
        {
          team[i][j] ->move(10+900*i+90*j,10);
        }
    }
//-----------------------------------------------------------------------

//-------------创建角色-----------------------------------------------------
    for (int i=0; i<6; i++)
    {
        playercure[i] = new QTextBrowser(this);
        playercure[i]->setText("0");
        playercure[i]->resize(40,25);

        playergem[i] = new QTextBrowser(this);
        playergem[i]->setText("0");
        playergem[i]->resize(40,25);

        playercrystal[i] = new QTextBrowser(this);
        playercrystal[i]->setText("0");
        playercrystal[i]->resize(40,25);

        playerteam[i] = new QTextBrowser(this);
        playerteam[i]->resize(40,25);

        QString s;
        s.sprintf("玩家%d",i);
        player[i][0] = new QLabel(s,this);
        player[i][0]-> resize(90,30);
        player[i][1] = new QLabel("队伍",this);
        player[i][1]-> resize(40,30);
        player[i][2] = new QLabel("宝石",this);
        player[i][2]-> resize(40,30);
        player[i][3] = new QLabel("水晶",this);
        player[i][3]-> resize(40,30);
        player[i][4] = new QLabel("治疗",this);
        player[i][4]-> resize(40,30);

        character[i] = new QLabel(this);
        character[i]->resize(40,30);

        card[i] = new QLabel("卡牌",this);
        card[i]->resize(40,40);

        status[i] = new QLabel("status",this);
        status[i]->resize(40,40);
        for (int j=0; j<6; j++)
        {
            playercard[i][j] = new QTextBrowser(this);
            //playercard[i][j]->setText("thunder100");
            playercard[i][j]->resize(75,40);
            playercard[i][j+6] = new QTextBrowser(this);
            //playercard[i][j]->setText("thunder100");
            playercard[i][j+6]->resize(75,40);

            playerstatus[i][j] = new QTextBrowser(this);
            //playerstatus[i][j]->setText("poison1");
            playerstatus[i][j]->resize(75,40);
        }
    }

    for (int i=0; i<3; i++)
    {

        playergem[i]->move(200,120+200*i);
        playercrystal[i]->move(280,120+200*i);
        playercure[i]->move(360,120+200*i);
        player[i][0]-> move(10,120+200*i);
        player[i][1]-> move(90,120+200*i);
        player[i][2]-> move(170,120+200*i);
        player[i][3]->move(250,120+200*i);
        player[i][4]->move(330,120+200*i);
        character[i]->move(50,120+200*i);
        card[i]->move(10,190+200*i);
        status[i]->move(10,150+200*i);
        playerteam[i]->move(120,120+200*i);
        for (int j=0; j<6; j++)
        {
            playercard[i][j]->move(50+75*j,190+200*i);
            playerstatus[i][j]->move(50+75*j,150+200*i);
            playercard[i][j+6]->move(50+75*j,230+200*i);
        }
    }

    for (int i=3; i<6; i++)
    {
        playergem[i]->move(1060,120+200*(i-3));
        playercrystal[i]->move(1140,120+200*(i-3));
        playercure[i]->move(1220,120+200*(i-3));
        player[i][0]-> move(870,120+200*(i-3));
        player[i][1]->move(950,120+200*(i-3));
        player[i][2]-> move(1030,120+200*(i-3));
        player[i][3]->move(1110,120+200*(i-3));
        player[i][4]->move(1190,120+200*(i-3));
        character[i]->move(910,120+200*(i-3));
        card[i]->move(870,190+200*(i-3));
        status[i]->move(870,150+200*(i-3));
        playerteam[i]->move(980,120+200*(i-3));
        for (int j=0; j<6; j++)
        {
            playercard[i][j]->move(910+75*j,190+200*(i-3));
            playercard[i][j+6]->move(910+75*j,230+200*(i-3));
            playerstatus[i][j]->move(910+75*j,150+200*(i-3));
        }
    }
//------------------------------------------------------------------------------
    //connect(pushbutton,SIGNAL(clicked()),server->team[0],SLOT(BroadCast()));
    //connect(pushbutton,SIGNAL(clicked()),server,SLOT(BroadCast()));
    //this->show();
}

/*
~textGUI::textGUI()
{
    delete []playergem;
    delete []playercrystal;
    delete []playerteam;

    for (int i=0;i<6;i++)
    {
        for (int j=0; j<4; j++)
        {
            delete []player[i];
        }
    }
    delete []player;

    delete []card;
    delete []status;
    for (int i=0;i<6;i++)
    {
        for (int j=0; j<6; j++)
        {
            delete []playerstatus[i];
            delete []playercard[i];
        }
    }
}
*/
void textGUI::tconnect()
{
<<<<<<< HEAD
//    for (int i=0; i<6; i++)
//    {
//       connect(pushbutton,SIGNAL(clicked()),server->players[i],SLOT(getMessage()));
//    }
=======
    for (int i=0; i<6; i++)
    {
       connect(pushbutton,SIGNAL(clicked()),server->players[i],SLOT(getMessage()));
    }

>>>>>>> hqs
    //connect(begin,SIGNAL(clicked()),server,SLOT(Game()));
}

/*
void textGUI::output()
{
    QString s =  lineEdit->text();
    textbrowser->append(s);
}
*/
