#include "mainwindowclient.h"
#include "ui_mainwindowclient.h"

MainWindowClient::MainWindowClient(QSqlDatabase database, QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowClient)
{
   db = database;
   ui->setupUi(this);
}

MainWindowClient::~MainWindowClient()
{
   delete ui;
   db.removeDatabase("Qt");
}
