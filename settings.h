#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QMovie>
#include <QWidget>
#include <QTimer>
#include <Channels/channelOptions.h>

namespace Ui {
class dSettings;
}

class dSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dSettings(int page, QWidget *parent = 0);
    ~dSettings();
    QTimer timerLoad;
    void addChannel(ChannelOptions * ch, int num);


private:
    Ui::dSettings *ui;
    QMovie mo;
    QTimer tUpdateTime;
    QTimer tUpdateBar;
    void updateGraf(int period);
    ChannelOptions * channel;

private slots:
    void on_exitButton_clicked();
    void on_saveButton_clicked();
    void timeoutLoad();
    void on_buttonUstavk_clicked();
    void DateUpdate();
    void updateBar();
    void on_verticalScrollBar_sliderMoved(int position);
    void on_period_currentIndexChanged(int index);
    void on_buttonBackUstavki_clicked();
};

#endif // SETTINGS_H
