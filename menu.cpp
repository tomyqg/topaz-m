#include "menu.h"
#include "ui_menu.h"
#include "defines.h"
#include "filemanager.h"
#include "keyboard.h"
#include <options.h>
#include <QFile>
#include <QDir>
#include <QString>
#include "usb_flash.h"


#define HEIGHT 768
#define WIDTH 1024
#define TIME_UPDATE DateLabelUpdateTimer
#define DRIVE_UPDATE 500

extern int dateindex;
extern int timeindex;
extern QStringList datestrings, timestrings;
extern QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;
extern QList<cSteel*> listSteel;
extern typeSteelTech steelTech[];
extern cChannelSlotController csc;
extern cSteelController ssc;
extern cSystemOptions systemOptions;  //класс хранения состемных опций
extern cUsbFlash * flash;


dMenu::dMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dMenu)
{


    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);

    QString ver = CURRENT_VER;
    ui->name->setText(QString("<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">MULTIGRAPH<br/>Ver. " + ver + "</span></p></body></html>"));

    ui->saveButton->setColorText(ColorBlue);
    ui->saveButton->setColorBg(QColor(0xff,0xff,0xff));
    ui->exitButton->setColorText(ColorBlue);
    ui->exitButton->setColorBg(QColor(0xff,0xff,0xff));

    QList<wButtonStyled *> buttons = ui->stackedWidget->findChildren<wButtonStyled *>();
    foreach(wButtonStyled * button, buttons)
    {
        button->setColorText(QColor(0xff,0xff,0xff));
        button->setColorBg(ColorButtonNormal);
        button->setAlignLeft();
    }

    //фильтр для поля редактирования даты
//    ui->dateEdit->installEventFilter(this);

    ui->load->setHidden(true);
    mo.setFileName(pathtoloadgif);
    ui->load->setMovie(&mo);
    mo.start();

    connect(&timerLoad, SIGNAL(timeout()), this, SLOT(timeoutLoad()));

    QPixmap pix(pathtologotip);
    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);

    ui->stackedWidget->setCurrentIndex(0);
    ui->frameNameSubMenu->setHidden(true);

    connect(&tUpdateTime, SIGNAL(timeout()), this, SLOT(DateUpdate()));
    tUpdateTime.start(TIME_UPDATE);
    DateUpdate();

    connect(flash, SIGNAL(newFlash(int)), this, SLOT(updateDriversWidgets()));

//    connect(&tUpdateTime, SIGNAL(timeout()), this, SLOT(DateUpdate()));
//    tUpdateTime.start(DRIVE_UPDATE);
//    DateUpdate();


    updateSystemOptions();

//    ui->bReadChanFromDrive->setFontSize(14);
//    ui->bReadChanFromDrive->setAlignCenter();
//    ui->bReadSysFromDrive->setFontSize(14);
//    ui->bReadSysFromDrive->setAlignCenter();
//    ui->bSaveChanToDrive->setFontSize(14);
//    ui->bSaveChanToDrive->setAlignCenter();
//    ui->bSaveSysToDrive->setFontSize(14);
//    ui->bSaveSysToDrive->setAlignCenter();

//    //в зависимости от подключенных плат определям, что показывать или не показывать в меню
//    if(csc.isConnect())
//    {
//        ui->frameAnalogModes->show();
//        ui->radioButAnalogModes->setEnabled(true);
//        ui->radioButAnalogModes->setChecked(true);
//        ui->bAnalog->show();
//    }
//    else
//    {
//        ui->frameAnalogModes->hide();
//        ui->radioButAnalogModes->setEnabled(false);
//        ui->bAnalog->hide();
//    }
//    if(ssc.isConnect())
//    {
//        ui->radioButSteelModes->setEnabled(true);
//        ui->bSteel->show();
//        if(!csc.isConnect())
//        {
//            ui->frameSteelMode->show();
//            ui->radioButSteelModes->setChecked(true);
//        }
//        else
//        {
//           ui->frameSteelMode->hide();
//        }
//    }
//    else
//    {
//        ui->radioButSteelModes->setEnabled(false);
//        ui->frameSteelMode->hide();
//        ui->bSteel->hide();
//    }


}

