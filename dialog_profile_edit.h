#ifndef DIALOG_PROFILE_EDIT_H
#define DIALOG_PROFILE_EDIT_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class DialogProfileEdit;
}

class DialogProfileEdit : public QDialog
{
   Q_OBJECT

public:
   explicit DialogProfileEdit(bool password, int id_client, QWidget *parent = nullptr);
   ~DialogProfileEdit();

private slots:
   void on_pushButton_ok_clicked();

   void on_pushButton_ok_2_clicked();

private:
   void update_view();

   Ui::DialogProfileEdit *ui;
   QSqlDatabase db;
   bool passwd;
   int user;
};

#endif // DIALOG_PROFILE_EDIT_H
