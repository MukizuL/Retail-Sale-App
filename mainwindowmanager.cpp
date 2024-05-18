#include "mainwindowmanager.h"
#include "ui_mainwindowmanager.h"

MainWindowManager::MainWindowManager(QSqlDatabase database, QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowManager)
{
   db = database;
   ui->setupUi(this);
}

MainWindowManager::~MainWindowManager()
{
   delete ui;
   db.removeDatabase("Qt");
}
