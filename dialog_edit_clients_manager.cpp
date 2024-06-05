#include "dialog_edit_clients_manager.h"
#include "ui_dialog_edit_clients_manager.h"

DialogEditClientsManager::DialogEditClientsManager(int id_user, bool registration, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogEditClientsManager)
{
   ui->setupUi(this); \
   db   = QSqlDatabase::database();
   user = id_user;
   if (user == -1)
   {
      ui->pushButton_delete->setEnabled(false);
      setWindowTitle("Создать пользователя");
   }
   else
   {
      setWindowTitle("Редактировать пользователя");
      update_view();
      adjustSize();
   }

   ui->ComboBox->setCurrentIndex(2);
   if (registration)
   {
      ui->ComboBox->setEnabled(false);
   }
   ui->LineEdit_phone->setInputMask("+9(999)999-99-99");
}

DialogEditClientsManager::~DialogEditClientsManager()
{
   delete ui;
}

void DialogEditClientsManager::update_view()
{
   QSqlQuery query(db);

   query.prepare("SELECT Users.login, "
                 "CASE IFNULL(Users.LegalName, CAST(Users.perms AS TEXT)) WHEN '0' THEN 'Менеджер' WHEN '1' THEN 'Склад' WHEN '2' THEN 'Физическое лицо' ELSE Users.LegalName END, "
                 "Users.Surname, Users.Name, Users.Otchestvo, Users.phone, Users.Addres, Users.perms "
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
   if (query.value(1).toString() == "Физическое лицо" ||
       query.value(1).toString() == "Склад" ||
       query.value(1).toString() == "Менеджер")
   {
      ui->LineEdit_legalname->setPlaceholderText(query.value(1).toString());
   }
   else
   {
      ui->LineEdit_legalname->setText(query.value(1).toString());
   }
   ui->LineEdit_surname->setText(query.value(2).toString());
   ui->LineEdit_name->setText(query.value(3).toString());
   ui->LineEdit_otchestvo->setText(query.value(4).toString());
   ui->LineEdit_phone->setText(query.value(5).toString());
   ui->LineEdit_address->setText(query.value(6).toString());
   ui->ComboBox->setCurrentIndex(query.value(7).toInt());
   initial_role = query.value(7).toInt();
}

void DialogEditClientsManager::on_pushButton_ok_clicked()
{
   if (user == -1) //We create new user
   {
      if (ui->LineEdit_login->text().isEmpty() ||
          ui->LineEdit_surname->text().isEmpty() ||
          ui->LineEdit_name->text().isEmpty() ||
          ui->LineEdit_phone->text().isEmpty() ||
          ui->LineEdit_address->text().isEmpty() ||
          ui->LineEdit_newpass->text().isEmpty())
      {
         QMessageBox::information(this, "Информация", "Не все поля заполнены.");
         return;
      }

      db.transaction();

      QSqlQuery query(db);
      QString   legalname = ui->LineEdit_legalname->text().isEmpty() ? QString() : ui->LineEdit_legalname->text();
      QString   otchestvo = ui->LineEdit_otchestvo->text().isEmpty() ? QString() : ui->LineEdit_otchestvo->text();

      query.prepare("INSERT INTO Users (login, legalname, surname, name, otchestvo, phone, addres, password, perms) values (?, ?, ?, ?, ?, ?, ?, ?, ?)");
      query.addBindValue(ui->LineEdit_login->text());
      query.addBindValue(legalname);
      query.addBindValue(ui->LineEdit_surname->text());
      query.addBindValue(ui->LineEdit_name->text());
      query.addBindValue(otchestvo);
      query.addBindValue(ui->LineEdit_phone->text());
      query.addBindValue(ui->LineEdit_address->text());
      query.addBindValue(ui->LineEdit_newpass->text());
      query.addBindValue(ui->ComboBox->currentIndex());

      if (!query.exec())
      {
         db.rollback();
         QSqlError err = query.lastError();
         QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
         return;
      }

      db.commit();
      QMessageBox::information(this, "Успех", "Изменения сохранены.");
      accept();
   }
   else //We edit existing
   {
      if (!ui->LineEdit_login->isModified() &&
          !ui->LineEdit_legalname->isModified() &&
          !ui->LineEdit_surname->isModified() &&
          !ui->LineEdit_name->isModified() &&
          !ui->LineEdit_otchestvo->isModified() &&
          !ui->LineEdit_phone->isModified() &&
          !ui->LineEdit_address->isModified() &&
          !ui->LineEdit_newpass->isModified() &&
          ui->ComboBox->currentIndex() == initial_role)
      {
         QMessageBox::information(this, "Информация", "Нет изменений.");
         reject();
         return;
      }

      if (ui->LineEdit_login->text().isEmpty() ||
          ui->LineEdit_surname->text().isEmpty() ||
          ui->LineEdit_name->text().isEmpty() ||
          ui->LineEdit_phone->text().isEmpty() ||
          ui->LineEdit_address->text().isEmpty())
      {
         QMessageBox::information(this, "Информация", "Не все требуемые поля заполнены.");
         return;
      }

      db.transaction();
      QSqlQuery query(db);
      QString   legalname = ui->LineEdit_legalname->text().isEmpty() ? QString() : ui->LineEdit_legalname->text();
      QString   otchestvo = ui->LineEdit_otchestvo->text().isEmpty() ? QString() : ui->LineEdit_otchestvo->text();

      if (ui->LineEdit_newpass->isModified())
      {
         QMessageBox::StandardButton reply = QMessageBox::question(this, "Сохранить", "Вы уверены, что хотите поменять пароль пользователя?",
                                                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

         if (reply == QMessageBox::No)
         {
            ui->LineEdit_newpass->clear();
            ui->LineEdit_newpass->setModified(false);
            return;
         }

         query.prepare("UPDATE Users SET login = ?, LegalName = ?, surname = ?, name = ?, otchestvo = ?, phone = ?, addres = ?, "
                       "password = ?, perms = ? WHERE id = ?");
         query.addBindValue(ui->LineEdit_login->text());
         query.addBindValue(legalname);
         query.addBindValue(ui->LineEdit_surname->text());
         query.addBindValue(ui->LineEdit_name->text());
         query.addBindValue(otchestvo);
         query.addBindValue(ui->LineEdit_phone->text());
         query.addBindValue(ui->LineEdit_address->text());
         query.addBindValue(ui->LineEdit_newpass->text().isEmpty() ? "password" : ui->LineEdit_newpass->text());
         query.addBindValue(ui->ComboBox->currentIndex());
         query.addBindValue(user);
      }
      else
      {
         query.prepare("UPDATE Users SET login = ?, LegalName = ?, surname = ?, name = ?, otchestvo = ?, phone = ?, addres = ?, perms = ? "
                       "WHERE id = ?");
         query.addBindValue(ui->LineEdit_login->text());
         query.addBindValue(legalname);
         query.addBindValue(ui->LineEdit_surname->text());
         query.addBindValue(ui->LineEdit_name->text());
         query.addBindValue(otchestvo);
         query.addBindValue(ui->LineEdit_phone->text());
         query.addBindValue(ui->LineEdit_address->text());
         query.addBindValue(ui->ComboBox->currentIndex());
         query.addBindValue(user);
      }

      if (!query.exec())
      {
         db.rollback();
         QSqlError err = query.lastError();
         QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
         return;
      }

      db.commit();
      QMessageBox::information(this, "Успех", "Изменения сохранены.");
      accept();
   }
}

//Delete user
void DialogEditClientsManager::on_pushButton_delete_clicked()
{
   QMessageBox::StandardButton reply = QMessageBox::question(this, "Удалить", "Вы уверены, что хотите удалить данного пользователя?",
                                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

   if (reply == QMessageBox::No)
   {
      reject();
      return;
   }

   db.transaction();

   QSqlQuery query(db);

   query.prepare("DELETE FROM Users WHERE id = ?");
   query.addBindValue(user);

   if (!query.exec())
   {
      db.rollback();
      QSqlError err = query.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }

   db.commit();
   QMessageBox::information(this, "Успех", "Изменения сохранены.");
   accept();
}
