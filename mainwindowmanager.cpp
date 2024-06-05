#include "mainwindowmanager.h"
#include "ui_mainwindowmanager.h"
#include "dialog_add_goods_manager.h"
#include "dialog_edit_goods_manager.h"
#include "dialog_discount_manager.h"
#include "dialog_edit_clients_manager.h"

MainWindowManager::MainWindowManager(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowManager)
{
   ui->setupUi(this);
   db           = QSqlDatabase::database();
   users_model  = new QSqlQueryModel;
   goods_model  = new QSqlQueryModel;
   orders_model = new QSqlQueryModel;
   update_model_users();
   update_model_goods();
   update_model_orders();
   ui->tableView_manager_clients->setModel(users_model);
   ui->tableView_manager_goods->setModel(goods_model);
   ui->tableView_manager_orders->setModel(orders_model);
   ui->tableView_manager_clients->setColumnHidden(0, true);
   ui->tableView_manager_goods->setColumnHidden(0, true);
   ui->tableView_manager_orders->setColumnHidden(0, true);
   adjustSize();
}

MainWindowManager::~MainWindowManager()
{
   delete ui;
   delete users_model;
   delete goods_model;
   delete orders_model;
}

void MainWindowManager::on_addButton_manager_goods_clicked()
{
   QDialog *dialog = new DialogAddGoodsManager(db, this);

   dialog->exec();
   update_model_goods();
   delete dialog;
}

void MainWindowManager::on_editButton_manager_goods_clicked()
{
   QList <QModelIndex> index = ui->tableView_manager_goods->selectionModel()->selectedRows();

   if (index.empty() || !index[0].isValid())
   {
      QMessageBox::warning(this, "Внимание", "Вы не выбрали товар.");
      return;
   }

   QVariant           variant;
   QVector <QVariant> data(5);

   for (int i = 0; i < 5; i++)
   {
      variant = goods_model->data(goods_model->index(index[0].row(), i));
      data[i] = variant;
   }

   QDialog *dialog = new DialogEditGoodsManager(data, this);

   dialog->exec();
   update_model_goods();
   delete dialog;
}

void MainWindowManager::on_discountButton_manager_goods_clicked()
{
   QDialog *dialog = new DialogDiscountManager(this);

   dialog->exec();
   delete dialog;
}

void MainWindowManager::on_editButton_manager_clients_clicked()
{
   QList <QModelIndex> index = ui->tableView_manager_clients->selectionModel()->selectedRows();

   if (index.empty() || !index[0].isValid())
   {
      QMessageBox::warning(this, "Внимание", "Вы не выбрали клиента.");
      return;
   }

   QDialog *dialog = new DialogEditClientsManager(users_model->data(users_model->index(index[0].row(), 0)).toInt(), this);

   dialog->exec();
   update_model_users();
   delete dialog;
}

void MainWindowManager::update_model_users()
{
   QSqlQuery query(db);

   query.prepare("SELECT Users.id, CASE IFNULL(Users.LegalName, CAST(Users.perms AS TEXT)) WHEN '0' THEN 'Менеджер' WHEN '1' THEN 'Склад' WHEN '2' THEN 'Физическое лицо' ELSE Users.LegalName END AS 'Клиент', Users.Surname AS 'Фамилия', Users.Name AS 'Имя', "
                 "CASE CAST(Users.Perms AS TEXT) "
                 "WHEN '0' THEN 'Менеджер' "
                 "WHEN '1' THEN 'Склад' "
                 "WHEN '2' THEN 'Покупатель' "
                 "ELSE 'Нет такой роли' END AS 'Роли' "
                 "FROM Users");

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }
   users_model->setQuery(std::move(query));
}

void MainWindowManager::update_model_goods()
{
   QSqlQuery query(db);

   query.prepare("SELECT Goods.id, Goods.Name AS 'Название', Goods.Retail_cost AS 'Розничная цена', "
                 "Goods.Bulk_cost AS 'Оптовая цена', Goods.Info AS 'Информация', Goods.Amount AS 'Количество' "
                 "FROM Goods ORDER BY Goods.Name");

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }
   goods_model->setQuery(std::move(query));
}

void MainWindowManager::update_model_orders()
{
   QSqlQuery query(db);

   query.prepare("SELECT Orders.id, Orders.id AS 'Номер заказа', IFNULL(Users.LegalName, 'Физическое лицо') AS 'Клиент', "
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

//Add user
void MainWindowManager::on_addButton_manager_clients_clicked()
{
   QDialog *dialog = new DialogEditClientsManager(-1, this);

   dialog->exec();
   update_model_users();
   delete dialog;
}
