#include "dialog_edit_goods_manager.h"
#include "ui_dialog_edit_goods_manager.h"

DialogEditGoodsManager::DialogEditGoodsManager(QVector <QVariant> data, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogEditGoodsManager)
{
   db        = QSqlDatabase::database();
   item_data = data;
   ui->setupUi(this);
   ui->lineEdit_name->setText(item_data[1].toString());
   ui->lineEdit_retail->setText(item_data[2].toString());
   ui->lineEdit_bulk->setText(item_data[3].toString());
   ui->plainTextEdit_info->setPlainText(item_data[4].toString());
}

DialogEditGoodsManager::~DialogEditGoodsManager()
{
   delete ui;
}

/*! This function is triggered when the delete button is clicked in the edit goods dialog.
 * It presents a confirmation dialog to the user, asking if they are sure about deleting the selected item.
 * If the user confirms the deletion (clicks "Yes"), the function proceeds to execute a SQL query
 * to delete the item from the "Goods" table in the database, using the item's ID.
 * After executing the query, it closes the dialog.
 */
void DialogEditGoodsManager::on_pushButton_delete_clicked()
{
   QMessageBox::StandardButton reply = QMessageBox::question(this, "Retail Sale App", "Вы уверены, что хотите удалить данный товар?",
                                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

   if (reply == QMessageBox::Yes)
   {
      QSqlQuery query;
      query.prepare("DELETE FROM Goods WHERE id = :id");
      query.bindValue(":id", item_data[0]);
      query.exec();
      accept();
   }
}

/*! This function is triggered when the OK button is clicked in the edit goods dialog.
 * It collects the data entered by the user in the various input fields (name, retail cost, bulk cost, and info).
 * If any of these fields are empty, it assigns an empty QString to the respective variables.
 * It then prepares an SQL query to update the corresponding record in the "Goods" table in the database using the item's ID.
 * The query binds the values from the input fields to the corresponding placeholders in the SQL query.
 * If the query execution fails, it displays an error message to the user using a message box.
 * If the query succeeds, it closes the dialog.
 */
void DialogEditGoodsManager::on_pushButton_ok_clicked()
{
   QSqlQuery query;
   QString   name   = ui->lineEdit_name->text().isEmpty() ? QString() : ui->lineEdit_name->text();
   QString   retail = ui->lineEdit_retail->text().isEmpty() ? QString() : ui->lineEdit_retail->text();
   QString   bulk   = ui->lineEdit_bulk->text().isEmpty() ? QString() : ui->lineEdit_bulk->text();
   QString   info   = ui->plainTextEdit_info->toPlainText().isEmpty() ? QString() : ui->plainTextEdit_info->toPlainText();

   query.prepare("UPDATE Goods SET Name = :Name, Retail_cost = :Retail_cost, Bulk_cost = :Bulk_cost, Info = :Info WHERE id = :id");
   query.bindValue(":Name", name);
   query.bindValue(":Retail_cost", retail);
   query.bindValue(":Bulk_cost", bulk);
   query.bindValue(":Info", info);
   query.bindValue(":id", item_data[0]);

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Error", err.databaseText());
      return;
   }
   accept();
}