bool dMenu::eventFilter(QObject *object, QEvent *event)
{
#ifndef Q_OS_WIN
    if ( (event->type() == QEvent::MouseButtonRelease) && \
         (object->property("enabled").toString() == "true") && \
         (QString::fromLatin1(object->metaObject()->className()) == "QDateTime") )
    {
//        QCalendarWidget cw;
//        cw.show();
        Options::olderprop = object->property("date").toString();
        keyboard kb(this);
        kb.setModal(true);
        kb.exec();
    }
#endif
    return QObject::eventFilter(object, event);
}

dMenu::~dMenu()
{
    delete ui;
}

void dMenu::on_exitButton_clicked()
{
    this->close();
}

void dMenu::on_saveButton_clicked()
{
    ui->load->setHidden(false);
//     засекаем время записи настроек в файл или ждать сигнал о завершении
    timerLoad.start(1000);
    //  запись файла //
    sysOptions.arrows = ui->arrowscheckBox->checkState();
    sysOptions.display = ui->modeGraf->currentIndex();
    sysOptions.display += (ui->modeBar->currentIndex() << 2);
    if(ui->radioButSteelModes->isChecked())
    {
        sysOptions.display = cSystemOptions::Steel;
    }
    sysOptions.autoscale = ui->autoscalecheckbox->isChecked();
    cFileManager::writeSystemOptionsToFile(pathtosystemoptions, &sysOptions);
    emit saveButtonSignal();
    //Окно закроется по сигналу таймаута
}

void dMenu::updateSystemOptions(QString path)
{
    cFileManager::readSystemOptionsFromFile(path, &sysOptions);
    ui->arrowscheckBox->setChecked(sysOptions.arrows);
    ui->modeBar->setCurrentIndex((sysOptions.display >> 2) & 4);
    ui->modeGraf->setCurrentIndex(sysOptions.display & 3);
    if(ssc.isConnect() && csc.isConnect())
    {
        ui->groupBoxTypePribor->show();
        if(sysOptions.display == cSystemOptions::Steel)
        {
            ui->radioButAnalogModes->setChecked(false);
            ui->radioButSteelModes->setChecked(true);
            ui->frameAnalogModes->hide();
            ui->frameSteelMode->show();
        } else {
            ui->radioButSteelModes->setChecked(false);
            ui->radioButAnalogModes->setChecked(true);
        }
    }
    else
    {
        ui->groupBoxTypePribor->hide();
    }
    ui->autoscalecheckbox->setChecked(sysOptions.autoscale);
}

void dMenu::timeoutLoad()
{
    this->close();
}

void dMenu::on_bWork_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->nameSubMenu->setText("РАБОТА");
    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
//    ui->nameSubMenu->setText("");
    ui->frameNameSubMenu->setHidden(true);
}


void dMenu::on_bSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameSubMenu->setText("НАСТРОЙКИ");
    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bBackFromSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
//    ui->nameSubMenu->setText("");
    ui->frameNameSubMenu->setHidden(true);
}

void dMenu::on_bBackFromSystem_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameSubMenu->setText("НАСТРОЙКИ");
//    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bBackFromInputs_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameSubMenu->setText("НАСТРОЙКИ");
//    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bInputs_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->nameSubMenu->setText("ВХОДЫ");
//    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bSystem_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->nameSubMenu->setText("СИСТЕМА");
}

void dMenu::DateUpdate() // каждую секунду обновляем значок времени
{
    QDateTime local(QDateTime::currentDateTime());
    QString str = "<html><head/><body><p align=\"center\"><span style=\" font-size:22pt; color:#ffffff;\">" \
                  + local.time().toString(timestrings.at(timeindex)) + \
                  "</span><span style=\" color:#ffffff;\"><br/></span>" \
                  "<span style=\" font-size:17pt; color:#ffffff;\">" \
                  + local.date().toString(datestrings.at(dateindex)) + \
                  "</span></p></body></html>";
    ui->date_time->setText(str);
}


