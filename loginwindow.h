#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

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

   void on_register_2_clicked();

private:
   Ui::LoginWindow *ui;
   QMainWindow *main_window = nullptr;
   QSqlDatabase db;
};
#endif // LOGINWINDOW_H
