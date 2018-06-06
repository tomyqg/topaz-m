#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QMovie>
#include <QWidget>
#include <QTimer>
#include <Channels/channelOptions.h>
#include <ustavka.h>

namespace Ui {
class dSettings;
}

class dSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dSettings(QList<ChannelOptions*> channels, QList<Ustavka*> ustavki, int num, int page = 0, QWidget *parent = 0);
    ~dSettings();
    QTimer timerLoad;

public slots:
    void resizeEvent(QResizeEvent * s);


private:
    Ui::dSettings *ui;
    QMovie mo;
    QTimer tUpdateTime;
    QTimer tUpdateBar;
    void updateGraf(int period);
    QList<ChannelOptions *> listChannels;
    QList<Ustavka *> listUstavok;
    void updateWidgets();
    int numChannel;
    ChannelOptions * channel;   //канал с которым сейчас работаем
    Ustavka * ustavka;          //уставка канала с которым работаем
    void addChannel(QList<ChannelOptions *> channels, QList<Ustavka *> ustavki, int num);

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
    void saveParam();
};

#endif // SETTINGS_H