void dMenu::on_bBackChannels_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->nameSubMenu->setText("ВХОДЫ");
}

void dMenu::on_bAnalog_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->nameSubMenu->setText("АНАЛОГ. ВХОДЫ");
}

void dMenu::openSettingsChannel(int num, int page)
{

    if(page == 0)
    {
        //проверка на наличие такого номера канала
        if((num <= 0) || (num > listChannels.size())) return;

        dialogSetingsChannel = new dSettings(listChannels, listUstavok, num, page);
        dialogSetingsChannel->exec();
        dialogSetingsChannel->deleteLater();
        //sendConfigChannelsToSlave();
    }
    else if(page == 4)
    {
        //проверка на наличие такого номера входной группы
        if((num <= 0) || (num > listSteel.size())) return;
        dialogSetingsChannel = new dSettings(listChannels, listUstavok, num, page);
//        dialogSetingsChannel->addSteel(listSteel.at(num), steelTech);
        dialogSetingsChannel->exec();
        dialogSetingsChannel->deleteLater();

    }

}

void dMenu::on_bChannel1_clicked()
{
    openSettingsChannel(1);
}

void dMenu::on_bChannel2_clicked()
{
    openSettingsChannel(2);
}

void dMenu::on_bChannel3_clicked()
{
    openSettingsChannel(3);
}

void dMenu::on_bChannel4_clicked()
{
    openSettingsChannel(4);
}

void dMenu::on_bApplication_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->nameSubMenu->setText("ПРИЛОЖЕНИЯ");
}

void dMenu::on_bBackApplications_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameSubMenu->setText("НАСТРОЙКИ");
}

void dMenu::on_bUstavki_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->nameSubMenu->setText("УСТАВКИ");
}

void dMenu::on_bBackApplications_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->nameSubMenu->setText("ПРИЛОЖЕНИЯ");
}


void dMenu::on_bDiagnost_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->nameSubMenu->setText("ДИАГНОСТИКА");
}

void dMenu::on_bBackDiagnostika_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    //    ui->nameSubMenu->setText("");
        ui->frameNameSubMenu->setHidden(true);
}

void dMenu::addChannels(QList<ChannelOptions *> channels, QList<Ustavka*> ustavki)
{
    foreach (ChannelOptions * ch, channels) {
        listChannels.append(ch);
    }
    foreach (Ustavka * ust, ustavki) {
        listUstavok.append(ust);
    }
}

void dMenu::selectPageWork()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->nameSubMenu->setText("РАБОТА");
    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bUstavka_1_clicked()
{
    openSettingsChannel(1, 3);
}

void dMenu::on_bUstavka_2_clicked()
{
    openSettingsChannel(2, 3);
}

void dMenu::on_bUstavka_3_clicked()
{
    openSettingsChannel(3, 3);
}

void dMenu::on_bUstavka_4_clicked()
{
    openSettingsChannel(4, 3);
}

void dMenu::on_bSteel1_clicked()
{
    openSettingsChannel(1, 4);
}

void dMenu::on_bSteel2_clicked()
{
    openSettingsChannel(2, 4);
}

void dMenu::on_bSteel3_clicked()
{
    openSettingsChannel(3, 4);
}

void dMenu::on_bSteel4_clicked()
{
    openSettingsChannel(4, 4);
}

void dMenu::on_bBackDateTime_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->nameSubMenu->setText("СИСТЕМА");
}

void dMenu::on_bExpert_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
    ui->nameSubMenu->setText("ЭКСПЕРТ");
    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bBackExpert_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->frameNameSubMenu->setHidden(true);
}

void dMenu::on_bAnaliz_clicked()
{
    UpdateAnalyze();
    ui->stackedWidget->setCurrentIndex(11);
    ui->nameSubMenu->setText("АНАЛИЗ");
}

void dMenu::on_bBackExpert_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->nameSubMenu->setText("РАБОТА");
}

