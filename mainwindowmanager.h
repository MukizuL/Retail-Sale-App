#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>

namespace Ui {
class MainWindowManager;
}

class MainWindowManager : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindowManager(int user, QWidget *parent = nullptr);
   ~MainWindowManager();

private slots:
   void on_addButton_manager_goods_clicked();

   void on_editButton_manager_goods_clicked();

   void on_discountButton_manager_goods_clicked();

   void on_editButton_manager_clients_clicked();

   void on_addButton_manager_clients_clicked();

   void on_showButton_manager_orders_clicked();

   void on_action_profile_triggered();

   void on_action_password_triggered();

private:
   void update_model_users();
   void update_model_goods();
   void update_model_orders();

   int id_user_;
   Ui::MainWindowManager *ui;
   QSqlDatabase db;
   QSqlQueryModel *users_model;
   QSqlQueryModel *goods_model;
   QSqlQueryModel *orders_model;
};

#endif // MAINWINDOWMANAGER_H
