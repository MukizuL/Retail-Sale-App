#ifndef DIALOG_VIEW_ORDERS_H
#define DIALOG_VIEW_ORDERS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

namespace Ui {
class DialogViewOrders;
}

class DialogViewOrders : public QDialog
{
   Q_OBJECT

public:
   explicit DialogViewOrders(QVector <QVariant> selected_order, QWidget *parent = nullptr);
   ~DialogViewOrders();

private slots:
   void on_pushButton_export_clicked();

   void on_pushButton_cancel_order_clicked();

private:
   void update_model();

   Ui::DialogViewOrders *ui;
   QSqlDatabase db;
   QSqlQueryModel *items_model;
   QVector <QVariant> items;
   QVariant id_order;
};

#endif // DIALOG_VIEW_ORDERS_H
