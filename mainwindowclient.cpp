#include "mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "dialog_create_order.h"
#include "dialog_view_orders.h"
#include "dialog_profile_edit.h"

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

//Create order
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
                  "CAST(SUM(Items.amount * Items.price) * (1 - Orders.discount / 100) AS TEXT) || ' руб.' AS 'Итог'"
                  "FROM Orders "
                  "JOIN Items ON Items.id_order = Orders.id "
                  "GROUP BY Orders.id");

   if (!orders.exec())
   {
      QSqlError err = orders.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }
   orders_model->setQuery(std::move(orders));
}

//View orders
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

//Profile edit
void MainWindowClient::on_action_profile_triggered()
{
   QDialog *dialog = new DialogProfileEdit(false, id_client, this);

   dialog->exec();
   delete dialog;
}

//Password edit
void MainWindowClient::on_action_triggered()
{
   QDialog *dialog = new DialogProfileEdit(true, id_client, this);

   dialog->exec();
   delete dialog;
}

//Cancel order
void MainWindowClient::on_delete_orderButton_client_clicked()
{
   QList <QModelIndex> index = ui->tableView_client->selectionModel()->selectedRows(); //index should contain only 1 item and it will

   if (index.empty())
   {
      QMessageBox::warning(this, "Внимание", "Вы не выбрали заказ.");
      return;
   }

   if (orders_model->data(orders_model->index(index[0].row(), 1)).toInt() == 4)
   {
      QMessageBox::warning(this, "Внимание", "Отправленный заказ нельзя отменить.");
      return;
   }

   QMessageBox::StandardButton ans = QMessageBox::question(this, "Retail Sale App", "Вы уверены, что хотите отменить заказ?",
                                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

   if (ans == QMessageBox::No)
   {
      return;
   }

   db.transaction();

   QVariant id_order = orders_model->data(orders_model->index(index[0].row(), 0));

   QSqlQuery select_items(db);
   QSqlQuery update_goods(db);
   QSqlQuery delete_items(db);
   QSqlQuery delete_orders(db);

   select_items.prepare("SELECT Items.id_good, Items.amount FROM Items WHERE Items.id_order = :id_order");
   select_items.bindValue(":id_order", id_order);

   if (!select_items.exec())
   {
      QSqlError err = select_items.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   QVector <QVariantList> data(2); //id_good, amount

   while (select_items.next())
   {
      data[0].append(select_items.value(0));
      data[1].append(select_items.value(1));
   }

   update_goods.prepare("UPDATE Goods SET amount = amount + ? WHERE id = ?");
   update_goods.addBindValue(data[1]);
   update_goods.addBindValue(data[0]);

   if (!update_goods.execBatch())
   {
      QSqlError err = update_goods.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   delete_items.prepare("DELETE FROM Items WHERE Items.id_order = :id_order");
   delete_items.bindValue(":id_order", id_order);

   if (!delete_items.exec())
   {
      QSqlError err = delete_items.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   delete_orders.prepare("DELETE FROM Orders WHERE Orders.id = :id_order");
   delete_orders.bindValue(":id_order", id_order);

   if (!delete_orders.exec())
   {
      QSqlError err = delete_orders.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   if (!db.commit())
   {
      QMessageBox::critical(this, "Ошибка", "Не удалось удалить заказ.");
      db.rollback();
      return;
   }

   update_model();
}
