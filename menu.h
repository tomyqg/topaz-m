#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class dMenu;
}

class dMenu : public QDialog
{
    Q_OBJECT

public:
    explicit dMenu(QWidget *parent = 0);
    ~dMenu();

private:
    Ui::dMenu *ui;

};

#endif // MENU_H
