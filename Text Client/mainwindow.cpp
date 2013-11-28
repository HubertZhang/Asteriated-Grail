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
    ui->label->setText(QString("Send"));
    connect(ui->pushButton, SIGNAL(clicked()),this,SLOT(sendMessage()));
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
