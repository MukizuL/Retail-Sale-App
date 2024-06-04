#include "dialog_create_order.h"
#include "ui_dialog_create_order.h"

DialogCreateOrder::DialogCreateOrder(QSqlDatabase database, int client, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogCreateOrder)
{
   ui->setupUi(this);
   db        = database;
   id_client = client;
   cart      = QSqlDatabase::database("shopcart");
   if (!cart.isValid())
   {
      cart = QSqlDatabase::addDatabase("QSQLITE", "shopcart");
      cart.setDatabaseName("./cart.db");
   }
   cart.open();
   goods_model = new QSqlTableModel(this, db);
   cart_model  = new QSqlTableModel(this, cart);
   goods_model->setTable("Goods");
   cart_model->setTable("Goods");
   goods_model->select();
   calculate_total();
   ui->tableView_goods->setModel(goods_model);
   ui->tableView_shopcart->setModel(cart_model);
   ui->tableView_goods->setColumnHidden(0, true);
   ui->tableView_shopcart->setColumnHidden(0, true);
   int rowCount = cart_model->rowCount();
   for (int i = 0; i < rowCount; ++i)
   {
      QModelIndex index = cart_model->index(i, 0);
      QVariant    data  = cart_model->data(index);
      added_items.append(data.toInt());
   }
}

DialogCreateOrder::~DialogCreateOrder()
{
   delete ui;
   delete goods_model;
   delete cart_model;
   cart.close();
}

void DialogCreateOrder::on_pushButton_add_clicked()
{
   QVariant           variant;
   QVector <QVariant> data(6);
   double             amount = ui->lineEdit_amount->text().toDouble();

   QList <QModelIndex> selected = ui->tableView_goods->selectionModel()->selectedRows();

   for (auto index : selected)
   {
      for (int i = 0; i < 6; i++)
      {
         variant = goods_model->data(goods_model->index(index.row(), i));
         data[i] = variant;
      }

      if (amount <= 0)
      {
         QMessageBox::warning(this, "Внимание", "Выберите больше товара.");
         return;
      }

      if (amount > data[5].toDouble())
      {
         QMessageBox::warning(this, "Внимание", "Вы выбрали товара больше чем есть");
         return;
      }

      if (added_items.contains(data[0]))
      {
         QMessageBox::warning(this, "Внимание", "Вы выбрали товара уже находящийся в корзине");
         return;
      }

      QSqlQuery query(cart);

      query.prepare("INSERT INTO Goods (id, Name, Price, Amount) values (:id, :Name, :Price, :Amount)");
      query.bindValue(":id", data[0]);
      query.bindValue(":Name", data[1]);

      if (amount >= 10)
      {
         query.bindValue(":Price", data[3]); //bulk
      }
      else
      {
         query.bindValue(":Price", data[2]); //retail
      }

      query.bindValue(":Amount", amount);

      if (!query.exec())
      {
         QSqlError err = query.lastError();
         QMessageBox::critical(this, "Error", err.databaseText() + "\n" + err.driverText());
         return;
      }
      added_items.append(data[0].toInt());
   }
   calculate_total();
}

void DialogCreateOrder::on_pushButton_remove_clicked()
{
   QVariant  variant;
   QSqlQuery query(cart);

   QList <QModelIndex> selected = ui->tableView_shopcart->selectionModel()->selectedRows();

   for (auto index : selected)
   {
      variant = cart_model->data(cart_model->index(index.row(), 0));
      query.prepare("DELETE FROM Goods WHERE id = :id");

      query.bindValue(":id", variant);
      if (!query.exec())
      {
         QSqlError err = query.lastError();
         QMessageBox::critical(this, "Error", err.databaseText() + "\n" + err.driverText());
         return;
      }
      added_items.remove(added_items.indexOf(variant.toInt()));
   }
   calculate_total();
}

void DialogCreateOrder::calculate_total()
{
   cart_model->select();
   double    total    = 0;
   double    discount = 0;
   int       items    = 0; //Unique items count
   QSqlQuery discount_db(db);
   QSqlQuery query(cart);
   query.prepare("SELECT Price, Amount FROM Goods");
   query.exec();
   while (query.next())
   {
      total += query.value(0).toDouble() * query.value(1).toDouble();    //Price * Amount
      items++;
   }
   discount_db.prepare("SELECT value, pretext FROM Discounts");
   discount_db.exec();
   discount_db.next();
   if (discount_db.value(1).toDouble() <= items)
   {
      discount += discount_db.value(0).toDouble();
   }

   discount_db.next();
   if (discount_db.value(1).toDouble() <= total)
   {
      discount += discount_db.value(0).toDouble();
   }

   ui->lineEdit_discount->setText(QString::number(discount));
   total *= 1 - discount / 100;
   ui->lineEdit_total->setText(QString::number(total));
}

