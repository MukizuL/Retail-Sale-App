#include "dialog_add_goods_manager.h"
#include "ui_dialog_add_goods_manager.h"

DialogAddGoodsManager::DialogAddGoodsManager(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogAddGoodsManager)
{
   ui->setupUi(this);
   db = QSqlDatabase::database();
   validator_decimalUS = new QDoubleValidator(this);
   validator_decimalUS->setBottom(0.0);
   validator_decimalUS->setDecimals(2);
   validator_decimalUS->setLocale(QLocale("en_US"));

   ui->lineEdit_bulk->setValidator(validator_decimalUS);
   ui->lineEdit_retail->setValidator(validator_decimalUS);
}

DialogAddGoodsManager::~DialogAddGoodsManager()
{
   delete ui;
}

void DialogAddGoodsManager::on_buttonBox_accepted()
{
   if (!(ui->lineEdit_bulk->hasAcceptableInput() &&
         ui->lineEdit_retail->hasAcceptableInput()))
   {
      QMessageBox::warning(this, "Внимание", "Цены могут быть дробными с 2-я знаками после точки.");
      return;
   }

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
