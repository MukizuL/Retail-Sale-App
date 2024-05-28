#ifndef DIALOG_EDIT_GOODS_MANAGER_H
#define DIALOG_EDIT_GOODS_MANAGER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class dialog_edit_goods_manager;
}

class dialog_edit_goods_manager : public QDialog
{
   Q_OBJECT

public:
   explicit dialog_edit_goods_manager(QSqlDatabase database, QVector <QVariant> data, QWidget *parent = nullptr);
   ~dialog_edit_goods_manager();

private slots:
   void on_pushButton_delete_clicked();

   void on_pushButton_ok_clicked();

private:
   Ui::dialog_edit_goods_manager *ui;
   QSqlDatabase db;
   QVector <QVariant> item_data;
};

#endif // DIALOG_EDIT_GOODS_MANAGER_H
