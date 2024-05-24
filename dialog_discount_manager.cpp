#include "dialog_discount_manager.h"
#include "ui_dialog_discount_manager.h"

dialog_discount_manager::dialog_discount_manager(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::dialog_discount_manager)
{
   ui->setupUi(this);
}

dialog_discount_manager::~dialog_discount_manager()
{
   delete ui;
}
