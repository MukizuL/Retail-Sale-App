#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include <QMessageBox>

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
   if (main_window_warehouse)
   {
      delete main_window_warehouse;
   }
   if (main_window_client)
   {
      delete main_window_client;
   }
   if (main_window_manager)
   {
      delete main_window_manager;
   }
   db.removeDatabase("Qt");
}

void LoginWindow::on_signin_clicked()
{
   QString login       = ui->login->text();
   QString password    = ui->password->text();
   int     permissions = 0;

   if (login == "admin" && password == "admin")
   {
      QMessageBox::information(this, "Успех", "Добрый день, " + login);
      switch (permissions)
      {
      case 0:
         hide();
         main_window_manager = new MainWindowManager(this);
         main_window_manager->show();
         break;

      case 1:
         hide();
         main_window_warehouse = new MainWindowWarehouse(this);
         main_window_warehouse->show();
         break;

      case 2:
         hide();
         main_window_client = new MainWindowClient(this);
         main_window_client->show();
         break;
      }
   }
   else
   {
      QMessageBox::warning(this, "Ошибка", "Введён неправильный логин/пароль!");
   }
}

void LoginWindow::on_invokesql_clicked()
{
   QSqlQuery query;

   query.exec("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE='BASE TABLE'");

   if (query.isActive())
   {
      qDebug() << "Tables in the database:";
      while (query.next())
      {
         QString tableName = query.value(0).toString();
         qDebug() << tableName;
      }
   }
   else
   {
      qDebug() << "Failed to retrieve tables:" << query.lastError().text();
   }
}

void LoginWindow::on_db_reconnect_clicked()
{
   db.close();
   db.open();
}

void LoginWindow::db_connect()
{
   db = QSqlDatabase::addDatabase("QODBC");
   db.setHostName("127.0.0.1");
   db.setPort(1433);
   db.setUserName("admin");
   db.setPassword("admin");
   db.setDatabaseName("Qt");

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
