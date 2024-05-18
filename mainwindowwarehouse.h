#ifndef MAINWINDOWWAREHOUSE_H
#define MAINWINDOWWAREHOUSE_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class MainWindowWarehouse;
}

class MainWindowWarehouse : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindowWarehouse(QSqlDatabase database, QWidget *parent = nullptr);
   ~MainWindowWarehouse();

private:
   Ui::MainWindowWarehouse *ui;
   QSqlDatabase db;
};

#endif // MAINWINDOWWAREHOUSE_H
