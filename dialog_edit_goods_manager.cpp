#include "dialog_edit_goods_manager.h"
#include "ui_dialog_edit_goods_manager.h"

dialog_edit_goods_manager::dialog_edit_goods_manager(QSqlDatabase database, QVector <QVariant> data, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::dialog_edit_goods_manager)
{
   db        = database;
   item_data = data;
   ui->setupUi(this);
   ui->lineEdit_name->setText(item_data[1].toString());
   ui->lineEdit_retail->setText(item_data[2].toString());
   ui->lineEdit_bulk->setText(item_data[3].toString());
   ui->plainTextEdit_info->setPlainText(item_data[4].toString());
}

dialog_edit_goods_manager::~dialog_edit_goods_manager()
{
   delete ui;
}
