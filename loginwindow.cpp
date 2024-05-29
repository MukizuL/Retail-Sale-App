#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include "mainwindowclient.h"
#include "mainwindowmanager.h"
#include "mainwindowwarehouse.h"

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
   //db.removeDatabase("Qt");

   db.removeDatabase("./DB.db");
}

void LoginWindow::on_signin_clicked()
{
   QString   login       = ui->login->text();
   QString   password    = ui->password->text();
   int       permissions = 0;
   QSqlQuery account;

   account.prepare("SELECT password, perms, Name, id FROM Users WHERE login = :login");

   account.bindValue(":login", login);
   account.exec();
   if (account.next())
   {
      if (password == account.value(0).toString())
      {
         permissions = account.value(1).toInt();
         QMessageBox::information(this, "Успех", "Добрый день, " + account.value(2).toString());
         switch (permissions)
         {
         case 0:
            main_window = new MainWindowManager(db);
            break;

         case 1:
            main_window = new MainWindowWarehouse(db);
            break;

         case 2:
            main_window = new MainWindowClient(db, account.value(3).toInt());
            break;
         }
         if (main_window)
         {
            //hide();
            main_window->show();
            close();
         }
      }
      else
      {
         QMessageBox::warning(this, "Ошибка", "Введён неправильный логин/пароль!");
      }
   }
   else
   {
      QMessageBox::warning(this, "Ошибка", "База данных не доступна!");
   }
}

void LoginWindow::on_db_reconnect_clicked()
{
   db.close();
   db.open();
}

void LoginWindow::db_connect()
{
   db = QSqlDatabase::addDatabase("QSQLITE");
   //db = QSqlDatabase::addDatabase("QODBC");
   //db.setHostName("127.0.0.1");
   //db.setPort(1433);
   // db.setUserName("admin");
   //db.setPassword("admin");
   //db.setDatabaseName("Qt");
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
