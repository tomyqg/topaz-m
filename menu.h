#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include <QWidget>
#include <QMovie>
#include <QTimer>

namespace Ui {
class dMenu;
}

class dMenu : public QDialog
{
    Q_OBJECT

public:
    explicit dMenu(QWidget *parent = 0);
    ~dMenu();
//    QTimer * timerLoad;

private slots:
    void on_exitButton_clicked();
    void on_saveButton_clicked();
//    void timeoutLoad();

private:
    Ui::dMenu *ui;
//    QMovie mo;
//    bool fTimeoutLoad;
};

#endif // MENU_H
