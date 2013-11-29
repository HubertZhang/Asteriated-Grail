#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit,SIGNAL(returnPressed()),ui->pushButton,SLOT(click()));
    connect(ui->pushButton, SIGNAL(clicked()),this,SLOT(begin()));
    connect(ui->lineEdit_2,SIGNAL(returnPressed()),ui->pushButton_2,SLOT(click()));
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::begin()
{
    disconnect(ui->pushButton, SIGNAL(clicked()),this,SLOT(begin()));
    NetworkClient.setup((char*)ui->lineEdit->text().toStdString().c_str());
    ui->lineEdit->clear();
    connect(&NetworkClient,SIGNAL(connected()),this,SLOT(socketConnected()));
    connect(&NetworkClient,SIGNAL(idReceived(int)),this,SLOT(chatBegin()));
    connect(ui->pushButton, SIGNAL(clicked()),this,SLOT(sendMessage()));


    ui->pushButton->setEnabled(true);
}
void MainWindow::chatBegin()
{
    chatClient.setup(NetworkClient.id,(char*)NetworkClient.peerAddress().toString().toStdString().c_str());

    ui->pushButton_2->setEnabled(true);
    connect(ui->pushButton_2, SIGNAL(clicked()),this,SLOT(sendChatMessage()));
}

void MainWindow::socketConnected()
{
    ui->infoLabel->setText(QString("Connected"));
    ui->pushButton->setText(QString("Send"));
    ui->label->setText(QString("Please input message:"));
    ui->pushButton->setEnabled(true);
    connect(&NetworkClient,SIGNAL(readFinished(std::vector<int>)),this,SLOT(displayMessage(std::vector<int>)));
    connect(&chatClient,SIGNAL(readFinished(int,QString)),this,SLOT(displayMessage(int,QString)));
}
void MainWindow::displayMessage(int id,QString message)
{
    ui->textBrowser->append(QString::number(id)+QString(": ")+message);
}

void MainWindow::displayMessage(std::vector<int> message)
{
    QString temp;
    for(int i = 0; i<message.size();i++)
    {
        temp+=QString::number(message[i]);
        temp+=QString(" ");
    }
    ui->infoLabel->setText(temp);
}

void MainWindow::sendMessage()
{
    QString temp = ui->lineEdit->text();
    ui->lineEdit->clear();
    QTextStream textout(&temp);
    std::cout << "here";
    vector<int> message;
    while(!textout.atEnd())
    {
        int t;
        textout >> t;
        message.push_back(t);
        std::cout << message.size()<<std::endl;
    }
    NetworkClient.sendMessage(message);
}

void MainWindow::sendChatMessage()
{
    QString temp = ui->lineEdit_2->text();
    ui->lineEdit_2->clear();
    std::cout << "here";
    chatClient.sendMessage(temp);
}


MainWindow::~MainWindow()
{
    delete ui;
}
