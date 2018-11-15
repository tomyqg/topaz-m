#include "menu.h"
#include "ui_menu.h"
#include "defines.h"
#include "filemanager.h"
#include "keyboard.h"
//#include <options.h>
#include <QFile>
#include <QDir>
#include <QString>
#include "usb_flash.h"
#include "Channels/group_channels.h"



#define HEIGHT 768
#define WIDTH 1024
#define TIME_UPDATE DateLabelUpdateTimer
#define DRIVE_UPDATE 500

extern int dateindex;
extern int timeindex;
extern QStringList datestrings, timestrings;
extern QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;
extern QList<ChannelOptions *> listChannels;
extern QList<Ustavka *> listUstavok;
extern QList<cSteel*> listSteel;
extern typeSteelTech steelTech[];
extern cChannelSlotController csc;
extern cSteelController ssc;
extern cSystemOptions systemOptions;  //класс хранения состемных опций
extern cUsbFlash * flash;
extern QList<cGroupChannels*> listGroup;


dMenu::dMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dMenu)
{
//    QTime time;
//    time.start();
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);

    QString ver = CURRENT_VER;
    ui->name->setText(QString("<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">MULTIGRAPH<br/>Ver. " + ver + "</span></p></body></html>"));

    ui->saveButton->setColorText(ColorBlue);
    ui->saveButton->setColorBg(QColor(0xff,0xff,0xff));
    ui->exitButton->setColorText(ColorBlue);
    ui->exitButton->setColorBg(QColor(0xff,0xff,0xff));

    ui->load->setHidden(true);
    mo.setFileName(pathtoloadgif);
    ui->load->setMovie(&mo);
    ui->progressBarLoadFiles->setHidden(true);
//    mo.start();       //замедляет скорость открытия окна на 250 мсек

    connect(&timerLoad, SIGNAL(timeout()), this, SLOT(timeoutLoad()));

    QPixmap pix(pathtologotip);
    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);

    ui->stackedWidget->setCurrentIndex(0);
    ui->frameNameSubMenu->setHidden(true);

    ui->stackedWidgetDIFunc->setCurrentIndex(0);

    connect(&tUpdateTime, SIGNAL(timeout()), this, SLOT(DateUpdate()));
    tUpdateTime.start(TIME_UPDATE);
    DateUpdate();

    connect(flash, SIGNAL(newFlash(int)), this, SLOT(updateDriversWidgets()));

    updateSystemOptions();


    QList<wButtonStyled *> buttons = ui->stackedWidget->findChildren<wButtonStyled *>();
    foreach(wButtonStyled * button, buttons)
    {
        button->setColorText(QColor(0xff,0xff,0xff));
        button->setColorBg(ColorButtonNormal);
        button->setAlignLeft();
    }

    QScroller::grabGesture(ui->scrollAreaUstavki, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaDI, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);

    log = new cLogger(pathtomessages, cLogger::UI);
    log->addMess("Menu > Open ", cLogger::SERVICE);

    //добавить виджеты групп каналов
    addWidgetGroup();
    addWidgetChannels();
//    qDebug() << "Time start dMenu:" << time.elapsed();
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
        keyboard kb(this);
        keyboard::olderprop = object->property("date").toString();
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
    log->addMess("Menu > Cancel", cLogger::SERVICE);
    this->close();
}

void dMenu::on_saveButton_clicked()
{
    mo.start();
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
    log->addMess("Menu > Save", cLogger::SERVICE);
    emit saveButtonSignal();
    //Окно закроется по сигналу таймаута
}

