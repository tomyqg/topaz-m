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
    QTimer timerLoad;

private slots:
    void on_exitButton_clicked();
    void on_saveButton_clicked();
    void timeoutLoad();
    void on_bWork_clicked();
    void on_bBack_clicked();
    void on_bSettings_clicked();
    void on_bBackFromSettings_clicked();
    void on_bBackFromSystem_clicked();
    void on_bBackFromInputs_clicked();
    void on_bInputs_clicked();
    void on_bSystem_clicked();
    void DateUpdate();

private:
    Ui::dMenu *ui;
    QMovie mo;
    QTimer tUpdateTime;
};

#endif // MENU_H
