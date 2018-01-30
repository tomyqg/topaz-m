#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <mainwindow.h>
#include <Channels/channelOptions.h>

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

    ChannelOptions options_channel1; //#include <channel1.h>
    ChannelOptions options_channel2;
    ChannelOptions options_channel3;
    ChannelOptions options_channel4;

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    static QString calibrationprm;
    static QString olderprop;
    static QString displayResolution, MonitorResolution;
    static int DisplayParametr;
    void SetLogMessagesLimit(double n);
    double GetLogMessagesLimit();
    QString GetCalibration() { return  calibrationprm;}
    void ReadSystemOptionsFromFile();
    void TouchScreenCalibrate();
    void resizeWindow(QObject & qobj, qreal xresolution, qreal yresolution);
    void OpenKeyboard();
    int GetCurrentDisplayParametr() { return DisplayParametr;}

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
    void on_comboBox_currentIndexChanged(int index);
    void on_ArchivetoUSB_button_clicked();
    void on_pushButton_6_clicked();

private:

    QString GetNewDateString();
    QString GetNewTimeString();
    QString GetNewDisplayResolution();
    void SetCurrentDisplayParametr(DisplayParametrEnum newparametr) { DisplayParametr = newparametr;}
    void UpdateCurrentDisplayParametr() ;

    void WriteAllChannelsOptionsToFile();
    bool eventFilter(QObject *object, QEvent *event);
    void WriteSystemOptionsToFile();
    void ApplyNewSettingstoAllChannels();
    void ApplyNewSettingstoOptionsUI();
    void CustomizeOptionsUI();
    void ReadChannelsOptionsFromFile();

    void ArchiveToUSBFlashDrive();
    void ResetToDefaults();

    static double maxmessageslimit;
    Ui::Options *ui;
};

#endif // OPTIONS_H