//Create order
void DialogCreateOrder::on_pushButton_create_clicked()
{
   if (ui->lineEdit_total->text() == "0")
   {
      QMessageBox::information(this, "Уведомление", "Корзина пуста");
   }
   QSqlQuery insert_query_orders_db(db);
   QSqlQuery insert_query_items_db(db);
   QSqlQuery update_query_goods_db(db);
   QSqlQuery select_query_goods_db(db);
   QSqlQuery select_query_cart(cart);

   db.transaction();

   //Create new Order
   insert_query_orders_db.prepare("INSERT INTO Orders (status, id_client, discount) values (0, :id_client, :discount)");
   insert_query_orders_db.bindValue(":id_client", id_client);
   insert_query_orders_db.bindValue(":discount", ui->lineEdit_discount->text().toDouble());

   if (!insert_query_orders_db.exec())
   {
      QSqlError err = insert_query_orders_db.lastError();
      QMessageBox::critical(this, "Error", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   //Get all items from Cart
   int id_order = insert_query_orders_db.lastInsertId().toInt();
   select_query_cart.prepare("SELECT id, price, amount FROM Goods");

   if (!select_query_cart.exec())
   {
      QSqlError err = select_query_cart.lastError();
      QMessageBox::critical(this, "Error", err.databaseText() + "\n" + err.driverText());
      return;
   }

   QVector <QVariantList> data(4);
   while (select_query_cart.next())
   {
      data[0].append(id_order);                   //id_order
      data[1].append(select_query_cart.value(0)); //id_good
      data[2].append(select_query_cart.value(1)); //price
      data[3].append(select_query_cart.value(2)); //amount
   }

   //Insert all items from Cart to Items
   insert_query_items_db.prepare("INSERT INTO Items (id_order, id_good, price, amount) values (?, ?, ?, ?)");
   insert_query_items_db.addBindValue(data[0]);
   insert_query_items_db.addBindValue(data[1]);
   insert_query_items_db.addBindValue(data[2]);
   insert_query_items_db.addBindValue(data[3]);

   if (!insert_query_items_db.execBatch())
   {
      QSqlError err = select_query_cart.lastError();
      QMessageBox::critical(this, "Error", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   //Update amount values in db
   QString idset = "(";
   for (int i = 0; i < data[1].length(); ++i)
   {
      idset += data[1].at(i).toString();
      if (i < data[1].length() - 1)
      {
         idset += ",";
      }
   }
   idset += ")";
   select_query_goods_db.prepare("SELECT id, amount FROM Goods WHERE id in " + idset + "");

   if (!select_query_goods_db.exec())
   {
      QSqlError err = select_query_cart.lastError();
      QMessageBox::critical(this, "Error", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   //QVector <QPair <int, double> > to_update;
   QVector <QVariantList> to_update(2);
   //while (select_query_goods_db.next())
   //{
   //   double new_val = select_query_goods_db.value(1).toDouble() - data[3].at(data[1].indexOf(select_query_goods_db.value(0))).toDouble();
   //   to_update.append(QPair <int, double>(select_query_goods_db.value(0).toInt(), new_val));
   //}
   while (select_query_goods_db.next())
   {
      double new_val = select_query_goods_db.value(1).toDouble() - data[3].at(data[1].indexOf(select_query_goods_db.value(0))).toDouble();
      to_update[0].append(select_query_goods_db.value(0));
      to_update[1].append(QVariant::fromValue(new_val));
   }


   update_query_goods_db.prepare("UPDATE Goods SET amount = ? WHERE id = ?");

   update_query_goods_db.addBindValue(to_update[1]);
   update_query_goods_db.addBindValue(to_update[0]);

   if (!update_query_goods_db.execBatch())
   {
      QSqlError err = update_query_goods_db.lastError();
      QMessageBox::critical(nullptr, "Error", "Failed to update goods: " + err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }


   //End transaction
   if (!db.commit())
   {
      QMessageBox::critical(this, "Error", "Transaction failed");
      return;
   }
   QMessageBox::information(this, "Успех", "Заказ создан и ждёт оплаты");
   QSqlQuery clean_cart(cart);
   clean_cart.exec("DELETE FROM Goods");
   accept();
}