void dMenu::on_bOptions_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
    ui->nameSubMenu->setText("ОПЦИИ");
}

void dMenu::on_bBackSystemOptions_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->nameSubMenu->setText("СИСТЕМА");
}

void dMenu::on_bModeDiagram_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
    ui->nameSubMenu->setText("ОТОБРАЖЕНИЕ");
}

void dMenu::on_bBackOtobrazhenie_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->nameSubMenu->setText("РАБОТА");
}


void dMenu::on_bBackSteel_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->nameSubMenu->setText("ВХОДЫ");
}

void dMenu::on_bExtMemory_clicked()
{
    ui->stackedWidget->setCurrentIndex(15);
    ui->nameSubMenu->setText("НАКОПИТЕЛИ");
    updateDriversWidgets();
}

void dMenu::on_bBackExternalDrive_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->nameSubMenu->setText("СИСТЕМА");
}

void dMenu::on_bSteel_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
    ui->nameSubMenu->setText("СТАЛЬ");
}

void dMenu::updateDriversWidgets()
{
    listDrives.clear();
    int size = flash->getNumDisk();
    for(int i = 0; i < size; i++)
    {
        QString name = flash->getNameDisk(i);
#ifdef Q_OS_WIN
        name.resize(name.size()-1);
#endif
        listDrives.append(name);
    }
    ui->comboDrives->clear();
    ui->comboDrives->addItems(listDrives);
    ui->comboDrives->setCurrentIndex(ui->comboDrives->count() - 1);
}

void dMenu::on_bEditDataTime_clicked()
{
    dateTime = QDateTime::currentDateTime();
    QDateTime local = dateTime;
    ui->timeEdit_h->setTime(local.time());
    ui->timeEdit_h->setDisplayFormat("hh");
    ui->timeEdit_m->setTime(local.time());
    ui->timeEdit_m->setDisplayFormat("mm");
    ui->timeEdit_s->setTime(local.time());
    ui->timeEdit_s->setDisplayFormat("ss");
    ui->dateEdit_d->setDate(local.date());
    ui->dateEdit_d->setDisplayFormat("dd");
    ui->dateEdit_m->setDate(local.date());
    ui->dateEdit_m->setDisplayFormat("MM");
    ui->dateEdit_y->setDate(local.date());
    ui->dateEdit_y->setDisplayFormat("yyyy");
    ui->DateFormat->clear();
    ui->DateFormat->addItems(datestrings);
    ui->DateFormat->setCurrentIndex(dateindex);
    ui->timeformat->clear();
    ui->timeformat->addItems(timestrings);
    ui->timeformat->setCurrentIndex(timeindex);
    ui->stackedWidget->setCurrentIndex(9);
    ui->nameSubMenu->setText("ДАТА/ВРЕМЯ");
}

void dMenu::on_bDateTimeSet_clicked()
{

#ifndef WIN32
    QProcess process;
//    QDateTime newuidate = ui->dateEdit->dateTime();
//    QTime newuitime = ui->timeEdit->time();

    QString newdate = QString::number(dateTime.date().year()) + "-" + QString::number(dateTime.date().month()) + "-" + QString::number(dateTime.date().day()) ;
    QString newtime = dateTime.time().toString();

    process.startDetached("date --set " + newdate);
    process.startDetached("date --set " + newtime); // max freq on
    process.startDetached("hwclock -w");

#endif
    dateindex = ui->DateFormat->currentIndex();
    timeindex = ui->timeformat->currentIndex();
//    ui->timeEdit->setDisplayFormat(timestrings.at(timeindex));
//    ui->dateEdit->setDisplayFormat(datestrings.at(dateindex));

}



