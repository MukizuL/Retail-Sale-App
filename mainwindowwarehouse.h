#ifndef MAINWINDOWWAREHOUSE_H
#define MAINWINDOWWAREHOUSE_H

#include <QMainWindow>

namespace Ui {
class MainWindowWarehouse;
}

class MainWindowWarehouse : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowWarehouse(QWidget *parent = nullptr);
    ~MainWindowWarehouse();

private:
    Ui::MainWindowWarehouse *ui;
};

#endif // MAINWINDOWWAREHOUSE_H
