#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AGClient.h"
#include "agchatclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    AGClient NetworkClient;
    AGChatClient chatClient;
public slots:
    void begin();
    void sendMessage();
    void sendChatMessage();
    void socketConnected();
    void chatBegin();
    void displayMessage(std::vector<int> message);
    void displayMessage(int id,QString message);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
