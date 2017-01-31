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

    ChannelOptions options1;
    ChannelOptions options2;
    ChannelOptions options3;

    ChannelOptions options16;

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

public slots:

    void WriteOptionsToFile();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void checkboxchange();

    void savesettings();

    int GetSignalType();

    QString GetSignalUnits();

    void on_VerhnPredIzmerChannel_1_valueChanged(int arg1);

    void on_PeriodIzmerChannel_1_valueChanged(int arg1);

    void on_UnitsChannel_1_editingFinished();

private:

    Ui::Options *ui;
};


#endif // OPTIONS_H
