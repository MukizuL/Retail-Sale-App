#include "dialog_inventory.h"
#include "ui_dialog_inventory.h"

DialogInventory::DialogInventory(QVector <QVariant> vector, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogInventory)
{
   ui->setupUi(this);
   db    = QSqlDatabase::database();
   data_ = std::move(vector);
   ui->LineEdit_name->setText(data_[1].toString());
   ui->LineEdit_name->setReadOnly(true);
   ui->LineEdit_inventory->setText(data_[2].toString());
   ui->LineEdit_arrived->setText("0");

   validator_int = new QIntValidator(0, 100000, this);
   ui->LineEdit_arrived->setValidator(validator_int);
   ui->LineEdit_inventory->setValidator(validator_int);

   connect(ui->checkBox, &QCheckBox::checkStateChanged, this, &DialogInventory::handleCheckBoxState);
}

DialogInventory::~DialogInventory()
{
   delete ui;
   delete validator_int;
}

void DialogInventory::handleCheckBoxState(Qt::CheckState state)
{
   if (state == Qt::Checked)
   {
      ui->LineEdit_inventory->setReadOnly(true);
      ui->LineEdit_arrived->setReadOnly(false);
   }
   else
   {
      ui->LineEdit_inventory->setReadOnly(false);
      ui->LineEdit_arrived->setReadOnly(true);
   }
}

void DialogInventory::on_pushButton_ok_clicked()
{
   QSqlQuery query(db);

   db.transaction();

   if (ui->LineEdit_arrived->isReadOnly()) //Fix amount in  inventory
   {
      if (!ui->LineEdit_inventory->hasAcceptableInput())
      {
         QMessageBox::warning(this, "Внимание", "Неверное значение в полях с количеством.");
         db.rollback();
         return;
      }

      query.prepare("UPDATE Goods SET amount = ? WHERE id = ?");
      query.addBindValue(ui->LineEdit_inventory->text().toInt());
      query.addBindValue(data_[0]);
   }
   else //Add to inventory
   {
      if (!ui->LineEdit_arrived->hasAcceptableInput())
      {
         QMessageBox::warning(this, "Внимание", "Неверное значение в полях с количеством.");
         db.rollback();
         return;
      }
      query.prepare("UPDATE Goods SET amount = amount + ? WHERE id = ?");
      query.addBindValue(ui->LineEdit_arrived->text().toInt());
      query.addBindValue(data_[0]);
   }

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   if (!db.commit())
   {
      QMessageBox::critical(this, "Ошибка", "Не удалось изменить количество товара.");
      db.rollback();
      return;
   }
   accept();
}
