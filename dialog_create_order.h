#ifndef DIALOG_CREATE_ORDER_H
#define DIALOG_CREATE_ORDER_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QSqlQueryModel>

namespace Ui {
class DialogCreateOrder;
}

class DialogCreateOrder : public QDialog
{
   Q_OBJECT

public:
   explicit DialogCreateOrder(int client, QWidget *parent = nullptr);
   ~DialogCreateOrder();

private slots:
   void on_pushButton_add_clicked();

   void on_pushButton_remove_clicked();

   void on_pushButton_create_clicked();

private:
   void calculate_total();
   void update_model_goods();
   void update_model_cart();

   Ui::DialogCreateOrder *ui;
   QSqlDatabase db;
   QSqlQueryModel *goods_model;
   QSqlQueryModel *cart_model;
   QVector <int> added_items;
   int id_client;
   QIntValidator *validator_int_goods;
};

#endif // DIALOG_CREATE_ORDER_H
