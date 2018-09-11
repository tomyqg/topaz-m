#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include <QWidget>
#include <QMovie>
#include <QTimer>
#include <settings.h>
#include <systemoptions.h>
#include <defines.h>
#include "kvitirovanie.h"

namespace Ui {
class dMenu;
}

class dMenu : public QDialog
{
    Q_OBJECT

public:
    explicit dMenu(QWidget *parent = 0);
    ~dMenu();
    QTimer timerLoad;
    void addChannels(QList<ChannelOptions *> channels, QList<Ustavka *> ustavki);
    void addSteels(QList<cSteel*> st, typeSteelTech tech);
    void selectPageWork();
    bool eventFilter(QObject* watched, QEvent* event);

public slots:
    void openSettingsChannel(int num, int page = 0);

private slots:
    void on_exitButton_clicked();
    void on_saveButton_clicked();
    void timeoutLoad();
    void on_bWork_clicked();
    void on_bBack_clicked();
    void on_bSettings_clicked();
    void on_bBackFromSettings_clicked();
    void on_bBackFromSystem_clicked();
    void on_bBackFromInputs_clicked();
    void on_bInputs_clicked();
    void on_bSystem_clicked();
    void DateUpdate();
    void on_bBackChannels_clicked();
    void on_bAnalog_clicked();
    void on_bChannel1_clicked();
    void on_bChannel2_clicked();
    void on_bChannel3_clicked();
    void on_bChannel4_clicked();
    void on_bApplication_clicked();
    void on_bBackApplications_clicked();
    void on_bUstavki_clicked();
    void on_bBackApplications_2_clicked();
    void on_bDiagnost_clicked();
    void on_bBackDiagnostika_clicked();

    void on_bUstavka_1_clicked();
    void on_bUstavka_2_clicked();
    void on_bUstavka_3_clicked();
    void on_bUstavka_4_clicked();

    void on_bBackDateTime_clicked();
    void on_bEditDataTime_clicked();
    void on_bDateTimeSet_clicked();

private:
    Ui::dMenu *ui;
    QMovie mo;
    QTimer tUpdateTime;
    QTimer tUpdateBar;
    dSettings * dialogSetingsChannel;
    QList<ChannelOptions *> listChannels;
    QList<Ustavka *> listUstavok;
    void UpdateAnalyze();
    void updateSystemOptions(QString path = pathtosystemoptions);
    cSystemOptions sysOptions;
    QDateTime dateTime;
    QStringList listDrives;
    kvitirovanie mesDialog;

//    QList<cSteel*> listSteels;
//    typeSteelTech steelTech[NUM_TECHNOLOGIES];

private slots:
    void on_bResetToDefault_clicked();
    void on_bExpert_clicked();
    void on_bBackExpert_clicked();
    void on_bAnaliz_clicked();
    void on_bBackExpert_2_clicked();
    void on_bOptions_clicked();
    void on_bBackSystemOptions_clicked();
    void on_bModeDiagram_clicked();
    void on_bBackOtobrazhenie_clicked();
    void on_bBackSteel_clicked();
    void on_bSteel_clicked();
    void on_bSteel1_clicked();
    void on_bSteel2_clicked();
    void on_bSteel3_clicked();
    void on_bSteel4_clicked();
    void on_radioButSteelModes_clicked();
    void on_radioButAnalogModes_clicked();
    void on_dateEdit_d_up_clicked();
    void on_dateEdit_m_up_clicked();
    void on_dateEdit_y_up_clicked();
    void on_dateEdit_d_down_clicked();
    void on_dateEdit_m_down_clicked();
    void on_dateEdit_y_down_clicked();
    void on_timeEdit_h_up_clicked();
    void on_timeEdit_m_up_clicked();
    void on_timeEdit_s_up_clicked();
    void on_timeEdit_h_down_clicked();
    void on_timeEdit_m_down_clicked();
    void on_timeEdit_s_down_clicked();
    void updUiTimeDate(QDateTime td);
    void on_bExtMemory_clicked();
    void on_bBackExternalDrive_clicked();
    void updateDriversWidgets();

    void on_bReadSysFromDrive_clicked();

    void on_bSaveSysToDrive_clicked();

    void on_bSaveMesToDrive_clicked();

    void on_bSaveChanToDrive_clicked();

    void on_bReadChanFromDrive_clicked();

signals:
    void saveButtonSignal();
};

#endif // MENU_H
