#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "mainwindowclient.h"
#include "mainwindowmanager.h"
#include "mainwindowwarehouse.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QDialog
{
   Q_OBJECT

public:
   LoginWindow(QWidget *parent = nullptr);
   ~LoginWindow();
   void db_connect();

private slots:
   void on_signin_clicked();

   void on_invokesql_clicked();

   void on_db_reconnect_clicked();

private:
   Ui::LoginWindow *ui;
   MainWindowClient *main_window_client       = NULL;
   MainWindowManager *main_window_manager     = NULL;
   MainWindowWarehouse *main_window_warehouse = NULL;
   QSqlDatabase db;
};
#endif // LOGINWINDOW_H
