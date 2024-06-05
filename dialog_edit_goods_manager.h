#ifndef DIALOG_EDIT_GOODS_MANAGER_H
#define DIALOG_EDIT_GOODS_MANAGER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>

namespace Ui {
class DialogEditGoodsManager;
}

class DialogEditGoodsManager : public QDialog
{
   Q_OBJECT

public:
   explicit DialogEditGoodsManager(QVector <QVariant> data, QWidget *parent = nullptr);
   ~DialogEditGoodsManager();

private slots:
   void on_pushButton_delete_clicked();

   void on_pushButton_ok_clicked();

private:
   Ui::DialogEditGoodsManager *ui;
   QSqlDatabase db;
   QVector <QVariant> item_data;
   QDoubleValidator *validator_decimalUS;
};

#endif // DIALOG_EDIT_GOODS_MANAGER_H
