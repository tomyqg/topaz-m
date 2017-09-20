#ifndef STACKEDOPTIONS_H
#define STACKEDOPTIONS_H

#include <QDialog>

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
    explicit StackedOptions(QWidget *parent = 0);
    ~StackedOptions();

private slots:

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

private:
    Ui::StackedOptions *ui;
    void ReadChannelsOptionsFromFile();
    void ReadSystemOptionsFromFile();
    void ApplyNewSettingstoOptionsUI();
};

#endif // STACKEDOPTIONS_H
