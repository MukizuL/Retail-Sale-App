#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>

namespace Ui {
class MainWindowClient;
}

class MainWindowClient : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindowClient(int user, QWidget *parent = nullptr);
   ~MainWindowClient();

private slots:
   void on_create_orderButton_client_clicked();

private:
   void update_model();

   Ui::MainWindowClient *ui;
   QSqlDatabase db;
   QSqlQueryModel *orders_model;
   int id_client;
};

#endif // MAINWINDOWCLIENT_H
