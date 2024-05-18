#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class MainWindowClient;
}

class MainWindowClient : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindowClient(QSqlDatabase database, QWidget *parent = nullptr);
   ~MainWindowClient();

private:
   Ui::MainWindowClient *ui;
   QSqlDatabase db;
};

#endif // MAINWINDOWCLIENT_H
