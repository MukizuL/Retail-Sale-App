#include "dialog_edit_goods_manager.h"
#include "ui_dialog_edit_goods_manager.h"

dialog_edit_goods_manager::dialog_edit_goods_manager(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::dialog_edit_goods_manager)
{
   ui->setupUi(this);
}

dialog_edit_goods_manager::~dialog_edit_goods_manager()
{
   delete ui;
}
