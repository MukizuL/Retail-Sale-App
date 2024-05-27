#include "dialog_add_goods_manager.h"
#include "ui_dialog_add_goods_manager.h"

dialog_add_goods_manager::dialog_add_goods_manager(QSqlDatabase database, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::dialog_add_goods_manager)
{
   db = database;
   ui->setupUi(this);
}

dialog_add_goods_manager::~dialog_add_goods_manager()
{
   delete ui;
}

void dialog_add_goods_manager::on_buttonBox_accepted()
{
   QSqlQuery query;
   QString   name   = ui->lineEdit_name->text().isEmpty() ? QString() : ui->lineEdit_name->text();
   QString   retail = ui->lineEdit_retail->text().isEmpty() ? QString() : ui->lineEdit_retail->text();
   QString   bulk   = ui->lineEdit_bulk->text().isEmpty() ? QString() : ui->lineEdit_bulk->text();
   QString   info   = ui->plainTextEdit->toPlainText().isEmpty() ? QString() : ui->plainTextEdit->toPlainText();

   query.prepare("INSERT INTO Goods (Name, Retail_cost, Bulk_cost, Info, Amount) values (:Name, :Retail_cost, :Bulk_cost, :Info, 0)");
   query.bindValue(":Name", name);
   query.bindValue(":Retail_cost", retail);
   query.bindValue(":Bulk_cost", bulk);
   query.bindValue(":Info", info);
   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Error", err.databaseText());
      return;
   }
   accept();
}
