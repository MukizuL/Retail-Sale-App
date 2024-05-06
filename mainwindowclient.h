#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include <QMainWindow>

namespace Ui {
class MainWindowClient;
}

class MainWindowClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowClient(QWidget *parent = nullptr);
    ~MainWindowClient();

private:
    Ui::MainWindowClient *ui;
};

#endif // MAINWINDOWCLIENT_H
