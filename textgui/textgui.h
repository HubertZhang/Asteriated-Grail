#ifndef TEXTGUI_H
#define TEXTGUI_H

#include <QWidget>
#include <QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QTextBrowser>
#include<QString>
class QLabel;
class QLineEdit;
class QTextBrowser;
class QPushButton;
class Server;
class textGUI : public QWidget
{
    Q_OBJECT

public:

    QPushButton *pushbutton;
    QPushButton *begin;
    QTextBrowser *textbrowser;
    QLineEdit *lineEdit;
    Server* server;
    QTextBrowser *playercard[6][12];
    QTextBrowser *playerstatus[6][6];
    QLabel *card[6];
    QLabel *status[6];
    QLabel *player[6][5];
    QLabel *team[2][5];
    QLabel *character[6];
    //QLabel *cure[6];
    QTextBrowser *playercure[6];
    QTextBrowser *playerteam[6];
    QTextBrowser *teamstatus[2][4];
    QTextBrowser *playergem[6];
    QTextBrowser *playercrystal[6];
    explicit textGUI(QWidget *parent = 0,Server* s = 0);
    void tconnect();
signals:
    //void output(QString &a);

public slots:
    //void output();
};

#endif // TEXTGUI_H
