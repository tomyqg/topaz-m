#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include <QWidget>
#include <QMovie>
#include <QTimer>
#include "settings.h"

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
    void addChannels(QList<ChannelOptions *> channels);

public slots:
    void openSettingsChannel(int num);

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
    void on_bUniversal_clicked();
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

private:
    Ui::dMenu *ui;
    QMovie mo;
    QTimer tUpdateTime;
    QTimer tUpdateBar;
    dSettings * dialogSetingsChannel;
    QList<ChannelOptions *> listChannels;
};

#endif // MENU_H
