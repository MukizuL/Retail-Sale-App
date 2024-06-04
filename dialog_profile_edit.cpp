#include "dialog_profile_edit.h"
#include "ui_dialog_profile_edit.h"

DialogProfileEdit::DialogProfileEdit(bool password, int id_client, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogProfileEdit)
{
   ui->setupUi(this);
   passwd = password;
   user   = id_client;
   db     = QSqlDatabase::database();
   if (password)
   {
      ui->profileWidget->hide();
      setWindowTitle("Смена пароля");
   }
   else
   {
      ui->passwordW->hide();
      update_view();
   }
   adjustSize();
}

DialogProfileEdit::~DialogProfileEdit()
{
   delete ui;
}

//New profile data
void DialogProfileEdit::on_pushButton_ok_clicked()
{
   if (ui->LineEdit_login->isModified() ||
       ui->LineEdit_legalname->isModified() ||
       ui->LineEdit_surname->isModified() ||
       ui->LineEdit_name->isModified() ||
       ui->LineEdit_otchestvo->isModified() ||
       ui->LineEdit_phone->isModified() ||
       ui->LineEdit_address->isModified())
   {
      QSqlQuery query(db);

      query.prepare("UPDATE Users SET login = ?, LegalName = ?, Surname = ?, Name = ?, Otchestvo = ?, phone = ?, Addres = ? WHERE id = ?");

      query.addBindValue(ui->LineEdit_login->text());
      query.addBindValue(ui->LineEdit_legalname->text());
      query.addBindValue(ui->LineEdit_surname->text());
      query.addBindValue(ui->LineEdit_name->text());
      query.addBindValue(ui->LineEdit_otchestvo->text());
      query.addBindValue(ui->LineEdit_phone->text());
      query.addBindValue(ui->LineEdit_address->text());
      query.addBindValue(user);

      if (!query.exec())
      {
         QSqlError err = query.lastError();
         QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
         return;
      }
      QMessageBox::information(this, "Успех", "Данные сохранены");
      accept();
   }
   accept();
}

void DialogProfileEdit::update_view()
{
   QSqlQuery query(db);

   query.prepare("SELECT Users.login, IFNULL(Users.LegalName, 'Физическое лицо'), "
                 "Users.Surname, Users.Name, Users.Otchestvo, Users.phone, Users.Addres "
                 "FROM Users WHERE Users.id = :id");
   query.bindValue(":id", user);

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }

   query.next();

   ui->LineEdit_login->setText(query.value(0).toString());
   ui->LineEdit_legalname->setText(query.value(1).toString());
   ui->LineEdit_surname->setText(query.value(2).toString());
   ui->LineEdit_name->setText(query.value(3).toString());
   ui->LineEdit_otchestvo->setText(query.value(4).toString());
   ui->LineEdit_phone->setText(query.value(5).toString());
   ui->LineEdit_address->setText(query.value(6).toString());
}

//New password
void DialogProfileEdit::on_pushButton_ok_2_clicked()
{
   db.transaction();
   QSqlQuery query(db);

   query.prepare("SELECT * FROM Users WHERE id = ? AND password = ?");
   query.addBindValue(user);
   query.addBindValue(ui->LineEdit_oldpass->text());

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   if (!query.next())
   {
      db.rollback();
      QMessageBox::warning(this, "Внимание", "Вы ввели неправильный старый пароль.");
      return;
   }

   if (ui->LineEdit_newpass->text().isEmpty())
   {
      db.rollback();
      QMessageBox::warning(this, "Внимание", "Новый пароль не должен быть пустым.");
      return;
   }

   if (!(ui->LineEdit_newpass->text() == ui->LineEdit_repeatpass->text()))
   {
      db.rollback();
      QMessageBox::warning(this, "Внимание", "Пароли не совпадают.");
      return;
   }

   query.prepare("UPDATE Users SET password = ? WHERE id = ?");
   query.addBindValue(ui->LineEdit_newpass->text());
   query.addBindValue(user);

   if (!query.exec())
   {
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      db.rollback();
      return;
   }

   db.commit();
   QMessageBox::information(this, "Успех", "Пароль изменён.");
   accept();
}
