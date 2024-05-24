#include "dialog_add_goods_manager.h"
#include "ui_dialog_add_goods_manager.h"

dialog_add_goods_manager::dialog_add_goods_manager(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::dialog_add_goods_manager)
{
   ui->setupUi(this);
}

dialog_add_goods_manager::~dialog_add_goods_manager()
{
   delete ui;
}
