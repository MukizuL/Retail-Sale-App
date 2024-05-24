#ifndef DIALOG_ADD_GOODS_MANAGER_H
#define DIALOG_ADD_GOODS_MANAGER_H

#include <QDialog>

namespace Ui {
   class dialog_add_goods_manager;
}

class dialog_add_goods_manager : public QDialog
{
   Q_OBJECT

public:
   explicit dialog_add_goods_manager(QWidget *parent = nullptr);
   ~dialog_add_goods_manager();

private:
   Ui::dialog_add_goods_manager *ui;
};

#endif // DIALOG_ADD_GOODS_MANAGER_H
