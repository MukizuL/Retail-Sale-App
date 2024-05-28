#ifndef DIALOG_DISCOUNT_MANAGER_H
#define DIALOG_DISCOUNT_MANAGER_H

#include <QDialog>

namespace Ui {
   class DialogDiscountManager;
}

class dialog_discount_manager : public QDialog
{
   Q_OBJECT

public:
   explicit dialog_discount_manager(QWidget *parent = nullptr);
   ~dialog_discount_manager();

private:
   Ui::DialogDiscountManager *ui;
};

#endif // DIALOG_DISCOUNT_MANAGER_H
