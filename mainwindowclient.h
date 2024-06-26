#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
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

   void on_pushButton_show_clicked();

   void on_delete_orderButton_client_clicked();

   void on_action_profile_triggered();

   void on_action_password_triggered();

private:
   void update_model();

   Ui::MainWindowClient *ui;
   QSqlDatabase db;
   QSqlQueryModel *orders_model;
   int id_client;
};

#endif // MAINWINDOWCLIENT_H
