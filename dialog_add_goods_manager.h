#ifndef DIALOG_ADD_GOODS_MANAGER_H
#define DIALOG_ADD_GOODS_MANAGER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>

namespace Ui {
class DialogAddGoodsManager;
}

class DialogAddGoodsManager : public QDialog
{
   Q_OBJECT

public:
   explicit DialogAddGoodsManager(QWidget *parent = nullptr);
   ~DialogAddGoodsManager();

private slots:
   void on_buttonBox_accepted();

private:
   Ui::DialogAddGoodsManager *ui;
   QSqlDatabase db;
   QDoubleValidator *validator_decimalUS;
};

#endif // DIALOG_ADD_GOODS_MANAGER_H
