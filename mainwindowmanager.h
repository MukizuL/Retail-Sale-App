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
   explicit MainWindowManager(QWidget *parent = nullptr);
   ~MainWindowManager();

private slots:
   void on_addButton_manager_goods_clicked();

   void on_editButton_manager_goods_clicked();

   void on_discountButton_manager_goods_clicked();

   void on_editButton_manager_clients_clicked();

private:
   void update_model_users();
   void update_model_goods();
   void update_model_orders();

   Ui::MainWindowManager *ui;
   QSqlDatabase db;
   QSqlQueryModel *users_model;
   QSqlQueryModel *goods_model;
   QSqlQueryModel *orders_model;
};

#endif // MAINWINDOWMANAGER_H
