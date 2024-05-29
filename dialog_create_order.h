#ifndef DIALOG_CREATE_ORDER_H
#define DIALOG_CREATE_ORDER_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>

namespace Ui {
class DialogCreateOrder;
}

class DialogCreateOrder : public QDialog
{
   Q_OBJECT

public:
   explicit DialogCreateOrder(QSqlDatabase database, int client, QWidget *parent = nullptr);
   ~DialogCreateOrder();

private slots:
   void on_pushButton_add_clicked();

   void on_pushButton_remove_clicked();

   void on_pushButton_create_clicked();

private:
   void calculate_total();

   Ui::DialogCreateOrder *ui;
   QSqlDatabase db;
   QSqlDatabase cart;
   QSqlTableModel *goods_model;
   QSqlTableModel *cart_model;
   QVector <int> added_items;
   int id_client;
};

#endif // DIALOG_CREATE_ORDER_H