void dMenu::on_bResetToDefault_clicked()
{
    //замена файлов настроек
    //настройки каналов
    QFile::remove(pathtooptions + QString(".backup"));
    QFile::rename(pathtooptions, pathtooptions + QString(".backup"));
    QFile::copy(pathtooptionsdef, pathtooptions);
    //системные настройки
    QFile::remove(pathtosystemoptions + QString(".backup"));
    QFile::rename(pathtosystemoptions, pathtosystemoptions + QString(".backup"));
    QFile::copy(pathtosystemoptionsdef, pathtosystemoptions);
    //чтение и применение настроек из новых файлов
    cFileManager::readChannelsSettings(pathtooptions, listChannels, listUstavok);
    updateSystemOptions();
    emit saveButtonSignal();

}





void dMenu::UpdateAnalyze()
{
    double averagechannel_1 , averagechannel_2 , averagechannel_3,averagechannel_4 , sum;

    averagechannel_1 = averagechannel_2 = averagechannel_3 = averagechannel_4 = 0.0;

    averagechannel_1 = mathresolver::dGetAverageValue(Y_coordinates_Chanel_1_archive);
    averagechannel_2 = mathresolver::dGetAverageValue(Y_coordinates_Chanel_2_archive);
    averagechannel_3 = mathresolver::dGetAverageValue(Y_coordinates_Chanel_3_archive);
    averagechannel_4 = mathresolver::dGetAverageValue(Y_coordinates_Chanel_4_archive);

    double minimumchannel_1 = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_1_archive);
    double maximumchannel_1 = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_1_archive);

    double minimumchannel_2 = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_2_archive);
    double maximumchannel_2 = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_2_archive);

    double minimumchannel_3 = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_3_archive);
    double maximumchannel_3 = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_3_archive);

    double minimumchannel_4 = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_4_archive);
    double maximumchannel_4 = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_4_archive);

    ui->analizenameChannel_1->setText( listChannels.at(0)->GetChannelName() \
                                        + " [" + listChannels.at(0)->GetUnitsName() + "]" );
    ui->analizenameChannel_2->setText( listChannels.at(1)->GetChannelName() \
                                       + " [" + listChannels.at(1)->GetUnitsName() + "]" );
    ui->analizenameChannel_3->setText( listChannels.at(2)->GetChannelName() \
                                       + " [" + listChannels.at(2)->GetUnitsName() + "]" );
    ui->analizenameChannel_4->setText( listChannels.at(3)->GetChannelName() \
                                       + " [" + listChannels.at(3)->GetUnitsName() + "]" );

    ui->analizeavgvaluechannel_1->setText(QString::number(averagechannel_1, 'f', 2));
    ui->analizeavgvaluechannel_2->setText(QString::number(averagechannel_2, 'f', 2));
    ui->analizeavgvaluechannel_3->setText(QString::number(averagechannel_3, 'f', 2));
    ui->analizeavgvaluechannel_4->setText(QString::number(averagechannel_4, 'f', 2));

    ui->analizeminvaluechannel_1->setText(QString::number(minimumchannel_1));
    ui->analizeminvaluechannel_2->setText(QString::number(minimumchannel_2));
    ui->analizeminvaluechannel_3->setText(QString::number(minimumchannel_3));
    ui->analizeminvaluechannel_4->setText(QString::number(minimumchannel_4));

    ui->analizemaxvaluechannel_1->setText(QString::number(maximumchannel_1));
    ui->analizemaxvaluechannel_2->setText(QString::number(maximumchannel_2));
    ui->analizemaxvaluechannel_3->setText(QString::number(maximumchannel_3));
    ui->analizemaxvaluechannel_4->setText(QString::number(maximumchannel_4));
}


void dMenu::on_radioButAnalogModes_clicked()
{
    ui->frameAnalogModes->show();
    ui->frameSteelMode->hide();
}

void dMenu::on_radioButSteelModes_clicked()
{
    ui->frameAnalogModes->hide();
    ui->frameSteelMode->show();
}

void dMenu::on_dateEdit_d_up_clicked()
{
    dateTime = dateTime.addDays(1);
    updUiTimeDate(dateTime);
}

void dMenu::on_dateEdit_m_up_clicked()
{
    dateTime = dateTime.addMonths(1);
    updUiTimeDate(dateTime);
}

