#ifndef DIALOG_DISCOUNT_MANAGER_H
#define DIALOG_DISCOUNT_MANAGER_H

#include <QDialog>

namespace Ui {
   class DialogDiscountManager;
}

class DialogDiscountManager : public QDialog
{
   Q_OBJECT

public:
   explicit DialogDiscountManager(QWidget *parent = nullptr);
   ~DialogDiscountManager();

private:
   Ui::DialogDiscountManager *ui;
};

#endif // DIALOG_DISCOUNT_MANAGER_H
