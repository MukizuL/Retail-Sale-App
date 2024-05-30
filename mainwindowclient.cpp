#include "mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "dialog_create_order.h"

MainWindowClient::MainWindowClient(int user, QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowClient)
{
   ui->setupUi(this);
   db           = QSqlDatabase::database();
   id_client    = user;
   orders_model = new QSqlQueryModel;
   update_model();
   ui->tableView_client->setModel(orders_model);
}

MainWindowClient::~MainWindowClient()
{
   delete ui;
   delete orders_model;
}

void MainWindowClient::on_create_orderButton_client_clicked()
{
   QDialog *dialog = new DialogCreateOrder(db, id_client, this);

   dialog->exec();
   update_model();
   delete dialog;
}

void MainWindowClient::update_model()
{
   QSqlQuery orders(db);

   orders.prepare("SELECT Orders.id AS 'Номер заказа',	CASE CAST(Orders.status AS TEXT)"
                  "WHEN '0' THEN 'Ожидает оплаты'"
                  "WHEN '1' THEN 'Оплачен'"
                  "WHEN '2' THEN 'Собирается'"
                  "WHEN '3' THEN 'Собран'"
                  "WHEN '4' THEN 'Отправлен'"
                  "ELSE 'Неизвестный статус'"
                  "END AS 'Статус',"
                  "CAST(Orders.discount AS TEXT) || ' %' AS 'Скидка',"
                  "CAST(Orders.total AS TEXT) || ' руб.' AS 'Итог'"
                  "FROM Orders");
   orders.exec();
   orders_model->setQuery(std::move(orders));
}