void dMenu::on_dateEdit_y_up_clicked()
{
    dateTime = dateTime.addYears(1);
    updUiTimeDate(dateTime);
}

void dMenu::on_dateEdit_d_down_clicked()
{
    dateTime = dateTime.addDays(-1);
    updUiTimeDate(dateTime);
}

void dMenu::on_dateEdit_m_down_clicked()
{
    dateTime = dateTime.addMonths(-1);
    updUiTimeDate(dateTime);
}

void dMenu::on_dateEdit_y_down_clicked()
{
    dateTime = dateTime.addYears(-1);
    updUiTimeDate(dateTime);
}

void dMenu::on_timeEdit_h_up_clicked()
{
    dateTime = dateTime.addSecs(3600);
    updUiTimeDate(dateTime);
}

void dMenu::on_timeEdit_m_up_clicked()
{
    dateTime = dateTime.addSecs(60);
    updUiTimeDate(dateTime);
}

void dMenu::on_timeEdit_s_up_clicked()
{
    dateTime = dateTime.addSecs(1);
    updUiTimeDate(dateTime);
}

void dMenu::on_timeEdit_h_down_clicked()
{
    dateTime = dateTime.addSecs(-3600);
    updUiTimeDate(dateTime);
}

void dMenu::on_timeEdit_m_down_clicked()
{
    dateTime = dateTime.addSecs(-60);
    updUiTimeDate(dateTime);
}

void dMenu::on_timeEdit_s_down_clicked()
{
    dateTime = dateTime.addSecs(-1);
    updUiTimeDate(dateTime);
}

void dMenu::updUiTimeDate(QDateTime td)
{
    QDateTime local(td);
    ui->timeEdit_h->setTime(local.time());
    ui->timeEdit_h->setDisplayFormat("hh");
    ui->timeEdit_m->setTime(local.time());
    ui->timeEdit_m->setDisplayFormat("mm");
    ui->timeEdit_s->setTime(local.time());
    ui->timeEdit_s->setDisplayFormat("ss");
    ui->dateEdit_d->setDate(local.date());
    ui->dateEdit_d->setDisplayFormat("dd");
    ui->dateEdit_m->setDate(local.date());
    ui->dateEdit_m->setDisplayFormat("MM");
    ui->dateEdit_y->setDate(local.date());
    ui->dateEdit_y->setDisplayFormat("yyyy");
}




