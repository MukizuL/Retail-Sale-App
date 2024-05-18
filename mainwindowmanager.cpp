#include "mainwindowmanager.h"
#include "ui_mainwindowmanager.h"

MainWindowManager::MainWindowManager(QSqlDatabase database, QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindowManager)
{
   ui->setupUi(this);
   this->setAttribute(Qt::WA_DeleteOnClose);
}

MainWindowManager::~MainWindowManager()
{
   delete ui;
}
