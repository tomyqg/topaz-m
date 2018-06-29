#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QMovie>
#include <QWidget>
#include <QTimer>
#include <Channels/channelOptions.h>
#include <ustavka.h>
#include <archivator.h>

namespace Ui {
class dSettings;
}

class dSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dSettings(QList<ChannelOptions*> channels,
                       QList<Ustavka*> ustavki,
                       int num,
                       int page = 0,
                       cArchivator * ar = NULL,
                       QWidget *parent = 0);
    ~dSettings();
    QTimer timerLoad;
    void addArch(cArchivator * archiv) {arch = archiv;}

public slots:
    void resizeEvent(QResizeEvent * s);


private:
    Ui::dSettings *ui;
    QMovie mo;
    QTimer tUpdateTime;
    QTimer tUpdateBar;
    QStringList StringListNapryagenie, StringListRTD, StringListTC; // списки названий для каждого типа сигналов
    void updateGraf(int period);
    QList<ChannelOptions *> listChannels;
    QList<Ustavka *> listUstavok;
    void updateWidgets();
    int numChannel;
    ChannelOptions * channel;   //канал с которым сейчас работаем
    Ustavka * ustavka;          //уставка канала с которым работаем
    void addChannel(QList<ChannelOptions *> channels, QList<Ustavka *> ustavki, int num);
    int getIndexSignalTypeTable(int st);
    void updateUiSignalTypeParam(int index);
    int sensorShemaFromUiShemaIndex(int index);
    int indexUiShemaFromSensorShema(int sh);
    QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
    cArchivator * arch;
    QVector<QString> Labels;

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
    bool eventFilter(QObject* watched, QEvent* event);
    void on_typeSignal_currentIndexChanged(int index);
};

#endif // SETTINGS_H
