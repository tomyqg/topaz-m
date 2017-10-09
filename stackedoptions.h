#ifndef STACKEDOPTIONS_H
#define STACKEDOPTIONS_H

#include <QDialog>
#include <QtGui>

#include <channel1.h>

namespace Ui {
class StackedOptions;
}

class StackedOptions : public QDialog
{
    Q_OBJECT

    ChannelOptions options_channel1;
    ChannelOptions options_channel2;
    ChannelOptions options_channel3;
    ChannelOptions options_channel4;

public:
    explicit StackedOptions(int pageindex, QWidget *parent);
    ~StackedOptions();
    static int GetCurrentDisplayParametr() { return DisplayParametr;}

    void SetPageIndex(int newpageindex)
    {
        pageindex = newpageindex;
    }

    int GetPageIndex ()
    {
        return pageindex;
    }

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

    static void SetCurrentDisplayParametr(DisplayParametrEnum newparametr) { DisplayParametr = newparametr;}

private slots:

    bool eventFilter(QObject* watched, QEvent* event);

    void Channel1TypeChange();
    void Channel2TypeChange();
    void Channel3TypeChange();
    void Channel4TypeChange();

    void UpdateArchiveData();

    void UpdateAnalyze();

    void SetLabelIndex(int newlabelindex);

    void SetStackIndex(int newstackindex);

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_13_clicked();

    void on_ChangeDateButton_clicked();

    void on_pushButton_15_clicked();

    void on_ExtendedOptionsButton_clicked();

    void on_pushButton_16_clicked();

    void on_ChangeDateButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_34_clicked();

    void on_analyzebackbutton_clicked();

    void on_plotarchive_clicked();

    void on_pushButton_109_clicked();

    void on_gamebutton_clicked();

private:

    Ui::StackedOptions *ui;
    void ReadChannelsOptionsFromFile();
    void ReadSystemOptionsFromFile();
    void ApplyNewSettingstoOptionsUI();
    void ApplyNewSettingstoAllChannels();
    void WriteSystemOptionsToFile();
    void WriteAllChannelsOptionsToFile();
    void UpdateCurrentDisplayParametr() ;
    void InitiateArchive() ;
    void ResetToDefaults();
    QString GetNewDateString();
    QString GetNewTimeString();
    QString GetNewDisplayResolution();

    QString GetCalibration() { return  calibrationprm;}
    static int DisplayParametr;
    static QString displayResolution, MonitorResolution;
    static QString calibrationprm;

    int pageindex ;
    QPen graphPen;
    QStringList StringListNone, StringListTok, StringListNapryagenie, StringListRTD, StringListTC; // списки названий для каждого типа сигналов



};

#endif // STACKEDOPTIONS_H
