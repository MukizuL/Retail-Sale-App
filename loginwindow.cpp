#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include "mainwindowclient.h"
#include "mainwindowmanager.h"
#include "mainwindowwarehouse.h"
#include "dialog_edit_clients_manager.h"

LoginWindow::LoginWindow(QWidget *parent)
   : QDialog(parent)
   , ui(new Ui::LoginWindow)
{
   ui->setupUi(this);
   db_connect();
}

LoginWindow::~LoginWindow()
{
   delete ui;
   if (main_window == nullptr)
   {
      delete main_window;
   }
   db.close();
}

void LoginWindow::on_signin_clicked()
{
   if (ui->login->text().isEmpty() || ui->password->text().isEmpty())
   {
      QMessageBox::warning(this, "Внимание", "Введите корректные данные.");
      return;
   }

   QString   login       = ui->login->text();
   QString   password    = ui->password->text();
   int       permissions = 0;
   QSqlQuery account(db);

   account.prepare("SELECT perms, Name, id FROM Users WHERE login = :login AND password = :password");
   account.bindValue(":login", login);
   account.bindValue(":password", password);

   if (!account.exec())
   {
      QSqlError err = account.lastError();
      QMessageBox::critical(this, "Ошибка", err.databaseText() + "\n" + err.driverText());
      return;
   }

   if (account.next())
   {
      permissions = account.value(0).toInt();
      QMessageBox::information(this, "Успех", "Добрый день, " + account.value(1).toString());
      switch (permissions)
      {
      case 0:
         main_window = new MainWindowManager(account.value(2).toInt());
         break;

      case 1:
         main_window = new MainWindowWarehouse(account.value(2).toInt());
         break;

      case 2:
         main_window = new MainWindowClient(account.value(2).toInt());
         break;
      }
      if (main_window)
      {
         main_window->show();
         accept();
      }
   }
   else
   {
      QMessageBox::critical(this, "Ошибка", "Введён неправильный логин/пароль!");
   }
}

void LoginWindow::db_connect()
{
   db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("./DB.db");

   if (db.open())
   {
      ui->db_status->setText("Подключен");
   }
   else
   {
      ui->db_status->setText("Отключен");
      QSqlError err = db.lastError();
      QMessageBox::critical(this, "No", err.text());
   }
}

//Registration
void LoginWindow::on_register_2_clicked()
{
   QDialog *dialog = new DialogEditClientsManager(-1, true, this);

   dialog->exec();
   delete dialog;
}