void dMenu::updateSystemOptions(QString path)
{
    cFileManager::readSystemOptionsFromFile(path, &sysOptions);
    ui->arrowscheckBox->setChecked(sysOptions.arrows);
    ui->modeBar->setCurrentIndex((sysOptions.display >> 2));
    ui->modeGraf->setCurrentIndex(sysOptions.display & 3);
    if(ssc.isConnect() && (listChannels.size() > 0))
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

void dMenu::addWidgetUstavki()
{
    clearLayout(ui->verticalLayoutUstavki);

    // генерация виджетов (кнопок) уставок
    int i = 0;
    foreach (Ustavka * u, listUstavok) {
        wButtonStyled * bUstavka = new wButtonStyled(ui->widgetScrollAreaUstavki);
        bUstavka->index = i+1;
        QString nameUstavka = u->getIdentifikator().size() ? (" | " + u->getIdentifikator()) : " ";
        bUstavka->setText("УСТАВКА " + QString::number(bUstavka->index) + nameUstavka);
        bUstavka->setMinimumSize(QSize(0, 70));
        bUstavka->setColorText(QColor(0xff,0xff,0xff));
        bUstavka->setColorBg(ColorButtonNormal);
        bUstavka->setAlignLeft();
        connect(bUstavka, SIGNAL(clicked(int)), this, SLOT(slotOpenSettings(int)));
        ui->verticalLayoutUstavki->addWidget(bUstavka);
        i++;
    }
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutUstavki->addItem(verticalSpacer);
}

void dMenu::addWidgetGroup()
{

    clearLayout(ui->verticalLayoutGroup);

    // генерация виджетов (кнопок) уставок
    int i = 0;
    foreach (cGroupChannels * group, listGroup) {
        wButtonStyled * bGroup = new wButtonStyled(ui->widgetScrollAreaGroup);
        bGroup->index = i+1;
        QString nameGroup = group->groupName.size() ? (" | " + group->groupName) : " ";
        bGroup->setText("ГРУППА " + QString::number(bGroup->index) + nameGroup);
        bGroup->setMinimumSize(QSize(0, 70));
        bGroup->setColorText(QColor(0xff,0xff,0xff));
        bGroup->setColorBg(ColorButtonNormal);
        bGroup->setAlignLeft();
        connect(bGroup, SIGNAL(clicked(int)), this, SLOT(slotOpenGroup(int)));
        ui->verticalLayoutGroup->addWidget(bGroup);
        i++;
    }
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutGroup->addItem(verticalSpacer);
}

void dMenu::addWidgetChannels()
{
    clearLayout(ui->verticalLayoutChannels);

    // генерация виджетов (кнопок) уставок
    int i = 0;
    foreach (ChannelOptions * channel, listChannels) {
        wButtonStyled * bChannel = new wButtonStyled(ui->widgetScrollAreaChannels);
        bChannel->index = i+1;
        QString nameChannel = channel->GetChannelName().size() ? (" | " + channel->GetChannelName()) : " ";
        bChannel->setText("КАНАЛ " + QString::number(bChannel->index) + nameChannel);
        bChannel->setMinimumSize(QSize(0, 70));
        bChannel->setColorText(QColor(0xff,0xff,0xff));
        bChannel->setColorBg(ColorButtonNormal);
        bChannel->setAlignLeft();
        connect(bChannel, SIGNAL(clicked(int)), this, SLOT(slotOpenChannel(int)));
        ui->verticalLayoutChannels->addWidget(bChannel);
        i++;
    }
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutChannels->addItem(verticalSpacer);
}

void dMenu::clearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
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
    addWidgetChannels();
}

