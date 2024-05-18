#include "mainwindowwarehouse.h"
#include "ui_mainwindowwarehouse.h"

MainWindowWarehouse::MainWindowWarehouse(QSqlDatabase database, QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowWarehouse)
{
   db = database;
   ui->setupUi(this);
}

MainWindowWarehouse::~MainWindowWarehouse()
{
   delete ui;
   db.removeDatabase("Qt");
}
