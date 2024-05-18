#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class MainWindowManager;
}

class MainWindowManager : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindowManager(QSqlDatabase database, QWidget *parent = nullptr);
   ~MainWindowManager();

private:
   Ui::MainWindowManager *ui;
   QSqlDatabase db;
};

#endif // MAINWINDOWMANAGER_H
