#include "dialog_view_orders.h"
#include "ui_dialog_view_orders.h"

DialogViewOrders::DialogViewOrders(QVector <QVariant> selected_order, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogViewOrders)
{
   ui->setupUi(this);
   db          = QSqlDatabase::database();
   items_model = new QSqlQueryModel;
   items       = std::move(selected_order);
   update_model();
   ui->tableView->setModel(items_model);
}

DialogViewOrders::~DialogViewOrders()
{
   delete ui;
   delete items_model;
}

void DialogViewOrders::update_model()
{
   QSqlQuery query(db);
   QSqlQuery order(db);

   query.prepare("SELECT Goods.Name AS 'Название', Items.Price * (1 - Orders.discount / 100) AS 'Цена', Items.Amount AS 'Количество', Items.Price * Items.Amount * (1 - Orders.discount / 100) AS 'Итог' "
                 "FROM Orders "
                 "JOIN Items ON Items.id_order = Orders.id "
                 "JOIN Goods ON Goods.id = Items.id_good "
                 "WHERE Orders.id = :id");
   query.bindValue(":id", items[0].toInt());

   order.prepare("SELECT Orders.id, Orders.Date, Orders.Discount, SUM(Items.amount * Items.price) * (1 - Orders.discount / 100) "
                 "FROM Orders "
                 "JOIN Items ON Items.id_order = Orders.id "
                 "WHERE Orders.id = :id");
   order.bindValue(":id", items[0].toInt());

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }

   if (!order.exec())
   {
      QSqlError err = order.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }

   order.next();
   ui->label->setText("Заказ №" + order.value(0).toString() + " Дата: " + order.value(1).toString());
   ui->lineEdit_discount->setText(order.value(2).toString());
   ui->lineEdit_total->setText(order.value(3).toString());

   items_model->setQuery(std::move(query));
}

void DialogViewOrders::on_pushButton_export_clicked()
{
   QString filename = QFileDialog::getSaveFileName(this, "Сохранить", QCoreApplication::applicationDirPath() + "/export/export.txt",
                                                   "Text files (*.txt)");

   if (filename.isEmpty())
   {
      return;
   }
   QFile file_out(filename);

   if (!file_out.open(QIODevice::WriteOnly | QIODevice::Text))
   {
      QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл: " + filename);
   }

   QTextStream out(&file_out);
   QSqlQuery   item(db);
   QSqlQuery   order(db);

   item.prepare("SELECT Goods.Name AS 'Название', Items.Price * (1 - Orders.discount / 100) AS 'Цена', Items.Amount AS 'Количество', Items.Price * Items.Amount * (1 - Orders.discount / 100) AS 'Итог' "
                "FROM Orders "
                "JOIN Items ON Items.id_order = Orders.id "
                "JOIN Goods ON Goods.id = Items.id_good "
                "WHERE Orders.id = :id");
   item.bindValue(":id", items[0].toInt());

   order.prepare("SELECT Users.LegalName, Users.Surname, Users.name, Users.Otchestvo, Date, Orders.Discount, SUM(Items.amount * Items.price) * (1 - Orders.discount / 100) "
                 "FROM Orders "
                 "JOIN Users ON Orders.id_client = Users.id "
                 "JOIN Items ON Items.id_order = Orders.id "
                 "WHERE Orders.id = :id");
   order.bindValue(":id", items[0].toInt());

   if (!order.exec())
   {
      QSqlError err = order.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }

   if (!item.exec())
   {
      QSqlError err = item.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }

   order.next();

   QString company = order.value(0).toString().isEmpty() ? "Физическое лицо" : order.value(0).toString();

   out << "Компания: " << company << Qt::endl << "ФИО: " << order.value(1).toString() << " " << order.value(2).toString() << " " << order.value(3).toString() << Qt::endl;
   out << "Дата: " << order.value(4).toString() << Qt::endl << "Скидка: " << order.value(5).toString() << " %" << Qt::endl
       << "Итог: " << order.value(6).toString() << " .руб" << Qt::endl;

   int row = 1;
   out << "Товары:" << Qt::endl << "   ";
   out.setFieldWidth(20);
   out.setFieldAlignment(QTextStream::AlignLeft);
   out << "Название" << "Цена" << "Количество" << "Сумма";
   out.setFieldWidth(0);
   out << Qt::endl;

   while (item.next())
   {
      out << row << ") ";
      out.setFieldWidth(20);
      out << item.value(0).toString() << item.value(1).toString() << item.value(2).toString() << item.value(3).toString();
      out.setFieldWidth(0);
      out << Qt::endl;
   }
   file_out.close();
}
