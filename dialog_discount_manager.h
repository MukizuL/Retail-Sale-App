#ifndef DIALOG_DISCOUNT_MANAGER_H
#define DIALOG_DISCOUNT_MANAGER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class DialogDiscountManager;
}

class DialogDiscountManager : public QDialog
{
   Q_OBJECT

public:
   explicit DialogDiscountManager(QSqlDatabase database, QWidget *parent = nullptr);
   ~DialogDiscountManager();

private slots:
   void on_pushButton_ok_clicked();

private:
   Ui::DialogDiscountManager *ui;
   QSqlDatabase db;
};

#endif // DIALOG_DISCOUNT_MANAGER_H
