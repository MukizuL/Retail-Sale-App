#include "mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "dialog_create_order.h"

MainWindowClient::MainWindowClient(QSqlDatabase database, int user, QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowClient)
{
   ui->setupUi(this);
   db           = database;
   id_client    = user;
   orders_model = new QSqlTableModel(this, db);
   orders_model->setTable("Orders");
   orders_model->select();
   ui->tableView_client->setModel(orders_model);
   ui->tableView_client->hideColumn(2);
   ui->tableView_client->hideColumn(4);
}

MainWindowClient::~MainWindowClient()
{
   delete ui;
   delete orders_model;
   db.removeDatabase("Qt");
}

void MainWindowClient::on_create_orderButton_client_clicked()
{
   QDialog *dialog = new DialogCreateOrder(db, id_client, this);

   dialog->exec();
   orders_model->select();
   delete dialog;
}
