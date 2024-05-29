#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class MainWindowClient;
}

class MainWindowClient : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindowClient(QSqlDatabase database, int user, QWidget *parent = nullptr);
   ~MainWindowClient();

private slots:
   void on_create_orderButton_client_clicked();

private:
   Ui::MainWindowClient *ui;
   QSqlDatabase db;
   QSqlTableModel *orders_model;
   int id_client;
};

#endif // MAINWINDOWCLIENT_H
