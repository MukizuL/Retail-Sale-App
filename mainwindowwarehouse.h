#ifndef MAINWINDOWWAREHOUSE_H
#define MAINWINDOWWAREHOUSE_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class MainWindowWarehouse;
}

class MainWindowWarehouse : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindowWarehouse(int user, QWidget *parent = nullptr);
   ~MainWindowWarehouse();

private slots:
   void on_showButton_warehouse_orders_clicked();

   void on_action_profile_triggered();

   void on_action_password_triggered();

   void on_inventButton_warehouse_goods_clicked();

private:
   void update_model_goods();
   void update_model_orders();

   int id_user_;
   Ui::MainWindowWarehouse *ui;
   QSqlDatabase db;
   QSqlQueryModel *orders_model;
   QSqlQueryModel *goods_model;
};

#endif // MAINWINDOWWAREHOUSE_H
