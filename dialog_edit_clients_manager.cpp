#include "dialog_edit_clients_manager.h"
#include "ui_dialog_edit_clients_manager.h"

dialog_edit_clients_manager::dialog_edit_clients_manager(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::dialog_edit_clients_manager)
{
   ui->setupUi(this);
}

dialog_edit_clients_manager::~dialog_edit_clients_manager()
{
   delete ui;
}
