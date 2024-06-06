#include "mainwindowwarehouse.h"
#include "ui_mainwindowwarehouse.h"
#include "dialog_profile_edit.h"
#include "dialog_view_orders.h"
#include "dialog_inventory.h"

MainWindowWarehouse::MainWindowWarehouse(int user, QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowWarehouse)
{
   ui->setupUi(this);
   id_user_     = user;
   db           = QSqlDatabase::database();
   goods_model  = new QSqlQueryModel;
   orders_model = new QSqlQueryModel;
   update_model_goods();
   update_model_orders();
   ui->tableView_warehouse_goods->setModel(goods_model);
   ui->tableView_warehouse_orders->setModel(orders_model);
   adjustSize();
   ui->tableView_warehouse_goods->setColumnHidden(0, true);
   adjustSize();
}

MainWindowWarehouse::~MainWindowWarehouse()
{
   delete ui;
   delete goods_model;
   delete orders_model;
}

//View order
void MainWindowWarehouse::on_showButton_warehouse_orders_clicked()
{
   QList <QModelIndex> index = ui->tableView_warehouse_orders->selectionModel()->selectedRows(); //index should contain only 1 item and it will

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
   QDialog *dialog = new DialogViewOrders(data, true, false, this);

   dialog->exec();
   update_model_orders();
   delete dialog;
}

void MainWindowWarehouse::update_model_goods()
{
   QSqlQuery query(db);

   query.prepare("SELECT Goods.id, Goods.Name AS 'Название', Goods.Info AS 'Информация', Goods.Amount AS 'Количество' "
                 "FROM Goods ORDER BY Goods.Name");

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }
   goods_model->setQuery(std::move(query));
}

void MainWindowWarehouse::update_model_orders()
{
   QSqlQuery query(db);

   query.prepare("SELECT Orders.id AS 'Номер заказа', IFNULL(Users.LegalName, 'Физическое лицо') AS 'Клиент', "
                 "CASE CAST(Orders.status AS TEXT)"
                 "WHEN '0' THEN 'Ожидает оплаты'"
                 "WHEN '1' THEN 'Оплачен'"
                 "WHEN '2' THEN 'Собирается'"
                 "WHEN '3' THEN 'Собран'"
                 "WHEN '4' THEN 'Отправлен'"
                 "ELSE 'Неизвестный статус'"
                 "END AS 'Статус',"
                 "CAST(SUM(Items.amount * Items.price) * (1 - Orders.discount / 100) AS TEXT) || ' руб.' AS 'Итог'"
                 "FROM Orders "
                 "JOIN Items ON Items.id_order = Orders.id "
                 "JOIN Users ON Orders.id_client = Users.id "
                 "GROUP BY Orders.id");

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }
   orders_model->setQuery(std::move(query));
}

//Profile edit
void MainWindowWarehouse::on_action_profile_triggered()
{
   QDialog *dialog = new DialogProfileEdit(false, id_user_, this);

   dialog->exec();
   delete dialog;
}

//Password edit
void MainWindowWarehouse::on_action_password_triggered()
{
   QDialog *dialog = new DialogProfileEdit(true, id_user_, this);

   dialog->exec();
   delete dialog;
}

//Update amount
void MainWindowWarehouse::on_inventButton_warehouse_goods_clicked()
{
   QList <QModelIndex> index = ui->tableView_warehouse_goods->selectionModel()->selectedRows(); //index should contain only 1 item and it will

   if (index.empty() || !index[0].isValid())
   {
      QMessageBox::warning(this, "Внимание", "Вы не выбрали товар.");
      return;
   }
   QVariant           temp;
   QVector <QVariant> data;                                                            //id_good, name, amount

   for (int i = 0; i < 2; i++)
   {
      temp = goods_model->data(goods_model->index(index[0].row(), i));
      data.append(temp);
   }
   temp = goods_model->data(goods_model->index(index[0].row(), 3));
   data.append(temp);

   QDialog *dialog = new DialogInventory(data, this);

   dialog->exec();
   update_model_goods();
   delete dialog;
}
