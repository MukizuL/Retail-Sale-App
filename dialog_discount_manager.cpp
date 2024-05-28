#include "dialog_discount_manager.h"
#include "ui_dialog_discount_manager.h"

DialogDiscountManager::DialogDiscountManager(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogDiscountManager)
{
   ui->setupUi(this);
}

DialogDiscountManager::~DialogDiscountManager()
{
   delete ui;
}
