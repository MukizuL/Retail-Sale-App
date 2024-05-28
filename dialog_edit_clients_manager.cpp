#include "dialog_edit_clients_manager.h"
#include "ui_dialog_edit_clients_manager.h"

DialogEditClientsManager::DialogEditClientsManager(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogEditClientsManager)
{
   ui->setupUi(this);
}

DialogEditClientsManager::~DialogEditClientsManager()
{
   delete ui;
}
