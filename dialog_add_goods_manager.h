#ifndef DIALOG_ADD_GOODS_MANAGER_H
#define DIALOG_ADD_GOODS_MANAGER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class dialog_add_goods_manager;
}

class dialog_add_goods_manager : public QDialog
{
   Q_OBJECT

public:
   explicit dialog_add_goods_manager(QSqlDatabase database, QWidget *parent = nullptr);
   ~dialog_add_goods_manager();

private slots:
   void on_buttonBox_accepted();

private:
   Ui::dialog_add_goods_manager *ui;
   QSqlDatabase db;
};

#endif // DIALOG_ADD_GOODS_MANAGER_H
