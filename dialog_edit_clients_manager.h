#ifndef DIALOG_EDIT_CLIENTS_MANAGER_H
#define DIALOG_EDIT_CLIENTS_MANAGER_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class DialogEditClientsManager;
}

class DialogEditClientsManager : public QDialog
{
   Q_OBJECT

public:
   explicit DialogEditClientsManager(int id_user, QWidget *parent = nullptr);
   ~DialogEditClientsManager();

private slots:
   void on_pushButton_ok_clicked();

   void on_pushButton_delete_clicked();

private:
   void update_view();

   Ui::DialogEditClientsManager *ui;
   QSqlDatabase db;
   int user;
   int initial_role;
};

#endif // DIALOG_EDIT_CLIENTS_MANAGER_H
