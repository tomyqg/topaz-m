#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>
#include "mainwindow.h"
#include "log.h"
#include <QString>

#include <Channels/channelOptions.h>
namespace Ui {
class keyboard;
}

typedef struct
{
    QString eng;
    QString ENG;
    QString rus;
    QString RUS;
}langSimbols;

class keyboard : public QDialog
{
    Q_OBJECT

public:
    explicit keyboard(QWidget *parent = 0);

    ~keyboard();

    static QString olderprop;
    static QString newString;

    QString olderproperty;
    bool onlydigits;
    void setWarning(QString warn, bool secr = false);

private slots:
//    void on_buttonBox_accepted();
    void textinput(int b);
    void on_pushButton_13_clicked();
    void on_pushButton_27_clicked();
    void on_pushButton_44_clicked();
    void on_comboBox_currentIndexChanged();

    void on_pushButton_28_clicked();

    void on_pushButton_27_clicked(bool checked);

    void on_pushButton_27_toggled(bool checked);

    void on_pushButton_59_clicked();

public slots:
    QString getcustomstring();
    void setolderproperty(QString str);
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::keyboard *ui;
    void ChangeLanguage(int eng);
    bool shift;
    static langSimbols simbols[32];
    QString styleUnclicked;
    QString styleClicked;
    bool secret;
//    QString warning;        //предупреждение на клавиатуре
};



#endif // KEYBOARD_H
