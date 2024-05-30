#include "dialog_discount_manager.h"
#include "ui_dialog_discount_manager.h"

DialogDiscountManager::DialogDiscountManager(QSqlDatabase database, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogDiscountManager)
{
   ui->setupUi(this);
   QSqlQuery query;
   query.exec("SELECT value, pretext FROM Discounts");
   query.next();
   ui->lineEdit_discount_1->setText(query.value(0).toString());
   ui->lineEdit_goods_count->setText(query.value(1).toString());
   query.next();
   ui->lineEdit_discount_2->setText(query.value(0).toString());
   ui->lineEdit_total->setText(query.value(1).toString());
}

DialogDiscountManager::~DialogDiscountManager()
{
   delete ui;
}

void DialogDiscountManager::on_pushButton_ok_clicked()
{
   QSqlQuery query;
   QVector <QVariantList> data(3);

   data[0].append(QVariant::fromValue(ui->lineEdit_discount_1->text()));
   data[0].append(QVariant::fromValue(ui->lineEdit_discount_2->text()));

   data[1].append(QVariant::fromValue(ui->lineEdit_goods_count->text()));
   data[1].append(QVariant::fromValue(ui->lineEdit_total->text()));

   data[2].append(QVariant::fromValue(1));
   data[2].append(QVariant::fromValue(2));

   query.prepare("UPDATE Discounts SET value = ?, pretext = ? WHERE id = ?");
   query.addBindValue(data[0]);
   query.addBindValue(data[1]);
   query.addBindValue(data[2]);

   if (!query.execBatch())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Error", err.databaseText());
      return;
   }
   accept();
}
