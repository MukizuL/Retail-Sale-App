#include "mainwindowmanager.h"
#include "ui_mainwindowmanager.h"
#include "dialog_add_goods_manager.h"
#include "dialog_edit_goods_manager.h"

MainWindowManager::MainWindowManager(QSqlDatabase database, QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowManager)
{
   db           = database;
   users_model  = new QSqlTableModel(this, db);
   goods_model  = new QSqlTableModel(this, db);
   orders_model = new QSqlTableModel(this, db);
   users_model->setTable("Users");
   goods_model->setTable("Goods");
   //orders_model ->setTable("Orders");
   users_model->select();
   goods_model->select();
   orders_model->select();
   ui->setupUi(this);
   ui->tableView_manager_clients->setModel(users_model);
   ui->tableView_manager_goods->setModel(goods_model);
   ui->tableView_manager_orders->setModel(orders_model);
   ui->tableView_manager_clients->setColumnHidden(0, true);
   ui->tableView_manager_goods->setColumnHidden(0, true);
   ui->tableView_manager_orders->setColumnHidden(0, true);
}

MainWindowManager::~MainWindowManager()
{
   delete ui;
   delete users_model;
   delete goods_model;
   delete orders_model;
   db.removeDatabase("Qt");
}

void MainWindowManager::on_addButton_manager_goods_clicked()
{
   QDialog *dialog = new dialog_add_goods_manager(db, this);

   dialog->exec();
   goods_model->select();
   delete dialog;
}

void MainWindowManager::on_editButton_manager_goods_clicked()
{
   QVariant           variant;
   QVector <QVariant> data(5);

   for (int i = 0; i < 5; i++)
   {
      variant = goods_model->data(goods_model->index(row, i));
      data[i] = variant;
   }
   QDialog *dialog = new dialog_edit_goods_manager(db, data, this);

   dialog->exec();
   goods_model->select();
   delete dialog;
}

void MainWindowManager::on_tableView_manager_goods_clicked(const QModelIndex&index)
{
   row = index.row();
}