void dMenu::openSettingsChannel(int num, int page)
{

    if(page == 0)
    {
        //проверка на наличие такого номера канала
        if((num <= 0) || (num > listChannels.size())) return;

        dialogSetingsChannel = new dSettings(listChannels, num, page);
        dialogSetingsChannel->exec();
        dialogSetingsChannel->deleteLater();
        //sendConfigChannelsToSlave();
    }
    else if(page == 4)
    {
        //проверка на наличие такого номера входной группы
        if((num <= 0) || (num > listSteel.size())) return;
        dialogSetingsChannel = new dSettings(listChannels, num, page);
//        dialogSetingsChannel->addSteel(listSteel.at(num), steelTech);
        dialogSetingsChannel->exec();
        dialogSetingsChannel->deleteLater();

    }
    else if(page == 3)
    {
        if((num <= 0) || (num > listUstavok.size())) return;
        dialogSetingsChannel = new dSettings(listChannels, num, page);
        dialogSetingsChannel->exec();
        dialogSetingsChannel->deleteLater();
        addWidgetUstavki();
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

void dMenu::on_bBackUstavki_clicked()
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

void dMenu::addChannels(QList<ChannelOptions *> channels)
{
    foreach (ChannelOptions * ch, channels) {
        listChannels.append(ch);
    }

    //генерация кнопок уставок
    addWidgetUstavki();

}

void dMenu::slotOpenSettings(int num)
{
    openSettingsChannel(num, 3);
}

void dMenu::slotOpenGroup(int num)
{
    curGroupEdit = num - 1;
    cGroupChannels * group = listGroup.at(curGroupEdit);
    ui->comboGroupState->setCurrentIndex(group->enabled);
    ui->nameGroup->setText(group->groupName);

    //определяем существующие каналы и добавляем в комбобоксы
    QStringList listComboChannels;
    listComboChannels.append("ОТКЛЮЧЕН");
    for(int i = 0; i < listChannels.size(); i++)
    {
        listComboChannels.append("АНАЛОГОВЫЙ ВХОД " + QString::number(i+1));
    }
//    foreach (ChannelOptions * ch, listChannels) {
//        listComboChannels.append(ch->GetChannelName());
//    }
    /*
     * добавить математические каналы и дискретные
    foreach (var, container) {

    }*/

    QList<QComboBox*> listCombo;
    listCombo.append(ui->comboGroupChannel1);
    listCombo.append(ui->comboGroupChannel2);
    listCombo.append(ui->comboGroupChannel3);
    listCombo.append(ui->comboGroupChannel4);
    foreach (QComboBox * combo, listCombo) {
        combo->clear();
        combo->addItems(listComboChannels);
    }

    for(int i = 0; i < listChannels.size(); i++)
    {
        for(int k = 0; k < listCombo.size(); k++)
        {
            if((group->typeInput[k] == 1)
                    && (group->channel[k] == listChannels.at(i)))
            {
                listCombo.at(k)->setCurrentIndex(i+1); ;
            }
        }
    }

    ui->stackedWidget->setCurrentIndex(23);
    ui->nameSubMenu->setText("ГРУППА " + QString::number(num));
}

void dMenu::slotOpenChannel(int num)
{
    openSettingsChannel(num, 0);
}

//void dMenu::onbAddUstavkaClick()
//{

//}

void dMenu::selectPageWork()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->nameSubMenu->setText("РАБОТА");
    ui->frameNameSubMenu->setHidden(false);
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

void dMenu::on_bBackDigitInputSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(16);
    ui->nameSubMenu->setText("ЦИФР. ВХОДА");
}

void dMenu::on_bBackDigitInputs_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->nameSubMenu->setText("ВХОДЫ");
}

void dMenu::on_bAddDigitInput_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bDigitals_clicked()
{
    ui->stackedWidget->setCurrentIndex(16);
    ui->nameSubMenu->setText("ЦИФР. ВХОДА");
}

void dMenu::on_bDigitInput1_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bDigitInput2_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bDigitInput3_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bDigitInput4_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bDigitInput5_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bDigitInput6_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bDigitInput7_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bDigitInput8_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);
    ui->nameSubMenu->setText("НАСТР. ВХОДА");
}

void dMenu::on_bOutputs_clicked()
{
    ui->stackedWidget->setCurrentIndex(18);
    ui->nameSubMenu->setText("ВЫХОДЫ");
}

void dMenu::on_bBackDigitOutputs_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameSubMenu->setText("НАСТРОЙКИ");
}

