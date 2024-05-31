#include "mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "dialog_create_order.h"
#include "dialog_view_orders.h"

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
   if (!orders.exec())
   {
      QSqlError err = orders.lastError();
      QMessageBox::critical(this, "Error", err.databaseText() + "\n" + err.driverText());
      return;
   }
   orders_model->setQuery(std::move(orders));
}

void MainWindowClient::on_pushButton_show_clicked()
{
   QList <QModelIndex> index = ui->tableView_client->selectionModel()->selectedRows(); //index should contain only 1 item and it will

   if (index.empty() || !index[0].isValid())
   {
      QMessageBox::warning(this, "Внимание", "Вы не выбрали заказ.");
      return;
   }
   QVariant           temp;
   QVector <QVariant> data;                                                            //id_order, date, discount, total

   for (int i = 0; i < 4; i++)
   {
      temp = orders_model->data(orders_model->index(index[0].row(), i));
      data.append(temp);
   }
   QDialog *dialog = new DialogViewOrders(data, this);

   dialog->exec();
   update_model();
   delete dialog;
}
