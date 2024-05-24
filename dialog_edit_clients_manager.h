#ifndef DIALOG_EDIT_CLIENTS_MANAGER_H
#define DIALOG_EDIT_CLIENTS_MANAGER_H

#include <QDialog>

namespace Ui {
   class dialog_edit_clients_manager;
}

class dialog_edit_clients_manager : public QDialog
{
   Q_OBJECT

public:
   explicit dialog_edit_clients_manager(QWidget *parent = nullptr);
   ~dialog_edit_clients_manager();

private:
   Ui::dialog_edit_clients_manager *ui;
};

#endif // DIALOG_EDIT_CLIENTS_MANAGER_H
