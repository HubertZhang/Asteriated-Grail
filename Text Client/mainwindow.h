#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AGClient.h"
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

public slots:
    void begin();
    void sendMessage();
    void socketConnected();
    void displayMessage(std::vector<int> message);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
