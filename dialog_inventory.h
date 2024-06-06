#ifndef DIALOG_INVENTORY_H
#define DIALOG_INVENTORY_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QIntValidator>
#include <QMessageBox>

namespace Ui {
class DialogInventory;
}

class DialogInventory : public QDialog
{
   Q_OBJECT

public:
   explicit DialogInventory(QVector <QVariant> vector, QWidget *parent = nullptr);
   ~DialogInventory();

private slots:
   void handleCheckBoxState(Qt::CheckState state);

   void on_pushButton_ok_clicked();

private:
   Ui::DialogInventory *ui;
   QVector <QVariant> data_;
   QIntValidator *validator_int;
   QSqlDatabase db;
};

#endif // DIALOG_INVENTORY_H
