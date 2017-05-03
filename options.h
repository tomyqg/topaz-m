#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>

#include <mainwindow.h>

#include <channel1.h>

namespace Ui {
class Options;
}


class Options : public QDialog
{
    Q_OBJECT

    ChannelOptions options_channel1;
    ChannelOptions options_channel2;
    ChannelOptions options_channel3;
    ChannelOptions options_channel4;

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    static QString calibrationprm;
    static QString olderprop;
    static int DisplayParametr;
    void SetLogMessagesLimit(double n);
    double GetLogMessagesLimit();
    enum DisplayParametrEnum {
        Trends = 0x01 ,
        Polar = 0x02 ,
        Bars = 0x03 ,
        TrendsBars = 0x04,
        Cyfra = 0x05,
        TrendsCyfra = 0x06,
        BarsCyfra = 0x07,
        TrendsCyfraBars = 0x08
    };
private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_UnitsChannel_1_editingFinished();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_horizontalScrollBar_sliderReleased();
    void Channel1TypeChange();
    void Channel2TypeChange();
    void Channel3TypeChange();
    void Channel4TypeChange();



private:

    void WriteAllChannelsOptionsToFile();
    bool eventFilter(QObject *object, QEvent *event);
    void WriteSystemOptionsToFile();
    void ApplyNewSettingstoAllChannels();
    void ApplyNewSettingstoOptionsUI();
    void CustomizeOptionsUI();
    void ReadChannelsOptionsFromFile();
    void ReadSystemOptionsFromFile();
    static double maxmessageslimit;
    Ui::Options *ui;



};

#endif // OPTIONS_H
