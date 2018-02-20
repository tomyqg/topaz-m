#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>
#include "mainwindow.h"
#include "log.h"

#include <Channels/channelOptions.h>
namespace Ui {
class keyboard;
}

class keyboard : public QDialog
{
    Q_OBJECT

public:
    explicit keyboard(QWidget *parent = 0);

    ~keyboard();

    QString olderproperty;

    bool onlydigits;

private slots:
//    void on_buttonBox_accepted();
    void textinput(int b);
    void on_pushButton_13_clicked();
    void on_pushButton_27_clicked();
    void on_pushButton_44_clicked();
    void on_comboBox_currentIndexChanged();

    void on_pushButton_28_clicked();

public slots:
    QString getcustomstring();
    void setolderproperty(QString str);
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::keyboard *ui;

private:
    void ChangeLanguage(int eng);
};

#endif // KEYBOARD_H
