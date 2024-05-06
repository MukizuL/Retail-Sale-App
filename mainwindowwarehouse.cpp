#include "mainwindowwarehouse.h"
#include "ui_mainwindowwarehouse.h"

MainWindowWarehouse::MainWindowWarehouse(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowWarehouse)
{
    ui->setupUi(this);
}

MainWindowWarehouse::~MainWindowWarehouse()
{
    delete ui;
}
