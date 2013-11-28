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

}

void MainWindow::begin()
{
    disconnect(ui->pushButton, SIGNAL(clicked()),this,SLOT(begin()));
    NetworkClient.setup((char*)ui->lineEdit->text().toStdString().c_str());
    ui->lineEdit->clear();

    connect(ui->pushButton, SIGNAL(clicked()),this,SLOT(sendMessage()));
    connect(&NetworkClient,SIGNAL(connected()),this,SLOT(socketConnected()));
    ui->pushButton->setEnabled(true);
}
void MainWindow::socketConnected()
{
    ui->infoLabel->setText(QString("Connected"));
    ui->pushButton->setText(QString("Send"));
    ui->label->setText(QString("Please input message:"));
    ui->pushButton->setEnabled(true);
    connect(&NetworkClient,SIGNAL(readFinished(std::vector<int>)),this,SLOT(displayMessage(std::vector<int>)));
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

MainWindow::~MainWindow()
{
    delete ui;
}
