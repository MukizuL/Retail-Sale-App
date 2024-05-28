#ifndef DIALOG_EDIT_CLIENTS_MANAGER_H
#define DIALOG_EDIT_CLIENTS_MANAGER_H

#include <QDialog>

namespace Ui {
   class DialogEditClientsManager;
}

class DialogEditClientsManager : public QDialog
{
   Q_OBJECT

public:
   explicit DialogEditClientsManager(QWidget *parent = nullptr);
   ~DialogEditClientsManager();

private:
   Ui::DialogEditClientsManager *ui;
};

#endif // DIALOG_EDIT_CLIENTS_MANAGER_H
