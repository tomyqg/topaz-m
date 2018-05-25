#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QMovie>
#include <QWidget>
#include <QTimer>

namespace Ui {
class dSettings;
}

class dSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dSettings(QWidget *parent = 0);
    ~dSettings();
    QTimer timerLoad;


private:
    Ui::dSettings *ui;
    QMovie mo;
    QTimer tUpdateTime;

private slots:
    void on_exitButton_clicked();
    void on_saveButton_clicked();
    void timeoutLoad();
    void on_buttonUstavk_clicked();
    void DateUpdate();
};

#endif // SETTINGS_H
