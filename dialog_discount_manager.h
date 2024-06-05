#ifndef DIALOG_DISCOUNT_MANAGER_H
#define DIALOG_DISCOUNT_MANAGER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>

namespace Ui {
class DialogDiscountManager;
}

class DialogDiscountManager : public QDialog
{
   Q_OBJECT

public:
   explicit DialogDiscountManager(QWidget *parent = nullptr);
   ~DialogDiscountManager();

private slots:
   void on_pushButton_ok_clicked();

private:
   Ui::DialogDiscountManager *ui;
   QSqlDatabase db;
   QIntValidator *validator_int_discount;
   QIntValidator *validator_int_goods;
   QDoubleValidator *validator_decimalUS;
};

#endif // DIALOG_DISCOUNT_MANAGER_H