void dMenu::on_bAddDigitOutput_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bDigitOutput_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bDigitOutput_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bDigitOutput_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bDigitOutput_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bDigitOutput_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bDigitOutput_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bDigitOutput_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bDigitOutput_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("НАСТР. ВЫХОДА");
}

void dMenu::on_bBackDigitOutputSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(18);
    ui->nameSubMenu->setText("ВЫХОДЫ");
}

void dMenu::on_bMath_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
    ui->nameSubMenu->setText("МАТЕМАТИКА");
}

void dMenu::on_bBackMath_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->nameSubMenu->setText("ПРИЛОЖЕНИЯ");
}

void dMenu::on_bGroups_clicked()
{
    ui->stackedWidget->setCurrentIndex(22);
    ui->nameSubMenu->setText("ГРУППЫ");
}

void dMenu::on_bBackGroup_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->nameSubMenu->setText("ПРИЛОЖЕНИЯ");
}

// Vag: удалить, когда будет дейлизовано динамическое добавление
void dMenu::on_bAddGroup_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(23);
    ui->nameSubMenu->setText("ГРУППА 1");
}

void dMenu::on_bBackGroupSetting_clicked()
{
    ui->stackedWidget->setCurrentIndex(22);
    ui->nameSubMenu->setText("ГРУППЫ");
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
    cFileManager::readChannelsSettings(pathtooptions, listChannels);
    updateSystemOptions();
    log->addMess("Reset to default", cLogger::USER);
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
        log->addMess("Read system setting from media", cLogger::STATISTIC);
        qDebug() << "System settings successfully read from the media";
        QString mess = QString("Системные настройки успешно прочитаны с указаного носителя\r\n");
        mess == QString("Нажмите СОХРАНИТЬ, чтобы применить новые настройки");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
    else
    {
        log->addMess("Error read system setting from media", cLogger::ERR);
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
        log->addMess("Save system setting to media", cLogger::STATISTIC);
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
        log->addMess("Error save system setting to media", cLogger::ERR);
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
        log->addMess("Save log to media", cLogger::STATISTIC);
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
        log->addMess("Error save log to media", cLogger::ERR);
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
        log->addMess("Save settings to media", cLogger::STATISTIC);
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
        log->addMess("Error save settings to media", cLogger::ERR);
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
        log->addMess("Read settings from media", cLogger::STATISTIC);
        qDebug() << "I / O settings were successfully read from the specified media";
        QString mess = QString("Настройки входов и выходов успешно прочитаны с указаного носителя\r\n");
        mess == QString("Нажмите СОХРАНИТЬ, чтобы применить новые настройки");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
    else
    {
        log->addMess("Error read settings from media", cLogger::ERR);
        qDebug() << "Error reading input and output settings" << strerror(errno);
        QString mess = QString("Ошибка! Проверьте, пожалуйста, доступность носителя и файлов настроек\r\n");
        mess += QString("Убедитесь, что формат файловой системы носителя соответствует FAT32");
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
}

void dMenu::on_bSaveArchiveToDrive_clicked()
{
    ui->progressBarLoadFiles->setValue(0);
    ui->progressBarLoadFiles->setHidden(false);
    QString src, dest, path;
    QDateTime currTime = QDateTime::currentDateTime();
    int daysArray[4] = {1, 7, 30, 365};
    int days = daysArray[ui->periodArchiveToDrive->currentIndex()];
    QDateTime firstTime = currTime.addDays(-days);
    QString pathArch = QString(pathtoarchivedata);
    QStringList sl = pathArch.split(".");
    int n = sl.size();
    QStringList strlist = pathArch.split("/");
    path = QString(ui->comboDrives->currentText() + "/" \
                   + ui->nameDirOnDrive->text() + "/");
#ifdef Q_OS_LINUX
    path = QString("/media/" + path);
#endif
    QDir dir;
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }
    countArchFiles = 0;
    QTimer * copyTimer = new QTimer;
    connect(copyTimer, SIGNAL(timeout()), this, SLOT(copyArchiveFile()));
    connect(this, SIGNAL(finishCopyArchive()), this, SLOT(copyLastArchFile()));
    connect(this, SIGNAL(finishCopyArchive()), copyTimer, SLOT(deleteLater()));
    copyTimer->start(100);
//    for(int i = 0; i < days; i++)
//    {
//        QString strDay = firstTime.addDays(i).toString("yyMMdd");
//        src = sl.at(n-2) + "_sek_" + strDay + "." + sl.at(n-1);
//        dest = QString(path + "archive_sek_" + strDay + ".dat");    //Vag: изменить
//        QFile::remove(dest);    //удалить с носителя (устарел)
//        QFile arch_sek(src);
//        if(arch_sek.exists())   //проверка наличия такого файла
//        {
//            QFile::copy(src, dest);
//        }
//        int progress = (i*100)/days;
//        ui->progressBarLoadFiles->setValue(progress);
//    }
//    src = sl.at(n-2) + "_sek." + sl.at(n-1);;
//    dest = QString(path + "archive_sek.dat");
//    QFile::remove(dest);    //удалить с носителя (устарел)
//    QFile arch_sek(src);
//    if(arch_sek.exists())   //проверка наличия такого файла
//    {
//        ui->progressBarLoadFiles->setValue(100);
//        QString mess;
//        if(QFile::copy(src, dest))
//        {
//            qDebug() << "The measurement archive was copied to the specified medium";
//            qDebug() << "Path" << dest.toStdString().c_str();
//            mess = QString("Архив измерений "\
//                                   + ui->periodArchiveToDrive->currentText() \
//                                   + " успешно скопирован на указанный носитель\r\n");
//        }
//        else
//        {
//            qDebug() << "Error writing measurement archive" << strerror(errno);
//            mess = QString("Ошибка! Проверьте, пожалуйста, доступность носителя");
//        }
//        mesDialog.showInfo(mess, "Сообщение");
//        mesDialog.setWindowModality(Qt::WindowModal);
//        mesDialog.show();
//    }
//    ui->progressBarLoadFiles->setHidden(true);
}

void dMenu::copyArchiveFile()
{
    QString src, dest, path;
    QDateTime currTime = QDateTime::currentDateTime();
    int daysArray[4] = {1, 7, 30, 365};
    int days = daysArray[ui->periodArchiveToDrive->currentIndex()];
    if(countArchFiles == days)
        emit finishCopyArchive();
    else
    {
        QDateTime firstTime = currTime.addDays(-days);
        QString pathArch = QString(pathtoarchivedata);
        QStringList sl = pathArch.split(".");
        int n = sl.size();
        QStringList strlist = pathArch.split("/");
        path = QString(ui->comboDrives->currentText() + "/" \
                       + ui->nameDirOnDrive->text() + "/");
#ifdef Q_OS_LINUX
        path = QString("/media/" + path);
#endif
        QString strDay = firstTime.addDays(countArchFiles).toString("yyMMdd");
        src = sl.at(n-2) + "_sek_" + strDay + "." + sl.at(n-1);
        dest = QString(path + "archive_sek_" + strDay + ".dat");    //Vag: изменить
        QFile::remove(dest);    //удалить с носителя (устарел)
        QFile arch_sek(src);
        if(arch_sek.exists())   //проверка наличия такого файла
        {
            QFile::copy(src, dest);
        }
        //        int progress = (countArchFiles*100)/days;
        ui->progressBarLoadFiles->setValue((countArchFiles*100)/days);
        countArchFiles++;
    }
}

void dMenu::copyLastArchFile()
{
    QString src, dest, path;
    path = QString(ui->comboDrives->currentText() + "/" \
                   + ui->nameDirOnDrive->text() + "/");
#ifdef Q_OS_LINUX
    path = QString("/media/" + path);
#endif
    QString pathArch = QString(pathtoarchivedata);
    QStringList sl = pathArch.split(".");
    int n = sl.size();
    src = sl.at(n-2) + "_sek." + sl.at(n-1);;
    dest = QString(path + "archive_sek.dat");
    QFile::remove(dest);    //удалить с носителя (устарел)
    QFile arch_sek(src);
    if(arch_sek.exists())   //проверка наличия такого файла
    {
        ui->progressBarLoadFiles->setValue(100);
        QString mess;
        if(QFile::copy(src, dest))
        {
            QString periods[4] = {"day", "week", "month", "year"};
            QString period = periods[ui->periodArchiveToDrive->currentIndex()];
            log->addMess("Save archive(" + period + ") to media", cLogger::STATISTIC);
            qDebug() << "The measurement archive was copied to the specified medium";
            qDebug() << "Path" << dest.toStdString().c_str();
            mess = QString("Архив измерений "\
                                   + ui->periodArchiveToDrive->currentText() \
                                   + " успешно скопирован на указанный носитель\r\n");
        }
        else
        {
            log->addMess("Error save archive to media", cLogger::ERR);
            qDebug() << "Error writing measurement archive" << strerror(errno);
            mess = QString("Ошибка! Проверьте, пожалуйста, доступность носителя");
        }
        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }
    ui->progressBarLoadFiles->setHidden(true);
}


void dMenu::on_comboDigitInputsFunc_currentIndexChanged(int index)
{
    ui->stackedWidgetDIFunc->setCurrentIndex(index);
}

void dMenu::on_digitInoutToOutput_currentIndexChanged(int index)
{
    ui->stackedObjectsMenage->setCurrentIndex(index);
}


void dMenu::on_bAddUstavka_clicked()
{
    Ustavka *ust = new Ustavka(this);
    int i = listUstavok.size();
    ust->setIdentifikator("Limit " + QString::number(i+1));
    listUstavok.append(ust);
    emit newUstavka(i);

    //регенерация кнопок уставок
    addWidgetUstavki();
}



void dMenu::on_bAddGroup_clicked()
{
    cGroupChannels *group = new cGroupChannels();
    int size = listGroup.size();
    if(size >= MAX_NUM_GROUP) return;
    group->enabled = true;
    group->groupName = "Group " + QString::number(size+1);
    listGroup.append(group);

    //регенерация кнопок уставок
    addWidgetGroup();
}

void dMenu::on_bDelGroup_clicked()
{
    if(listGroup.size() > 1)
    {
        listGroup.removeAt(curGroupEdit);
    }
    ui->stackedWidget->setCurrentIndex(22);
    ui->nameSubMenu->setText("ГРУППЫ");
    addWidgetGroup();
}

void dMenu::on_bApplayGroup_clicked()
{
    cGroupChannels * g = listGroup.at(curGroupEdit);
    g->enabled = ui->comboGroupState->currentIndex();
    g->groupName = ui->nameGroup->text();
    QList<QComboBox*> listCombo;
    listCombo.append(ui->comboGroupChannel1);
    listCombo.append(ui->comboGroupChannel2);
    listCombo.append(ui->comboGroupChannel3);
    listCombo.append(ui->comboGroupChannel4);
    for(int i = 0; i < listCombo.size(); i++)
    {

        int indexCh = listCombo.at(i)->currentIndex();
        if(indexCh == 0)
        {
            g->typeInput[i] = 0;
        }
        else if(indexCh <= listChannels.size())
        {
            g->typeInput[i] = 1;
            g->channel[i] = listChannels.at(indexCh-1);
        }
//    else if(index <= listMath.size())
//    {

//    }
    }

    ui->stackedWidget->setCurrentIndex(22);
    ui->nameSubMenu->setText("ГРУППЫ");
    addWidgetGroup();
}