void dMenu::on_bReadSysFromDrive_clicked()
{
//    QProcess process;
    QString src, dest, path;
    path = ui->comboDrives->currentText() + "/" + ui->nameDirOnDrive->text() + "/";
#ifdef Q_OS_LINUX
    path = QString("/media/" + path);
#endif
    src = QString(path + "systemoptions.txt");
    if(QFile::exists(src))
    {
        updateSystemOptions(src);
        qDebug() << "System settings successfully read from the media";
        QString mess = QString("Системные настройки успешно прочитаны с указаного носителя\r\n");
        mess == QString("Нажмите СОХРАНИТЬ, чтобы применить новые настройки");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
    else
    {
        qDebug() << "Error reading the system settings file on the media" << strerror(errno);
        QString mess = QString("Ошибка! Проверьте, пожалуйста, доступность носителя и файлов настроек\r\n");
        mess += QString("Убедитесь, что формат файловой системы носителя соответствует FAT32");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
}

void dMenu::on_bSaveSysToDrive_clicked()
{
    QString src, dest, path;
    path = QString(ui->comboDrives->currentText() + "/" + ui->nameDirOnDrive->text() + "/");
#ifdef Q_OS_LINUX
    path = QString("/media/" + path);
#endif
    src = pathtosystemoptions;
    dest = QString(path + "systemoptions.txt");
    QDir dir;
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }
    QFile::remove(dest + QString(".backup"));
    QFile::rename(dest, dest + QString(".backup"));
    if(QFile::copy(src, dest))
    {
        qDebug() << "The system settings were recorded successfully";
        qDebug() << "Path" << dest.toStdString().c_str();
        path = QString("/" + ui->nameDirOnDrive->text() + "/" + "systemoptions.txt");
        QString mess = QString("Системные настройки успешно сохранены на указаном носителе\r\n");
        mess += QString("Путь к файлу: %1").arg(path);
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
    else
    {
        qDebug() << "Error writing system settings" << strerror(errno);
        QString mess = QString("Ошибка! Проверьте, пожалуйста, доступность носителя");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
}

void dMenu::on_bSaveMesToDrive_clicked()
{
    QString src, dest, path;
    path = QString(ui->comboDrives->currentText() + "/" + ui->nameDirOnDrive->text() + "/");
#ifdef Q_OS_LINUX
    path = QString("/media/" + path);
#endif
    src = pathtomessages;
    dest = QString(path + "Log.txt");
    QDir dir;
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }
    QFile::remove(dest + QString(".backup"));
    QFile::rename(dest, dest + QString(".backup"));
    if(QFile::copy(src, dest))
    {
        qDebug() << "The event log was successfully saved on the media";
        qDebug() << "Path" << dest.toStdString().c_str();
        path = QString("/" + ui->nameDirOnDrive->text() + "/" + "Log.txt");
        QString mess = QString("Журнал событий успешно сохранён на указаном носителе\r\n");
        mess += QString("Путь к файлу: %1").arg(path);
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
    else
    {
        qDebug() << "Error writing event log" << strerror(errno);
        QString mess = QString("Ошибка! Проверьте, пожалуйста, доступность носителя");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
}

void dMenu::on_bSaveChanToDrive_clicked()
{
    QString src, src2, dest, dest2, path;
    path = QString(ui->comboDrives->currentText() + "/" + ui->nameDirOnDrive->text() + "/");
#ifdef Q_OS_LINUX
    path = QString("/media/" + path);
#endif
    src = pathtooptions;
    src2 = pathtosteeloptions;
    dest = QString(path + "options.txt");
    dest2 = QString(path + "steeloptions.txt");
    QDir dir;
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }
    QFile::remove(dest + QString(".backup"));
    QFile::rename(dest, dest + QString(".backup"));
    QFile::remove(dest2 + QString(".backup"));
    QFile::rename(dest2, dest2 + QString(".backup"));
    if(QFile::copy(src, dest) && QFile::copy(src2, dest2))
    {
        qDebug() << "The settings for the inputs and outputs were successfully saved on the media";
        qDebug() << "Path" << dest.toStdString().c_str();
        qDebug() << "Path" << dest2.toStdString().c_str();
        path = QString("/" + ui->nameDirOnDrive->text() + "/");
        QString mess = QString("Настройки входов и выходов успешно сохранены на носителе\r\n");
        mess += QString("Директория файлов: %1").arg(path);
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
    else
    {
        qDebug() << "Error writing settings" << strerror(errno);
        QString mess = QString("Ошибка! Проверьте, пожалуйста, доступность носителя");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
}

void dMenu::on_bReadChanFromDrive_clicked()
{
    QString src, src2, dest, dest2, path;
    path = ui->comboDrives->currentText() + "/" + ui->nameDirOnDrive->text() + "/";
#ifdef Q_OS_LINUX
    path = QString("/media/" + path);
#endif
    src = QString(path + "options.txt");
    src2 = QString(path + "steeloptions.txt");
    if(QFile::exists(src) && QFile::exists(src2))
    {
        updateSystemOptions(src);
        qDebug() << "I / O settings were successfully read from the specified media";
        QString mess = QString("Настройки входов и выходов успешно прочитаны с указаного носителя\r\n");
        mess == QString("Нажмите СОХРАНИТЬ, чтобы применить новые настройки");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
    else
    {
        qDebug() << "Error reading input and output settings" << strerror(errno);
        QString mess = QString("Ошибка! Проверьте, пожалуйста, доступность носителя и файлов настроек\r\n");
        mess += QString("Убедитесь, что формат файловой системы носителя соответствует FAT32");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
}
