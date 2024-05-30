#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class MainWindowManager;
}

class MainWindowManager : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindowManager(QWidget *parent = nullptr);
   ~MainWindowManager();

private slots:
   void on_addButton_manager_goods_clicked();

   void on_editButton_manager_goods_clicked();

   void on_tableView_manager_goods_clicked(const QModelIndex&index);

   void on_discountButton_manager_goods_clicked();

private:
   Ui::MainWindowManager *ui;
   QSqlDatabase db;
   QSqlTableModel *users_model;
   QSqlTableModel *goods_model;
   QSqlTableModel *orders_model;
   int row;
};

#endif // MAINWINDOWMANAGER_H
