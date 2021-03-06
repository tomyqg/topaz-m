#include "menu.h"
#include "ui_menu.h"
#include "defines.h"
#include "filemanager.h"
#include "keyboard.h"
#include "expert_access.h"
//#include <options.h>
#include <QFile>
#include <QDir>
#include <QString>
#include "assert.h"
#include "usb_flash.h"
#include "Channels/group_channels.h"
#include "Channels/freq_channel.h"
#include "device_slot.h"
#include "ip_controller.h"
#include "qtcsv-master/include/qtcsv/writer.h"
#include "qtcsv-master/include/qtcsv/stringdata.h"

#define DEBUG_UPDATE_SOFT

#define HEIGHT 768
#define WIDTH 1024
#define TIME_UPDATE DateLabelUpdateTimer
#define TIME_UPDATE_DEVICE_UI   500
#define DRIVE_UPDATE 500
#define TIME_UPD_DIAGNOSTIC     500
#define TIME_UPD_ANALIZ         500

//cExpertAccess access;

//extern int dateindex;
//extern int timeindex;
extern QStringList datestrings, timestrings;
//extern QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;
extern QList<cDevice*> listDevice;
extern QList<ChannelOptions *> listChannels;
extern QList<Ustavka *> listUstavok;
extern QList<cSteel*> listSteel;
extern QList<cRelay*> listRelais;
extern typeSteelTech steelTech[];
extern cSystemOptions systemOptions;  //класс хранения состемных опций
extern cUsbFlash * flash;
extern QList<cGroupChannels*> listGroup;
extern QList<cMathChannel*> listMath;
extern QList<cFreqChannel*> listFreq; //список частотных каналов
extern cIpController * ethernet;
extern QMutex mSysOpt;
extern QMutex mListUstvok;
extern QMutex mListMath;
extern QMutex mListChannel;
extern QMutex mListDev;
extern QMutex mListFreq;
extern QMutex mListRelay;


dMenu::dMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dMenu)
{
//    QTime time;
//    time.start();
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);

    updateVer();

    ui->frameModeAccess->hide();

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

    connect(&tUpdateDiagnostic, SIGNAL(timeout()), this, SLOT(updateLabelDiagnostic()));
    connect(&tUpdateDiagnostic, SIGNAL(timeout()), this, SLOT(updateLabelModeling()));
    connect(&tUpdateDiagnostic, SIGNAL(timeout()), this, SLOT(updateMathResultFormula()));
    tUpdateDiagnostic.start(TIME_UPD_DIAGNOSTIC);

    //периодические обновления виджетов информации о платах
    connect(&tUpdateDeviceUI, SIGNAL(timeout()), this, SLOT(updateDevicesUI()));
    connect(&tUpdateDeviceUI, SIGNAL(timeout()), this, SLOT(updateDeviceMain()));
    tUpdateDeviceUI.start(TIME_UPDATE_DEVICE_UI);
    curDiagnostDevice = 0;

    connect(flash, SIGNAL(newFlash(int)), this, SLOT(updateDriversWidgets()));

    updateSystemOptions();


    // стили для всех ComboBox
    QList<QComboBox *> combos = ui->stackedWidget->findChildren<QComboBox *>();
    foreach(QComboBox * combo, combos)
    {
        QListView *view = new QListView(combo);
        view->setStyleSheet("QListView::item{height: 50px;}\n QListView::item:selected{background-color:rgb(44, 61, 77);}");
        QFont font;
        font.setFamily(QStringLiteral("Open Sans"));
        font.setPointSize(14);
        view->setFont(font);
        QScroller::grabGesture(view->viewport(), QScroller::LeftMouseButtonGesture);
        combo->setView(view);
        QString comboStyle = combo->styleSheet();
        QString styleAppend = "\nQComboBox::drop-down {\n	width:0px;\n }";
        combo->setStyleSheet(comboStyle + styleAppend);
//        view->deleteLater();
    }

    QList<wButtonStyled *> buttons = ui->stackedWidget->findChildren<wButtonStyled *>();
    foreach(wButtonStyled * button, buttons)
    {
        button->setColorText(QColor(0xff,0xff,0xff));
        button->setColorBg(ColorButtonNormal);
        button->setAlignLeft();
    }

    // стили для всех QScrollBar
    QString styleScrollBars = "QScrollBar:vertical {              \n        background:rgb(179, 179, 179);\n        margin: 0px 0px 0px 0px;\n		border-radius: 0px;\n		width:30px; \n    }\nQScrollBar::handle:vertical {\n       background:rgb(77, 77, 77);\n        min-height: 50px;\n		border-radius: 0px;\n    }\nQScrollBar::add-line:vertical {\n       background:rgb(179, 179, 179);\n        height: 0px;\n        subcontrol-position: bottom;\n        subcontrol-origin: margin;\n		border-radius: 0px;\n    }\n\nQScrollBar::sub-line:vertical {\n        background:rgb(77, 77, 77);\n        height: 0 px;\n        subcontrol-position: top;\n        subcontrol-origin: margin;\n		border-radius: 0px;\n    }\nQScrollBar::up-arrow:vertica, QScrollBar::down-arrow:verticall {\n        background:rgb(77, 77, 77);\n        height: 0px;\n		border-radius: 0px;\n    }\n\nQScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n    background: none;\n}\n";
    QList<QScrollBar *> scrolls = ui->stackedWidget->findChildren<QScrollBar *>();
    foreach(QScrollBar * scroll, scrolls)
    {
        scroll->setStyleSheet(styleScrollBars);
    }

    QScroller::grabGesture(ui->scrollAreaUstavki->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaDI->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaDigitalOutputs->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaModeling->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaChannels->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaDevices->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaMeasures->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaFreq->viewport(), QScroller::LeftMouseButtonGesture);



    log = new cLogger(pathtomessages, cLogger::UI);

    //добавить виджеты групп каналов
    addWidgetGroup();
    addWidgetChannels();
    addWidgetFreqs();

    light = systemOptions.brightness;
    ui->volLight->setText(QString::number(light));
    ui->progressLight->setValue(light);

    QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->listDeviceErrors->viewport(), QScroller::LeftMouseButtonGesture);

//    connect(ethernet, SIGNAL(signalErrIp()), this, SLOT(slotIpErr()));
//    connect(ethernet, SIGNAL(signalStatus(bool,bool)), \
//            this, SLOT(slotUpdateEthernetStatus(bool,bool)));

    // Подключение редактируемых с клавиатуры текстовых полей QLineEdit
    listEditableLineText << ui->ipAddr \
                         << ui->netMask \
                         << ui->gateWay \
                         << ui->nameGroup \
                         << ui->nameMath \
                         << ui->nameFreqChannel \
                         << ui->formulaMath \
                         << ui->unitMath \
                         << ui->modbusSlavePort;

    foreach (QWidget * w, listEditableLineText) {
        w->installEventFilter(this);
    }

//    ui->ipAddr->installEventFilter(this);
//    ui->netMask->installEventFilter(this);
//    ui->gateWay->installEventFilter(this);
//    ui->nameGroup->installEventFilter(this);
//    ui->nameMath->installEventFilter(this);
//    ui->formulaMath->installEventFilter(this);

    // список виджетов, которые нужно скрывать от лишних глаз
    listWidgetsExpert << ui->frameModeAccess \
                      << ui->bSystem \
                      << ui->bTypeConnect \
                      << ui->bEditDataTime \
                      << ui->bProtect \
                      << ui->bExtMemory \
                      << ui->bMessages \
                      << ui->bOptions \
                      << ui->bUstavki \
                      << ui->bLogEvents \
                      << ui->bModeling \
                      << ui->bEthernet \
                      << ui->bSeraInterface \
                      << ui->bModbusSlave \
                      << ui->bEthernetIP \
                      << ui->bProfibus \
                      << ui->bProfinet;
                      //<< ui->bTypeMultigraph;
    listWidgetsAdmin << ui->bTypeMultigraph;

    // скрыть эти выджеты(кнопки) изначально
    changeVisibleWidgets();

    // обновить состояния виджетов интерфейсов
    updateInterfaceWidgets();

//    connect(ui->arrowscheckBox, SIGNAL(clicked(bool)), this, SLOT(clickCheckBox()));

//    qDebug() << "Time start dMenu:" << time.elapsed();

}

void dMenu::updateVer()
{
    QString ver = CURRENT_VER;
    QString name = "MULTIGRAPH";
    if(systemOptions.typeMultigraph == cSystemOptions::Multigraph_Steel)
    {
        ver = ver + "S";
        name = name + "<br>STEEL";
    }
    ui->name->setText(QString("<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">" + name + "<br/>Ver. " + ver + "</span></p></body></html>"));
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
        keyboard::olderprop = object->property("date").toString();
        keyboard kb(this);
        kb.setModal(true);
        kb.exec();
        object->setProperty("text",kb.getcustomstring() );
        object->setProperty("value",kb.getcustomstring() );
    }
#endif

    if ( (event->type() == QEvent::MouseButtonRelease) && \
         (object->objectName().contains("bModeling")))
    {
        if(QString::fromLatin1(object->metaObject()->className()) == "QPushButton")
        {
            QPushButton * widget = (QPushButton*)object;
            widget->setStyleSheet("background-color: rgb(230, 230, 230);\n"
                                  "color: rgb(0, 0, 0);\n"
                                  "border-radius: 0px;");
        }
        QStringList listParam = object->objectName().split('_');
        int num = listParam.at(2).toInt();
        mListRelay.lock();
        if(listParam.at(1) == "On") listRelais.at(num)->setState(true);
        else if(listParam.at(1) == "Off") listRelais.at(num)->setState(false);
        mListRelay.unlock();
        //возвращать цвет кнопки

    }

    if ( (event->type() == QEvent::MouseButtonPress) && \
         (object->objectName().contains("bModeling")))
    {
        if(QString::fromLatin1(object->metaObject()->className()) == "QPushButton")
        {
            QPushButton * widget = (QPushButton*)object;
            //менять цвет кнопки
            widget->setStyleSheet("background-color: rgb(180, 180, 180);\n"
                                  "color: rgb(0, 0, 0);\n"
                                  "border-radius: 0px;");
        }
    }

#ifndef Q_OS_WIN
    if ( (event->type() == QEvent::MouseButtonPress) && \
         (object->property("enabled").toString() == "true"))
    {
        // проверка виджетов на разрешение использовать клавиатуру для ввода
        foreach (QWidget * w, listEditableLineText) {
            if(object->objectName() == w->objectName()){
                //окрывать клавиатуру только, если это QLineEdit
                if(QString::fromLatin1(object->metaObject()->className()) == "QLineEdit")
                {
                    keyboard::olderprop = object->property("text").toString();
                    keyboard kb(this);
                    kb.setModal(true);
                    kb.exec();
                    object->setProperty("text",kb.getcustomstring() );
                }
                break;
            }
        }
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
    cExpertAccess::resetAccess();
    // Изменить видимость виджетов в соответствии с режимом доступа
    changeVisibleWidgets();

    mListDev.lock();
    foreach (cDevice * device, listDevice) {
        device->setMode(Device_Mode_Regular);
    }
    mListDev.unlock();
    foreach (cSteel * steel, listSteel) {
        steel->mode = Device_Mode_Regular;
    }
    this->close();
}


void dMenu::on_saveButton_clicked()
{
    mo.start();
    ui->load->setHidden(false);
//     засекаем время записи настроек в файл или ждать сигнал о завершении
    timerLoad.start(3000);
    //  запись файла //
    mSysOpt.lock();
    systemOptions.arrows = ui->arrowscheckBox->checkState();
    systemOptions.display = ui->modeGraf->currentIndex();
    systemOptions.display += (ui->modeBar->currentIndex() << 2);
    systemOptions.autoscale = ui->autoscalecheckbox->isChecked();
    systemOptions.brightness = light;

    // Изменить тип плат 4AI/STEEL если тип прибора поменялся
    if(systemOptions.typeMultigraph != (cSystemOptions::TypeMultigraphEnum)ui->comboTypeMultigraph->currentIndex())
    {
        foreach (cDevice * dev, listDevice) {
            if(dev->getOnline() \
                    && ((dev->deviceType == deviceTypeEnum::Device_4AI) \
                    || (dev->deviceType == deviceTypeEnum::Device_STEEL)))
            {
                Transaction t(Transaction::W, dev->getSlot());
                t.offset = cRegistersMap::getOffsetByName("configDeviceType");
                if((cSystemOptions::TypeMultigraphEnum)ui->comboTypeMultigraph->currentIndex() == cSystemOptions::Multigraph_Steel)
                {
                    t.volInt = deviceTypeEnum::Device_STEEL;
                }
                else if((cSystemOptions::TypeMultigraphEnum)ui->comboTypeMultigraph->currentIndex() == cSystemOptions::Multigraph)
                {
                    t.volInt = deviceTypeEnum::Device_4AI;
                }
                emit signalToWorker(t);
//                t.dir = Transaction::R;
//                t.offset = cRegistersMap::getOffsetByName("deviceType");
//                emit signalToWorker(t);
                dev->deviceType = Device_None;

            }
        }
    }
    systemOptions.typeMultigraph = (cSystemOptions::TypeMultigraphEnum)ui->comboTypeMultigraph->currentIndex();

    if(ui->stackedWidget->currentIndex() == 9)
    {
        systemOptions.timeindex = ui->timeformat->currentIndex();
        systemOptions.dateindex = ui->DateFormat->currentIndex();
    }
    systemOptions.extModbus.type = (cSystemOptions::TypeExtModbusInterface)ui->comboModbusSlaveInterface->currentIndex();
    systemOptions.extModbus.adress = ui->modbusSlaveAdress->value();
    systemOptions.extModbus.baud = systemOptions.listBauds.at(ui->comboModbusSlaveBaud->currentIndex());
    systemOptions.extModbus.parity = ui->comboModbusSlaveParity->currentIndex();
    systemOptions.extModbus.dataBits = ui->modbusSlaveDataBits->value();
    systemOptions.extModbus.stopBits = ui->modbusSlaveStopBits->value();
    systemOptions.extModbus.port = ui->modbusSlavePort->text().toInt();
    mSysOpt.unlock();

    updateVer();

    if(ui->stackedWidget->currentIndex() == 32)
    {
        on_bApplayMath_clicked();
        ui->stackedWidget->setCurrentIndex(32);
    }

    cFileManager::writeSystemOptionsToFile(pathtosystemoptions, &systemOptions);
    cFileManager::writeChannelsSettings(pathtooptions);
    log->addMess("Menu > Save", cLogger::SERVICE);
    cExpertAccess::resetAccess();
    // Изменить видимость виджетов в соответствии с режимом доступа
    changeVisibleWidgets();
    mListDev.lock();
    foreach (cDevice * device, listDevice) {
        device->setMode(Device_Mode_Regular);
    }
    mListDev.unlock();
    foreach (cSteel * steel, listSteel) {
        steel->mode = Device_Mode_Regular;
    }



    emit saveButtonSignal();

//    // ожидание таймера
//    while(timerLoad.remainingTime);
//    timeoutLoad();

}



void dMenu::updateSystemOptions(QString path)
{
    cFileManager::readSystemOptionsFromFile(path, &systemOptions);

    ui->arrowscheckBox->setChecked(systemOptions.arrows);
    ui->modeBar->setCurrentIndex((systemOptions.display >> 2) % ui->modeBar->count());
    ui->modeGraf->setCurrentIndex(systemOptions.display & 3);
//    if((listSteel.size() > 0) && (listChannels.size() > 0))
//    {
//        ui->groupBoxTypePribor->show();
//        if(sysOptions.display == cSystemOptions::Steel)
//        {
//            ui->radioButAnalogModes->setChecked(false);
//            ui->radioButSteelModes->setChecked(true);
//            ui->frameAnalogModes->hide();
////            ui->frameSteelMode->show();
//        } else {
//            ui->radioButSteelModes->setChecked(false);
//            ui->radioButAnalogModes->setChecked(true);
////            ui->frameSteelMode->hide();
//        }
//    }
//    else
//    {
//        ui->groupBoxTypePribor->hide();
//    }
    ui->autoscalecheckbox->setChecked(systemOptions.autoscale);
    ui->comboTypeMultigraph->setCurrentIndex((int)systemOptions.typeMultigraph);
}

void dMenu::addWidgetUstavki()
{
    clearLayout(ui->verticalLayoutUstavki);

    // генерация виджетов (кнопок) уставок
    int i = 0;
    mListUstvok.lock();
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
    mListUstvok.unlock();
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutUstavki->addItem(verticalSpacer);
}



void dMenu::addWidgetGroup()
{

    clearLayout(ui->verticalLayoutGroup);

    // генерация виджетов (кнопок) групп
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
    mListChannel.lock();
    foreach (ChannelOptions * channel, listChannels) {
        if(channel->enable)
        {
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
    }
    mListChannel.unlock();
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutChannels->addItem(verticalSpacer);
}

void dMenu::addWidgetFreqs()
{
    clearLayout(ui->verticalLayoutFreq);

    int i = 0;
    mListFreq.lock();
    foreach (cFreqChannel * freq, listFreq) {
        if(freq->enable)
        {
            wButtonStyled * bFreq = new wButtonStyled(ui->widgetScrollAreaFreq);
            bFreq->index = i+1;
            QString nameChannel = freq->GetChannelName().size() ? (" | " + freq->GetChannelName()) : " ";
            bFreq->setText("КАНАЛ " + QString::number(bFreq->index) + nameChannel);
            bFreq->setMinimumSize(QSize(0, 70));
            bFreq->setColorText(QColor(0xff,0xff,0xff));
            bFreq->setColorBg(ColorButtonNormal);
            bFreq->setAlignLeft();
            connect(bFreq, SIGNAL(clicked(int)), this, SLOT(slotOpenFreq(int)));
            ui->verticalLayoutFreq->addWidget(bFreq);
            i++;
        }
    }
    mListFreq.unlock();
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutFreq->addItem(verticalSpacer);
}

void dMenu::addWidgetDigitOutputs()
{
    clearLayout(ui->verticalLayoutDigitalOutputs);

    int i = 0;
    mListRelay.lock();
    foreach (cRelay * relay, listRelais) {
        if(relay->enable)
        {
            wButtonStyled * bOutput = new wButtonStyled(ui->scrollAreaWidgetDigitalOutputs);
            bOutput->index = i+1;
            bOutput->setText("ВЫХОД " + QString::number(bOutput->index));
            bOutput->setMinimumSize(QSize(0, 70));
            bOutput->setColorText(QColor(0xff,0xff,0xff));
            bOutput->setColorBg(ColorButtonNormal);
            bOutput->setAlignLeft();
            connect(bOutput, SIGNAL(clicked(int)), this, SLOT(slotOpenDigitOutput(int)));
            ui->verticalLayoutDigitalOutputs->addWidget(bOutput);
            i++;
        }
    }
    mListRelay.unlock();
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutDigitalOutputs->addItem(verticalSpacer);
}

void dMenu::addWidgetMeasures()
{
    clearLayout(ui->verticalLayoutMeasures);
    listLabelDiagnostic.clear();
    listLabelDiagnosticFreq.clear();
    // генерация виджетов
    if(systemOptions.display == cSystemOptions::Steel)
    {
        listComboTypeTermo.clear();
        listLabelTempAnalizSteel.clear();
        listLabelCj.clear();
        listLabelEmfpAnalizSteel.clear();

        for(int i = 0; i < listSteel.size(); i++)
        {
            if(!listSteel.at(i)->enable) return;
            QFont font1;
            font1.setFamily(QStringLiteral("Open Sans"));
            font1.setPointSize(14);
            QFont font4;
            font4.setFamily(QStringLiteral("Open Sans"));
            font4.setPointSize(20);

            QFrame * frameAnalizSteel = new QFrame(ui->widgetScrollAreaMeasures);
            frameAnalizSteel->setFrameShape(QFrame::NoFrame);
            frameAnalizSteel->setFrameShadow(QFrame::Raised);
            QGridLayout * gridLayout_24 = new QGridLayout(frameAnalizSteel);
            gridLayout_24->setHorizontalSpacing(0);
            gridLayout_24->setVerticalSpacing(9);
            QLabel * analizeLblGroupSteel = new QLabel(frameAnalizSteel);
            analizeLblGroupSteel->setFont(font4);
            gridLayout_24->addWidget(analizeLblGroupSteel, 0, 0, 1, 1);

            QLabel * analizeLblTemp = new QLabel(frameAnalizSteel);
            analizeLblTemp->setFont(font1);
            gridLayout_24->addWidget(analizeLblTemp, 2, 0, 1, 1);

            QLabel * analizeLblCj = new QLabel(frameAnalizSteel);
            analizeLblCj->setFont(font1);
            gridLayout_24->addWidget(analizeLblCj, 3, 0, 1, 1);

            QLabel * analizeSteelCj = new QLabel(frameAnalizSteel);
            analizeSteelCj->setMinimumSize(QSize(131, 31));
            analizeSteelCj->setMaximumSize(QSize(185, 45));
            analizeSteelCj->setFont(font1);
            analizeSteelCj->setStyleSheet(QLatin1String("background-color:rgb(44, 61, 77);\n"
                                                         "color: rgb(255 , 255, 255);\n"
                                                         "border-radius: 0px;"));
            analizeSteelCj->setAlignment(Qt::AlignCenter);
            gridLayout_24->addWidget(analizeSteelCj, 3, 1, 1, 1);

            QLabel * analizeLblEmf = new QLabel(frameAnalizSteel);
            analizeLblEmf->setFont(font1);
            gridLayout_24->addWidget(analizeLblEmf, 4, 0, 1, 1);

            QLabel * analizeSteelEmf = new QLabel(frameAnalizSteel);
            analizeSteelEmf->setMinimumSize(QSize(131, 31));
            analizeSteelEmf->setMaximumSize(QSize(185, 45));
            analizeSteelEmf->setFont(font1);
//            analizeSteelEmf->setStyleSheet(QLatin1String("background-color:rgb(44, 61, 77);\n"
            analizeSteelEmf->setStyleSheet("background-color:" + QString(ColorToTextRgb(ColorCh3)) + ";\n"
                                                         "color: rgb(255 , 255, 255);\n"
                                                         "border-radius: 0px;");
            analizeSteelEmf->setAlignment(Qt::AlignCenter);
            gridLayout_24->addWidget(analizeSteelEmf, 4, 1, 1, 1);

            QLabel * analizeLblNameTech = new QLabel(frameAnalizSteel);
            analizeLblNameTech->setMinimumSize(QSize(131, 31));
            analizeLblNameTech->setMaximumSize(QSize(185, 45));
            analizeLblNameTech->setFont(font1);
            analizeLblNameTech->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
                                                            "color: rgb(0, 0, 0);\n"
                                                            "border-radius: 0px;\n"
                                                            ""));
            analizeLblNameTech->setAlignment(Qt::AlignCenter);
            gridLayout_24->addWidget(analizeLblNameTech, 0, 1, 1, 1);

            QLabel * analizeLblTmp = new QLabel(frameAnalizSteel);
            analizeLblTmp->setMinimumSize(QSize(131, 31));
            analizeLblTmp->setMaximumSize(QSize(185, 45));
            analizeLblTmp->setFont(font1);
            analizeLblTmp->setStyleSheet(QLatin1String("	background-color: rgb(21, 159, 133);\n"
                                                       "	color: rgb(255, 255, 255);\n"
                                                       "	border-radius: 0px;"));
            analizeLblTmp->setAlignment(Qt::AlignCenter);
            gridLayout_24->addWidget(analizeLblTmp, 2, 1, 1, 1);
            ui->verticalLayoutMeasures->addWidget(frameAnalizSteel);

            analizeLblGroupSteel->setText("ВХОДНАЯ ГРУППА" + QString::number(i+1));
//            labelTypeTermo->setText("ТЕРМОПАРА");
            analizeLblCj->setText("ХОЛОДНЫЙ СПАЙ, °C");
            analizeLblTemp->setText("ТЕМПЕРАТУРА, °C");
            analizeLblEmf->setText("EMF, мВ");
            cSteel * steel = listSteel.at(i);
            analizeLblNameTech->setText(QString(steel->technology->name));
            analizeSteelCj->setText(QString::number(steel->cj));
            analizeLblTmp->setText(QString::number(steel->temp));
            analizeSteelEmf->setText(QString::number(steel->eds));

            listLabelTempAnalizSteel.append(analizeLblTmp);
            listLabelCj.append(analizeSteelCj);
            listLabelEmfpAnalizSteel.append(analizeSteelEmf);
//            listComboTypeTermo.append(typeTermoCouple);
        }
    }
    else
    {
        int i = 0;
        mListChannel.lock();
        foreach (ChannelOptions * channel, listChannels) {

            if(channel->enable)
            {
                QLabel * labelValMeasure = addFramMeasureDiagnostic("АНАЛОГ. КАНАЛ " + QString::number(channel->getNum()),\
                                                                    channel->GetChannelName(),\
                                                                    QString::number(channel->GetCurrentChannelValue()),\
                                                                    channel->GetUnitsName());
                listLabelDiagnostic.append(labelValMeasure);
            }
            i++;
        }
        mListChannel.unlock();

        i = 0;
        mListFreq.lock();
        foreach (cFreqChannel * channel, listFreq) {
            if(channel->enable)
            {
                QLabel * labelValMeasure = addFramMeasureDiagnostic("ЧАСТОТ. КАНАЛ " + QString::number(channel->getNum()),\
                                                                    channel->GetChannelName(),\
                                                                    QString::number(channel->GetCurrentChannelValue()),\
                                                                    channel->getUnit());
                listLabelDiagnosticFreq.append(labelValMeasure);
            }
            i++;
        }
        mListFreq.unlock();

    }

    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutMeasures->addItem(verticalSpacer);
}

QLabel * dMenu::addFramMeasureDiagnostic(QString lblParam, QString lblName, QString lblValue, QString lblUnit)
{
    QFont font2;
    font2.setFamily(QStringLiteral("Open Sans"));
    font2.setPointSize(14);

    QFrame * frameMeasure1 = new QFrame(ui->widgetScrollAreaMeasures);
    frameMeasure1->setFrameShape(QFrame::NoFrame);
    frameMeasure1->setFrameShadow(QFrame::Raised);

    QHBoxLayout * horizontalLayout_3 = new QHBoxLayout(frameMeasure1);

    QLabel * labelMeasure1 = new QLabel(frameMeasure1);
    labelMeasure1->setFont(font2);
    labelMeasure1->setText(lblParam);
    horizontalLayout_3->addWidget(labelMeasure1);

    QLabel * labelNameMeasure1 = new QLabel(frameMeasure1);
    labelNameMeasure1->setFont(font2);
    labelNameMeasure1->setAlignment(Qt::AlignCenter);
    labelNameMeasure1->setText(lblName);
    horizontalLayout_3->addWidget(labelNameMeasure1);
    ui->verticalLayoutMeasures->addWidget(frameMeasure1);

    QLabel * volMeasure1 = new QLabel(frameMeasure1);
    volMeasure1->setMinimumSize(QSize(131, 31));
    volMeasure1->setMaximumSize(QSize(185, 45));
    volMeasure1->setFont(font2);
    volMeasure1->setStyleSheet(QLatin1String("	background-color: rgb(21, 159, 133);\n"
                                             "	color: rgb(255, 255, 255);\n"
                                             "	border-radius: 0px;"));
    volMeasure1->setAlignment(Qt::AlignCenter);
    volMeasure1->setText(lblValue);
//    listLabelDiagnosticFreq.append(volMeasure1);
    horizontalLayout_3->addWidget(volMeasure1);

    QLabel * labelMesMeasure1 = new QLabel(frameMeasure1);
    labelMesMeasure1->setFont(font2);
    labelMesMeasure1->setAlignment(Qt::AlignCenter);
    labelMesMeasure1->setText(lblUnit);
    horizontalLayout_3->addWidget(labelMesMeasure1);

    return volMeasure1;
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



void dMenu::setBrightness(int l)
{
#ifndef Q_OS_WIN32
    QFile fileBright("/sys/class/backlight/pwm-backlight/brightness");
    fileBright.open(QIODevice::WriteOnly);
    QTextStream out(&fileBright);
    out << l;
    fileBright.close();
#endif
}

void dMenu::slotUpdateEthernetStatus(bool online, bool enable)
{
//    if(enable)
//    {
//       ui->bToConnect->setText("ПОДКЛЮЧИТЬСЯ");
//    }
//    else
//    {
//        ui->bToConnect->setText("ОТКЛЮЧИТЬСЯ");
//    }
}

void dMenu::slotIpErr()
{
    ui->bToConnect->setText("ПОДКЛЮЧИТЬСЯ");
}

void dMenu::timeoutLoad()
{
    timerLoad.stop();
    mo.stop();
    ui->load->setHidden(true);
    this->close();
}

void dMenu::on_bWork_clicked()
{
    if(systemOptions.display == cSystemOptions::Steel) ui->bModeDiagram->hide();
    else ui->bModeDiagram->show();
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
    if(systemOptions.display == cSystemOptions::Steel) ui->bApplication->hide();
    else ui->bApplication->show();
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
    if(systemOptions.display == cSystemOptions::Steel)
    {
        ui->stackedWidget->setCurrentIndex(14);
        ui->nameSubMenu->setText("СТАЛЬ");
    }
    else
    {
        ui->bSteel->hide();
        ui->stackedWidget->setCurrentIndex(4);
        ui->nameSubMenu->setText("ВХОДЫ");
    }
//    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bSystem_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->nameSubMenu->setText("СИСТЕМА");
}

//void dMenu::clickCheckBox()
//{
//    if (widget->inherits("QCheckBox")) {
//        QCheckBox *cbox = static_cast<QCheckBox *>(widget);
////        button->toggle();
//        cbox->setObjectName("Bla-bla-bla");
//      }
//}


void dMenu::DateUpdate() // каждую секунду обновляем значок времени
{



    QDateTime local(QDateTime::currentDateTime());
    QString str = "<html><head/><body><p align=\"center\"><span style=\" font-size:22pt; color:#ffffff;\">" \
                  + local.time().toString(timestrings.at(systemOptions.timeindex)) + \
                  "</span><span style=\" color:#ffffff;\"><br/></span>" \
                  "<span style=\" font-size:17pt; color:#ffffff;\">" \
                  + local.date().toString(datestrings.at(systemOptions.dateindex)) + \
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

        dialogSetingsChannel = new dSettings(listChannels, num);
        dialogSetingsChannel->exec();
        dialogSetingsChannel->deleteLater();
        addWidgetChannels();
        //sendConfigChannelsToSlave();
    }
    else if(page == 4)
    {
        //проверка на наличие такого номера входной группы
        if((num <= 0) || (num > listSteel.size())) return;
        dialogSetingsChannel = new dSettings(listChannels, num, page);
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
    addWidgetUstavki();
}

void dMenu::on_bBackUstavki_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->nameSubMenu->setText("ПРИЛОЖЕНИЯ");
}


void dMenu::on_bDiagnost_clicked()
{
    //if(systemOptions.display == cSystemOptions::Steel) ui->bMeasuredValue->hide();
    //else ui->bMeasuredValue->show();

    if(systemOptions.display == cSystemOptions::Steel)
    {
        ui->bAnaliz->hide();
        // включаем режим проверки метрологии на всех платах
        // пока только для стали
        mListDev.lock();
        foreach (cDevice * device, listDevice) {
            device->setMode(Device_Mode_Metrological);
        }
        mListDev.unlock();
        foreach (cSteel * steel, listSteel) {
            steel->mode = Device_Mode_Metrological;
        }
    }
    else ui->bAnaliz->show();
    ui->stackedWidget->setCurrentIndex(8);
    ui->nameSubMenu->setText("ДИАГНОСТИКА");
    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bBackDiagnostika_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
//        ui->nameSubMenu->setText("");
    ui->frameNameSubMenu->setHidden(true);
}

void dMenu::addChannels(QList<ChannelOptions *> channels)
{
    mListChannel.lock();
    foreach (ChannelOptions * ch, channels) {
        listChannels.append(ch);
    }
    mListChannel.unlock();

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
    mListChannel.lock();
    for(int i = 0; i < listChannels.size(); i++)
    {
        QString nameCh = listChannels.at(i)->GetChannelName();
        QString stateCh = (listChannels.at(i)->enable ? "ВКЛ." : "ОТКЛ.");
        listComboChannels.append(nameCh + " (A" + QString::number(i+1) + ") | " + stateCh);
    }
    mListChannel.unlock();

    //математические каналы
    mListMath.lock();
    for (int i = 0; i < listMath.size(); i++) {
        listComboChannels.append(listMath.at(i)->getName() + " (M" + QString::number(i+1) + ")");
    }
    mListMath.unlock();

    //частотные каналы
    mListFreq.lock();
    for (int i = 0; i < listFreq.size(); i++) {
        QString nameCh = listFreq.at(i)->GetChannelName();
        QString stateCh = (listFreq.at(i)->enable ? "ВКЛ." : "ОТКЛ.");
        listComboChannels.append(nameCh + " (F" + QString::number(i+1) + ") | " + stateCh);
    }
    mListFreq.unlock();

    QList<QComboBox*> listCombo;
    listCombo.append(ui->comboGroupChannel1);
    listCombo.append(ui->comboGroupChannel2);
    listCombo.append(ui->comboGroupChannel3);
    listCombo.append(ui->comboGroupChannel4);
    int i = 0;
    foreach (QComboBox * combo, listCombo) {
        combo->clear();
        combo->addItems(listComboChannels);
        if(group->typeInput[i] == cGroupChannels::Input_Analog)
        {
            combo->setCurrentIndex(group->channel[i] + 1);
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Math)
        {
            combo->setCurrentIndex(group->mathChannel[i] + listChannels.size() + 1);
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Freq)
        {
            combo->setCurrentIndex(group->freqChannel[i] + listMath.size() + listChannels.size() + 1);
        }
        else
        {
            combo->setCurrentIndex(0);
        }
        i++;
    }

    ui->stackedWidget->setCurrentIndex(23);
    ui->nameSubMenu->setText("ГРУППА " + QString::number(num));
}

void dMenu::slotOpenChannel(int num)
{
    openSettingsChannel(num, 0);
}

void dMenu::slotOpenDigitOutput(int num)
{
    curRelay = num - 1;
    mListRelay.lock();
    ui->comboDigitOutputType->setCurrentIndex(listRelais.at(curRelay)->type & 1);
    mListRelay.unlock();
    ui->stackedWidget->setCurrentIndex(19);
    ui->nameSubMenu->setText("ВЫХОД " + QString::number(num));
}

void dMenu::slotOpenFreq(int num)
{
    curFreq = num - 1;
    mListFreq.lock();
    cFreqChannel * freq = listFreq.at(curFreq);
    ui->comboTypeFreq->setCurrentIndex(freq->GetSignalType());
    ui->impulseDuration->setValue(freq->GetImpulseDuration());
    ui->impulseWeight->setValue(freq->getImpulseWeight());
    ui->nameFreqChannel->setText(freq->GetChannelName());
    ui->scaleDownFreq->setValue(freq->GetLowerMeasureLimit());
    ui->scaleUpFreq->setValue(freq->GetHigherMeasureLimit());
    ui->unitFreq->setText(freq->getUnit());
    ui->periodChFreq->setValue(freq->getMeasurePeriod());
    ui->dempferFreq->setValue(freq->getDempher());
    on_comboTypeFreq_currentIndexChanged(ui->comboTypeFreq->currentIndex());
    ui->stackedWidget->setCurrentIndex(35);
    ui->nameSubMenu->setText("ВХОД " + QString::number(num));
    mListFreq.unlock();
}

void dMenu::selectPageMain()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->nameSubMenu->setText("");
    ui->frameNameSubMenu->setHidden(true);
}

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
    keyboard::olderprop = "";
    keyboard kb(this);
    kb.setModal(true);
    kb.setWarning("Введите пароль режима ЭКСПЕРТ", true);
    kb.exec();
    cExpertAccess::accessRequest(keyboard::newString);

    if(cExpertAccess::getMode() == Access_Admin)
    {
        mListDev.lock();
        foreach (cDevice * dev, listDevice) {
            dev->setHashRoot(true);
        }
        mListDev.unlock();
    }
    else
    {
        mListDev.lock();
        foreach (cDevice * dev, listDevice) {
            dev->setHashRoot(false);
        }
        mListDev.unlock();
    }

    // Изменить видимость виджетов в соответствии с режимом доступа
    changeVisibleWidgets();
}

void dMenu::changeVisibleWidgets()
{
    // получить информацию об уровне доступа
    accessModeType access = cExpertAccess::getMode();

    //скрыть все, что может скрываться
    foreach (QWidget * w, listWidgetsAdmin) { w->hide(); }
    foreach (QWidget * w, listWidgetsExpert) { w->hide(); }

    if(access == Access_Expert)
    {
        ui->modeAccess->setText("ЭКСПЕРТ");
    }
    else if(access == Access_Admin)
    {
        ui->modeAccess->setText("АДМИН");
    }
    else
    {
        ui->modeAccess->setText("ЮЗЕР");
    }

    // включить отображение виджетов, соответствующих уровню доступа
    switch(access)
    {
    case  Access_Admin:
        foreach (QWidget * w, listWidgetsAdmin) { w->show(); }
    case Access_Expert:
        foreach (QWidget * w, listWidgetsExpert) { w->show(); }
    case Access_User:
        // нечего больше показывать простому сметрному
    default:
        break;
    }
}


void dMenu::on_bAnaliz_clicked()
{
    if(systemOptions.display == cSystemOptions::Steel) return;
    initAnalizePage();
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
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameSubMenu->setText("НАСТРОЙКИ");
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
    ui->nameSubMenu->setText("ЦИФР. ВХОДЫ");
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
    ui->nameSubMenu->setText("ДИСКРЕТ. ВХ.");
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
    ui->stackedWidget->setCurrentIndex(10);
    ui->nameSubMenu->setText("ВЫХОДЫ");
}

void dMenu::on_bBackDigitOutputs_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
    ui->nameSubMenu->setText("ВЫХОДЫ");
}

void dMenu::on_bBackDigitOutputSettings_clicked()
{
    addWidgetDigitOutputs();
    ui->stackedWidget->setCurrentIndex(18);
    ui->nameSubMenu->setText("ДИСКР. ВЫХОДЫ");
}

void dMenu::on_bMath_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
    ui->nameSubMenu->setText("МАТЕМАТИКА");
    addWidgetMath();
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

void dMenu::on_bBackDevices_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->nameSubMenu->setText("ДИАГНОСТИКА");
    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bDevices_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
    ui->nameSubMenu->setText("О ПРИБОРЕ");
}

void dMenu::on_bBackDevice_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
    ui->nameSubMenu->setText("О ПРИБОРЕ");
}

void dMenu::on_bDevicesMain_clicked()
{
    ui->stackedWidget->setCurrentIndex(37);
    ui->nameSubMenu->setText("КОНТРОЛЛЕР");
}

void dMenu::on_bBackDevice_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(24);
    ui->nameSubMenu->setText("О ПРИБОРЕ");
}

void dMenu::on_bDevice1_clicked()
{
    if(!listDevice.at(0)->getOnline()) return;
    if(listDevice.at(0)->deviceType >= Count_Device_Type) return;
    ui->stackedWidget->setCurrentIndex(25);
    ui->nameSubMenu->setText("МОДУЛЬ 1");
    curDiagnostDevice = 1;
    findUpdateFales();
}

void dMenu::on_bDevice2_clicked()
{
    if(!listDevice.at(1)->getOnline()) return;
    if(listDevice.at(1)->deviceType >= Count_Device_Type) return;
    ui->stackedWidget->setCurrentIndex(25);
    ui->nameSubMenu->setText("МОДУЛЬ 2");
    curDiagnostDevice = 2;
    findUpdateFales();
}

void dMenu::on_bDevice3_clicked()
{
    if(!listDevice.at(2)->getOnline()) return;
    if(listDevice.at(2)->deviceType >= Count_Device_Type) return;
    ui->stackedWidget->setCurrentIndex(25);
    ui->nameSubMenu->setText("МОДУЛЬ 3");
    curDiagnostDevice = 3;
    findUpdateFales();
}

void dMenu::on_bDevice4_clicked()
{
    if(!listDevice.at(3)->getOnline()) return;
    if(listDevice.at(3)->deviceType >= Count_Device_Type) return;
    ui->stackedWidget->setCurrentIndex(25);
    ui->nameSubMenu->setText("МОДУЛЬ 4");
    curDiagnostDevice = 4;
    findUpdateFales();
}

void dMenu::on_bDevice5_clicked()
{
    if(!listDevice.at(4)->getOnline()) return;
    if(listDevice.at(4)->deviceType >= Count_Device_Type) return;
    ui->stackedWidget->setCurrentIndex(25);
    ui->nameSubMenu->setText("МОДУЛЬ 5");
    curDiagnostDevice = 5;
    findUpdateFales();
}

void dMenu::on_bDevice6_clicked()
{
    if(!listDevice.at(5)->getOnline()) return;
    if(listDevice.at(5)->deviceType >= Count_Device_Type) return;
    ui->stackedWidget->setCurrentIndex(25);
    ui->nameSubMenu->setText("МОДУЛЬ 6");
    curDiagnostDevice = 6;
    findUpdateFales();
}

void dMenu::on_bBackLight_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->nameSubMenu->setText("РАБОТА");
}

void dMenu::on_bSetLight_clicked()
{
    ui->stackedWidget->setCurrentIndex(26);
    ui->nameSubMenu->setText("ЯРКОСТЬ");
}

void dMenu::on_bListDiagnostics_clicked()
{
    ui->stackedWidget->setCurrentIndex(27);
    ui->nameSubMenu->setText("ОШИБКИ");
    updateDiagnosticMess();
}

void dMenu::on_bBackListDiagnostics_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->nameSubMenu->setText("ДИАГНОСТИКА");
}

void dMenu::on_bBackMeasure_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->nameSubMenu->setText("ДИАГНОСТИКА");
}

void dMenu::on_bMeasuredValue_clicked()
{
    ui->stackedWidget->setCurrentIndex(28);
    ui->nameSubMenu->setText("ИЗМЕРЕНИЯ");
    addWidgetMeasures();
}

void dMenu::on_bModeling_clicked()
{
    ui->stackedWidget->setCurrentIndex(29);
    ui->nameSubMenu->setText("МОДЕЛИР.");
    addWidgetModeling();
}

void dMenu::on_bBackModeling_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->nameSubMenu->setText("ДИАГНОСТИКА");
}

void dMenu::on_bProtect_clicked()
{

}

void dMenu::on_bTypeConnect_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
    ui->nameSubMenu->setText("ТИП СВЯЗИ");
}



void dMenu::on_bEthernet_clicked()
{
//    if(ethernet->getOnline())
//    {
//        if(ethernet->getDhcpEn())
//        {
            ethernet->updateParamEternet();
            ui->ipAddr->setText(ethernet->getIpAddr());
            ui->netMask->setText(ethernet->getNetMask());
            ui->gateWay->setText(ethernet->getGateWay());
//        }
//        ui->bToConnect->setText("ОТКЛЮЧИТЬСЯ");
//    }
//    else
//    {
//        ui->ipAddr->setText("10.12.13.5");
//        ui->netMask->setText("255.255.255.0");
//        ui->gateWay->setText("10.12.13.255");
//        ui->bToConnect->setText("ПОДКЛЮЧИТЬСЯ");
//    }
    ui->stackedWidget->setCurrentIndex(31);
    ui->nameSubMenu->setText("ETHERNET");
}

void dMenu::on_bBackTypeConnect_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameSubMenu->setText("НАСТРОЙКИ");
}

void dMenu::on_bBackEthernet_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
    ui->nameSubMenu->setText("ТИП СВЯЗИ");
}

void dMenu::on_bBackFromOutputs_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameSubMenu->setText("НАСТРОЙКИ");
    ui->frameNameSubMenu->setHidden(false);
}

void dMenu::on_bDigitalOutputs_clicked()
{
    addWidgetDigitOutputs();
    ui->stackedWidget->setCurrentIndex(18);
    ui->nameSubMenu->setText("ДИСКР. ВЫХОДЫ");
}

void dMenu::on_bBackMathSetting_clicked()
{
    ui->stackedWidget->setCurrentIndex(20);
    ui->nameSubMenu->setText("МАТЕМАТИКА");
}

void dMenu::on_bTypeMultigraph_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
    ui->nameSubMenu->setText("ТИП ПРИБОРА");
}

void dMenu::on_bBackTypeMultigraph_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
    ui->nameSubMenu->setText("ОПЦИИ");
}

void dMenu::on_bModbusSlave_clicked()
{
    ui->stackedWidget->setCurrentIndex(34);
    ui->nameSubMenu->setText("MODBUS");
    updateInterfaceWidgets();
}

void dMenu::on_bBackModbusSlave_clicked()
{
    ui->stackedWidget->setCurrentIndex(30);
    ui->nameSubMenu->setText("ТИП СВЯЗИ");
}

void dMenu::on_bFreq_clicked()
{
    addWidgetFreqs();
    ui->stackedWidget->setCurrentIndex(36);
    ui->nameSubMenu->setText("ЧАСТ. ВХОДЫ");
}

void dMenu::on_bBackFreq_clicked()
{
    addWidgetFreqs();
    ui->stackedWidget->setCurrentIndex(36);
    ui->nameSubMenu->setText("ЧАСТ. ВХОДЫ");
}

void dMenu::on_bBackListFreq_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->nameSubMenu->setText("ВХОДЫ");
}



void dMenu::updateDriversWidgets()
{
    QStringList listComboStrDrives;
    listDrives.clear();
    int size = flash->getNumDisk();
    for(int i = 0; i < size; i++)
    {
        QString name = flash->getNameDisk(i);
#ifdef Q_OS_WIN
        name.resize(name.size()-1);
#endif
        if(!name.contains("mmc"))
        {
            if(name.contains("sd"))
            {
                listComboStrDrives.append("USB накопитель (" + name + ")");
            }
            else
            {
                listComboStrDrives.append("Накопитель (" + name + ")");
            }
            listDrives.append(name);
        }
    }
    ui->comboDrives->clear();
    ui->comboDrives->addItems(listComboStrDrives);
    ui->comboDrives->setCurrentIndex(ui->comboDrives->count() - 1);
}

void dMenu::on_bEditDataTime_clicked()
{
    if(cExpertAccess::getMode() == ACCESS_USER) return;
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
    ui->DateFormat->setCurrentIndex(systemOptions.dateindex);
    ui->timeformat->clear();
    ui->timeformat->addItems(timestrings);
    ui->timeformat->setCurrentIndex(systemOptions.timeindex);
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
    systemOptions.dateindex = ui->DateFormat->currentIndex();
    systemOptions.timeindex = ui->timeformat->currentIndex();
//    systemOptions.timeFormat = timestrings.at(timeindex);
//    systemOptions.dateFormat = datestrings.at(dateindex);

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
//    int countChannels = listChannels.size();
    cFileManager::readChannelsSettings(pathtooptions);
//    int newCountChannels = listChannels.size();
//    for(int i = countChannels; i < newCountChannels; i++)
//    {
//        connect(listChannels.at(i), SIGNAL(updateSignal(int)), this, SLOT(updateChannelSlot(int)));
//    }
    updateSystemOptions();
    log->addMess("Reset to default", cLogger::USER);
    emit saveButtonSignal();
}

void dMenu::initAnalizePage()
{
//    if(systemOptions.display == cSystemOptions::Steel)
//    {
//        ui->stackedWidgetTypeAnaliz->setCurrentIndex(1);

//        clearLayout(ui->verticalLayoutAnalizSteel);

//        QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
//        ui->verticalLayoutAnalizSteel->addItem(verticalSpacer);
//    }
//    else
//    {
        if(listChannels.size() == 0) return;
        ui->stackedWidgetTypeAnaliz->setCurrentIndex(0);
//    }
}

void dMenu::UpdateAnalyze()
{

    if(listChannels.size() < 4) return;
    double averagechannel_1 , averagechannel_2 , averagechannel_3,averagechannel_4 , sum;

    averagechannel_1 = averagechannel_2 = averagechannel_3 = averagechannel_4 = 0.0;

//    averagechannel_1 = mathresolver::dGetAverageValue(Y_coordinates_Chanel_1_archive);
//    averagechannel_2 = mathresolver::dGetAverageValue(Y_coordinates_Chanel_2_archive);
//    averagechannel_3 = mathresolver::dGetAverageValue(Y_coordinates_Chanel_3_archive);
//    averagechannel_4 = mathresolver::dGetAverageValue(Y_coordinates_Chanel_4_archive);

//    double minimumchannel_1 = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_1_archive);
//    double maximumchannel_1 = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_1_archive);

//    double minimumchannel_2 = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_2_archive);
//    double maximumchannel_2 = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_2_archive);

//    double minimumchannel_3 = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_3_archive);
//    double maximumchannel_3 = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_3_archive);

//    double minimumchannel_4 = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_4_archive);
//    double maximumchannel_4 = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_4_archive);

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

//    ui->analizeminvaluechannel_1->setText(QString::number(minimumchannel_1));
//    ui->analizeminvaluechannel_2->setText(QString::number(minimumchannel_2));
//    ui->analizeminvaluechannel_3->setText(QString::number(minimumchannel_3));
//    ui->analizeminvaluechannel_4->setText(QString::number(minimumchannel_4));

//    ui->analizemaxvaluechannel_1->setText(QString::number(maximumchannel_1));
//    ui->analizemaxvaluechannel_2->setText(QString::number(maximumchannel_2));
//    ui->analizemaxvaluechannel_3->setText(QString::number(maximumchannel_3));
//    ui->analizemaxvaluechannel_4->setText(QString::number(maximumchannel_4));
}

void dMenu::updateDiagnosticMess()
{
    QJsonArray messagesarray;
    cLogger log(pathtomessages);
    messagesarray = log.MessRead();

    for (int var = 0; var < messagesarray.count() ; ++var) {
        QJsonObject mes = messagesarray.at(var).toObject();
        if(((mes.value("C") == cLogger::SERVICE) \
            && (cExpertAccess::getMode() != Access_User))\
                || (mes.value("C") == cLogger::ERR))
        {
//            if((mes.value("S") == cLogger::CHANNEL) || \
//                    (mes.value("S") == cLogger::DEVICE) || \
//                    (mes.value("S") == cLogger::MODBUS))
//            {
                QString num = QString("%1").arg(var+1, 4, 10, QChar('0'));
                ui->listWidget->addItem(num + " | " + mes.value("D").toString() \
                                        + " " +  mes.value("T").toString() \
                                        + " | "+ mes.value("M").toString());
//            }
        }
    }
    ui->listWidget->scrollToBottom();
}


void dMenu::on_radioButAnalogModes_clicked()
{
    ui->frameAnalogModes->show();
//    ui->frameSteelMode->hide();
}

void dMenu::on_radioButSteelModes_clicked()
{
    ui->frameAnalogModes->hide();
//    ui->frameSteelMode->show();
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
    path = listDrives.at(ui->comboDrives->currentIndex()) + "/" + ui->nameDirOnDrive->text() + "/";
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
    path = QString(listDrives.at(ui->comboDrives->currentIndex()) + "/" + ui->nameDirOnDrive->text() + "/");
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
    path = QString(listDrives.at(ui->comboDrives->currentIndex()) + "/" + ui->nameDirOnDrive->text() + "/");
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
    path = QString(listDrives.at(ui->comboDrives->currentIndex()) + "/" + ui->nameDirOnDrive->text() + "/");
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
    path = listDrives.at(ui->comboDrives->currentIndex()) + "/" + ui->nameDirOnDrive->text() + "/";
#ifdef Q_OS_LINUX
    path = QString("/media/" + path);
#endif
    src = QString(path + "options.txt");
    src2 = QString(path + "steeloptions.txt");
    if(QFile::exists(src) && QFile::exists(src2))
    {
        cFileManager::readChannelsSettings(src);
        cFileManager::readSteelsSettings(src2);
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
    path = QString(listDrives.at(ui->comboDrives->currentIndex()) + "/" \
                   + ui->nameDirOnDrive->text() + "/archive/");
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
    QString src, dest, path, csv, destCsv;
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
//        QStringList strlist = pathArch.split("/");
        path = QString(listDrives.at(ui->comboDrives->currentIndex()) + "/" \
                       + ui->nameDirOnDrive->text() + "/archive/");
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

            //Создание CSV-файла
            csv = sl.at(n-2) + "_sek_" + strDay + ".csv";
            destCsv = QString(path + "archive_sek_" + strDay + ".csv");
            makeCsvFileArchive(src, csv, destCsv);
        }

        //        int progress = (countArchFiles*100)/days;
        ui->progressBarLoadFiles->setValue((countArchFiles*100)/days);
        countArchFiles++;
    }
}

void dMenu::copyLastArchFile()
{
    QString src, dest, path, csv, destCsv;
    path = QString(listDrives.at(ui->comboDrives->currentIndex()) + "/" \
                   + ui->nameDirOnDrive->text() + "/archive/");
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

        // создание CSV-файла на внешнем носителе
        csv = sl.at(n-2) + "_sek.csv";
        destCsv = QString(path + "archive_sek.csv");
        makeCsvFileArchive(src, csv, destCsv);

        mesDialog.showInfo(mess, "Сообщение");
        mesDialog.setWindowModality(Qt::WindowModal);
        mesDialog.show();
    }



    ui->progressBarLoadFiles->setHidden(true);
}


void dMenu::makeCsvFileArchive(QString src, QString csv, QString dest)
{
    QVector<sTickCh> vecTicks;
    QFile arch_sek(src);

    if(arch_sek.exists())   //проверка наличия такого файла
    {
        if(arch_sek.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&arch_sek);
            sTickCh tick;
            while(!stream.atEnd())
            {   //чтение файла до конца
                stream >> tick.time;
                stream.readRawData((char *) tick.channel, sizeof(tick.channel));
                vecTicks.append(tick);
            }
            arch_sek.close();
        }
    }

    if(vecTicks.size() > 0)
    {
        //Создание CSV-файла
        QFile csvFile(csv);
        // Открываем, или создаём файл, если он не существует
        if(csvFile.open( QIODevice::WriteOnly ))
        {
            // Создаём текстовый поток, в который будем писать данные
            QTextStream textStream( &csvFile );
            QStringList stringList; // Вспомогательный объект QSqtringList, который сформирует строку
            QDateTime time2000 = QDateTime::fromString(STR_DATE_2000, FORMAT_STR_DATE_2000);

            // Строка заголовков CSV-файла
            stringList << "Date Time";
            for(int i = 0; i < TOTAL_NUM_CHANNELS; i++)
            {
                stringList << "Channel" + QString::number(i+1);
            }
            textStream << stringList.join( ';' )+"\n";

            //Конвертация значений
            foreach(sTickCh tick, vecTicks)
            {
                stringList.clear(); // ... каждый раз очищая stringList

                QDateTime time(time2000.addSecs(tick.time));
                stringList << time.toString(FORMAT_STR_DATE_2000);

                for(int i = 0; i < TOTAL_NUM_CHANNELS; i++)
                {
                    stringList << QString::number(tick.channel[i], 'f', 8);
                }

                /* После чего отправляем весь stringList в файл через текстовый поток
             * добавляя разделители в виде ";", а также поставив в конце символ окончания строки
             * */
                textStream << stringList.join( ';' )+"\n";
            }

            csvFile.close();
        }

        // перенос файла CSV на внешний носитель
        if(csvFile.exists())   //проверка наличия такого файла
        {
            QFile::remove(dest);    //удалить с носителя (устарел)
            QFile::copy(csv, dest);
            QFile::remove(csv);
        }
    }

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
    mListUstvok.lock();
    Ustavka *ust = new Ustavka();
    int i = listUstavok.size();
    ust->setIdentifikator("Limit " + QString::number(i+1));
    listUstavok.append(ust);
    mListUstvok.unlock();
    emit newUstavka(i);
    log->addMess("New setpoint", cLogger::SERVICE, cLogger::USTAVKA);

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
    log->addMess("New group", cLogger::USER, cLogger::UI);

    //регенерация кнопок групп
    addWidgetGroup();
}

void dMenu::on_bDelGroup_clicked()
{
    if(listGroup.size() > 1)
    {
        QString mess = "Del group " + listGroup.at(curGroupEdit)->groupName + "(" + QString::number(curGroupEdit) + ")";
        log->addMess(mess, cLogger::USER, cLogger::UI);
        listGroup.removeAt(curGroupEdit);
    }
    ui->stackedWidget->setCurrentIndex(22);
    ui->nameSubMenu->setText("ГРУППЫ");
    addWidgetGroup();
}

void dMenu::on_bDelMath_clicked()
{
    mListMath.lock();
    if(listMath.size() > 1)
    {
        listMath.at(curMathEdit)->deleteLater();
        listMath.removeAt(curMathEdit);
    }
    mListMath.unlock();
    ui->stackedWidget->setCurrentIndex(20);
    ui->nameSubMenu->setText("МАТЕМАТИКА");
    addWidgetMath();
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
            g->typeInput[i] = cGroupChannels::Input_None;
        }
        else if(indexCh <= listChannels.size())
        {
            g->typeInput[i] = cGroupChannels::Input_Analog;
            g->channel[i] = indexCh-1;
        }
        else if(indexCh <= (listChannels.size() + listMath.size()))
        {
            g->typeInput[i] = cGroupChannels::Input_Math;
            g->mathChannel[i] = indexCh-1-listChannels.size();
        }
        else if(indexCh <= (listChannels.size() + listMath.size() + listFreq.size()))
        {
            g->typeInput[i] = cGroupChannels::Input_Freq;
            g->freqChannel[i] = indexCh-1-listChannels.size()-listMath.size();
        }
    }

    ui->stackedWidget->setCurrentIndex(22);
    ui->nameSubMenu->setText("ГРУППЫ");
    addWidgetGroup();
}

void dMenu::on_lightUp_clicked()
{
    if(light < 91) light += 10;
    ui->volLight->setText(QString::number(light));
    ui->progressLight->setValue(light);
    setBrightness(light);
}

void dMenu::on_lightDown_clicked()
{
    if(light > 10) light -= 10;
    ui->volLight->setText(QString::number(light));
    ui->progressLight->setValue(light);
    setBrightness(light);
}

void dMenu::on_bLogEvents_clicked()
{
    dialogSetingsChannel = new dSettings(listChannels, 1, 1);
    dialogSetingsChannel->exec();
    dialogSetingsChannel->deleteLater();
}

/*
 * Обновление данных диагностики (измеренные значения)
 */
void dMenu::updateLabelDiagnostic()
{
    if(systemOptions.display == cSystemOptions::Steel)
    {
        if(listSteel.size() == 0) return;
        if(listLabelEmfpAnalizSteel.size() == 0) return;
        for(int i = 0; (i < listSteel.size()) && (i < listLabelEmfpAnalizSteel.size()); i++)
        {
            cSteel * steel = listSteel.at(i);
            if(!std::isnan(steel->temp))
                listLabelTempAnalizSteel.at(i)->setText(QString::number(steel->temp, 'f', 1));
            else
                listLabelTempAnalizSteel.at(i)->setText("НЕТ ДАННЫХ");

            if(!std::isnan(steel->cj))
                listLabelCj.at(i)->setText(QString::number(steel->cj, 'f', 1));
            else
                listLabelCj.at(i)->setText("НЕТ ДАННЫХ");

            if(!std::isnan(steel->eds))
                listLabelEmfpAnalizSteel.at(i)->setText(QString::number(steel->eds, 'f', 1));
            else
                listLabelEmfpAnalizSteel.at(i)->setText("НЕТ ДАННЫХ");

//            int indexTC[] = {0, 0, 0, 1, 2, 0, 0};
//            listComboTypeTermo.at(i)->setCurrentIndex(indexTC[steel->technology->nSt]);
        }
    }
    else
    {
        // Аналоговые сигналы
        int i = 0;
        foreach(QLabel * volLabel, listLabelDiagnostic)
        {
            mListChannel.lock();
            if(i >= listChannels.size()) break; // Ошибка: столько каналов тут нет
            if(listChannels.at(i)->enable == false) continue; // Пропустить отключенные каналы
            ChannelOptions * channel = listChannels.at(i);

            double val = 0;
            if(channel->getVoltageType() == ChannelOptions::Value_Real)
            {
                val = channel->GetCurrentChannelValue();
            }
            else
            {
                val = channel->ConvertVisualValue(channel->GetCurrentChannelValue());
            }
            if(!std::isnan(val))
            {
                volLabel->setText(QString::number(val));
            }
            else
            {
                volLabel->setText(NaNMessage);
            }

            mListChannel.unlock();
            i++;
        }
        // Частотные сигналы
        i = 0;
        foreach(QLabel * volLabel, listLabelDiagnosticFreq)
        {

            if(i >= listFreq.size()) break; // Ошибка: столько каналов тут нет
            if(listFreq.at(i)->enable == false) continue; // Пропустить отключенные каналы

            mListFreq.lock();
            volLabel->setText(QString::number(listFreq.at(i)->GetCurrentChannelValue()));
            mListFreq.unlock();
            i++;
        }
    }
}

void dMenu::updateLabelModeling()
{
    int i = 0;
    foreach(QLabel * volLabel, listLabelModeling)
    {
        QList<QColor> colors;
                  //ON        //OFF            //NO_CONFIRM
        colors << ColorCh1 << ColorCh4Light << COLOR_GRAY;

        QStringList strStyle;
        strStyle << "background-color: rgb(" + \
                    QString::number(colors.at(0).red()) + ", " + \
                    QString::number(colors.at(0).green()) + ", " + \
                    QString::number(colors.at(0).blue()) + ");\n"
                    "color: rgb(255, 255, 255);\n"
                    "border-radius: 0px;";
        strStyle << "background-color: rgb(" + \
                    QString::number(colors.at(1).red()) + ", " + \
                    QString::number(colors.at(1).green()) + ", " + \
                    QString::number(colors.at(1).blue()) + ");\n"
                    "color: rgb(255, 255, 255);"
                    "border-radius: 0px;";
        strStyle << "background-color: rgb(" + \
                    QString::number(colors.at(2).red()) + ", " + \
                    QString::number(colors.at(2).green()) + ", " + \
                    QString::number(colors.at(2).blue()) + ");\n"
                    "color: rgb(255, 255, 255);"
                    "border-radius: 0px;";

//        mListRelay.lock();
        if(i < listRelais.size())
        {
            cRelay * r = listRelais.at(i);
            volLabel->setText(r->getCurState() ? "ON" : "OFF");
            if(r->confirmedState)
            {
                volLabel->setStyleSheet(r->getCurState() ? strStyle.at(0) : strStyle.at(1));
            }
            else
            {
                volLabel->setStyleSheet(strStyle.at(2));
            }
        }
//        mListRelay.unlock();
        i++;
    }
}

void dMenu::updateMathResultFormula()
{
    if(ui->stackedWidget->currentIndex() == 32)
    {
        cMathChannel math;
        double result = math.testFormula(ui->formulaMath->text(),\
                                         ui->comboMathArg1->currentIndex()-1,\
                                         ui->comboMathArg2->currentIndex()-1,\
                                         ui->comboMathArg3->currentIndex()-1,\
                                         ui->comboMathArg4->currentIndex()-1);
        ui->labelResultFormula->setText(QString::number(result, 'f'));
    }
}



void dMenu::on_modelingOn_clicked()
{
    mListRelay.lock();
    listRelais.at(0)->setState(true);
    mListRelay.unlock();
}

void dMenu::on_modelingOff_clicked()
{
    mListRelay.lock();
    listRelais.at(0)->setState(false);
    mListRelay.unlock();
}

void dMenu::addWidgetModeling()
{

    clearLayout(ui->verticalLayouModeling);
    listLabelModeling.clear();

    QFont font6;
    font6.setFamily(QStringLiteral("Open Sans"));
    font6.setPointSize(20);

    int i = 0;
    mListRelay.lock();
    foreach(cRelay * relay, listRelais)
    {
        if(!relay->enable) continue;
        QFrame * frameModeling = new QFrame(ui->widgetScrollAreaModeling);
        frameModeling->setFrameShape(QFrame::NoFrame);
        frameModeling->setFrameShadow(QFrame::Raised);

        QHBoxLayout * horizontalLayout_8 = new QHBoxLayout(frameModeling);

        QLabel * labelModeling = new QLabel(frameModeling);
        labelModeling->setFont(font6);
        horizontalLayout_8->addWidget(labelModeling);

        QSpacerItem * horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        horizontalLayout_8->addItem(horizontalSpacer_5);

        QPushButton * modelingOn = new QPushButton(frameModeling);
        modelingOn->setObjectName(QStringLiteral("bModeling_On_") + QString::number(i));
        modelingOn->setMinimumSize(QSize(100, 41));
        modelingOn->setMaximumSize(QSize(100, 41));
        modelingOn->setFont(font6);
        modelingOn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
                                                "color: rgb(0, 0, 0);\n"
                                                "border-radius: 0px;"));
        modelingOn->installEventFilter(this);
        horizontalLayout_8->addWidget(modelingOn);
//        listButtonModeling.append(modelingOn);


        QLabel * modelingVol = new QLabel(frameModeling);
        modelingVol->setMinimumSize(QSize(131, 31));
        modelingVol->setMaximumSize(QSize(185, 45));
        modelingVol->setFont(font6);
        modelingVol->setStyleSheet(QLatin1String("	background-color: rgb(21, 159, 133);\n"
                                                 "	color: rgb(255, 255, 255);\n"
                                                 "	border-radius: 0px;"));
        modelingVol->setAlignment(Qt::AlignCenter);
        horizontalLayout_8->addWidget(modelingVol);
        listLabelModeling.append(modelingVol);

        QPushButton * modelingOff = new QPushButton(frameModeling);
        modelingOff->setObjectName(QStringLiteral("bModeling_Off_") + QString::number(i));
        modelingOff->setMinimumSize(QSize(100, 41));
        modelingOff->setMaximumSize(QSize(100, 41));
        modelingOff->setFont(font6);
        modelingOff->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
                                                 "color: rgb(0, 0, 0);\n"
                                                 "border-radius: 0px;"));
        modelingOff->installEventFilter(this);
        horizontalLayout_8->addWidget(modelingOff);
        ui->verticalLayouModeling->addWidget(frameModeling);
//        listButtonModeling.append(modelingOff);

        QString strOut = "ВЫХОД " + QString::number(i+1);
        labelModeling->setText(strOut);
        modelingOn->setText(QApplication::translate("dMenu", "ВКЛ", Q_NULLPTR));
        modelingVol->setText(relay->getCurState() ? "ON" : "OFF");
        modelingOff->setText(QApplication::translate("dMenu", "ОТКЛ", Q_NULLPTR));

        i++;
    }
    mListRelay.unlock();

    QSpacerItem * verticalSpacer_36 = new QSpacerItem(20, 165, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayouModeling->addItem(verticalSpacer_36);
}

void dMenu::updateDevicesUI()
{
    QList<wButtonStyled*> listButtonDevices;
    listButtonDevices << ui->bDevice1 << ui->bDevice2 << ui->bDevice3 \
                      << ui->bDevice4 << ui->bDevice5 << ui->bDevice6;
    assert(listDevice.size() == listButtonDevices.size());
    QStringList strType;
    strType << "" << "4AI" << "8RP" << "STEEL" << "6DI6RO";
    int i = 0;
    foreach(wButtonStyled * bDev, listButtonDevices)
    {
        mListDev.lock();
        cDevice * device = listDevice.at(i);
        QString str = "МОДУЛЬ РАСШИРЕНИЯ " + QString::number(i+1) + " | ";
        QStringList strOnline;
//        strOnline << "ОТКЛЮЧЕН" << "ВКЛЮЧЕН";
//        str += strOnline.at(device->getOnline());

        if(device->getOnline() || !device->getStable()) //плата онлайн
        {
            if(((int)(device->deviceType) < Count_Device_Type) && ((int)(device->deviceType) > Device_None))
            {
                str += "ВКЛЮЧЕН";
                if(strType.at((int)(device->deviceType)).size())
                {
                    str +=  " | " + strType.at(device->deviceType);
                }
            }
            else
            {
                str += "МОДУЛЬ (" + QString::number((int)(device->deviceType)) + ")";
            }
        }
        else    //плата оффлайн
        {
            str += "ОТКЛЮЧЕН";
        }

        bDev->setText(str);
        i++;
        mListDev.unlock();
    }
    if(curDiagnostDevice != 0)
    {
        cDevice * curDev = listDevice.at(curDiagnostDevice - 1);
        if(curDev->deviceType < strType.size())
        {
            ui->deviceType->setText(strType.at(curDev->deviceType));
        }
//        ui->deviceState->setText(QString::number(curDev->deviceState));
        QStringList strStatus;
//        strStatus << "NOINIT" << "CONFIG" << "EXECUTE" << "IDLE" << "ERROR";
//        if((int)curDev->deviceStatus < strStatus.size())
//        {
//            ui->deviceStatus->setText(strStatus.at(curDev->deviceStatus));
//        }
        int numErr = 0;
        QStringList devErrors;
        devErrors << "MODEL" << "SERIAL" << "FACTORY" << "CRC32" << "MODE" << "ADDRESS" \
                  << "SPEED" << "MODEL_CHECK" << "REZERV" << "RESERVE" << "RESERVE" \
                  << "RESERVE" << "RESERVE" << "RESERVE" << "RESERVE" << "RESERVE";
        QStringList activeErrors;
        for(int i = 0; i < 16; i++)
        {
//            numErr += ((curDev->devErrors >> i) & 1);
            if((curDev->devErrors >> i) & 1)
            {
                numErr++;
                activeErrors << devErrors.at(i);
            }
        }
        ui->devErrors->setText(QString::number(numErr));
        ui->listDeviceErrors->clear();
        ui->listDeviceErrors->addItems(activeErrors);
        ui->protocolVersion->setText(QString::number(curDev->protocolVersion));
        ui->hardwareVersion->setText(QString::number(curDev->hardwareVersion));
        QString strVersion = QString::number(curDev->softwareVersion >> 24);
        strVersion += ("." + QString::number((curDev->softwareVersion >> 16) & 0xFF));
        strVersion += ("." + QString::number((curDev->softwareVersion >> 8) & 0xFF));
        strVersion += ("." + QString::number(curDev->softwareVersion & 0xFF));
        ui->softwareVersion->setText(strVersion);
        ui->serialNumber->setText(QString::number(curDev->serialNumber));
        ui->uptime->setText(QString::number(curDev->uptime));
        int year = (curDev->factoryDate >> 24) & 0xFF;
        int month = (curDev->factoryDate >> 16) & 0xFF;
        int day = (curDev->factoryDate >> 8) & 0xFF;
        QString strDate = QString::number(day) + "."\
                + QString::number(month) + "."\
                + QString::number(year);
        ui->dataOrder->setText(strDate);
    }
}

void dMenu::updateDeviceInfo(uint8_t index)
{
    mListDev.lock();
    assert(listDevice.size() >= index);
    cDevice * device = listDevice.at(index);
    if(!device->getOnline()){
        ui->frameDeviceInfo1->hide();
        ui->frameDeviceInfo2->hide();
    }
    else
    {
        QStringList strType;
        strType << "" << "4AI" << "8RP" << "STEEL" << "6DI6RO";
        ui->deviceType->setText(strType.at(device->deviceType % strType.size()));
//        ui->deviceState->setText(QString::number(device->deviceState));
//        ui->deviceStatus->setText(QString::number(device->deviceStatus));
//        int countErr = 0;
//        QList<int> listErrors;
//        for(int i = 0; i < 8; i++)
//        {
//            if(device->)
//        }
//        ui->devErrors->setText();
        ui->frameDeviceInfo1->show();
        ui->frameDeviceInfo2->show();
    }
    mListDev.unlock();
}


//void dMenu::on_comboDhcpEn_currentIndexChanged(int index)
//{
//    ui->ipAddr->setEnabled(index == 0);
//    ui->netMask->setEnabled(index == 0);
//    ui->gateWay->setEnabled(index == 0);
//}

void dMenu::on_bToConnect_clicked()
{
    QString mess = "Set Ethernet: ip " + ui->ipAddr->text();
    log->addMess(mess, cLogger::SERVICE, cLogger::EXT_NET);
    ethernet->setConfig(ui->ipAddr->text(), \
                            ui->netMask->text(), ui->gateWay->text());
}



void dMenu::on_bDigitOutputSettingsApply_clicked()
{
    mListRelay.lock();
    listRelais.at(curRelay)->type = ui->comboDigitOutputType->currentIndex();
    mListRelay.unlock();
}

void dMenu::addWidgetMath()
{
    clearLayout(ui->verticalLayoutMath);

    // генерация виджетов (кнопок) матканалов
    int i = 0;
    mListMath.lock();
    foreach (cMathChannel * math, listMath) {
        wButtonStyled * bMath = new wButtonStyled(ui->widgetScrollAreaMath);
        bMath->index = i+1;
        QString nameMath = math->getName().size() ? (" | " + math->getName()) : " ";
        bMath->setText("МАТКАНАЛ " + QString::number(bMath->index) + nameMath);
        bMath->setMinimumSize(QSize(0, 70));
        bMath->setColorText(QColor(0xff,0xff,0xff));
        bMath->setColorBg(ColorButtonNormal);
        bMath->setAlignLeft();
        connect(bMath, SIGNAL(clicked(int)), this, SLOT(slotOpenMathChannel(int)));
        ui->verticalLayoutMath->addWidget(bMath);
        i++;
    }
    mListMath.unlock();
    QSpacerItem * verticalSpacer = new QSpacerItem(20, 169, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayoutMath->addItem(verticalSpacer);
}

void dMenu::slotOpenMathChannel(int num)
{
    curMathEdit = num - 1;
    cMathChannel * math = listMath.at(num - 1);

    ui->nameMath->setText(math->getName());
    ui->formulaMath->setText(math->GetMathString());
    ui->unitMath->setText(math->getUnit());

    //определяем существующие каналы и добавляем в комбобоксы
    QStringList listComboChannels;
    listComboChannels.append("ОТКЛЮЧЕН");
    mListChannel.lock();
    for(int i = 0; i < listChannels.size(); i++)
    {
        QString nameCh = listChannels.at(i)->GetChannelName();
        listComboChannels.append(nameCh + " (A" + QString::number(i+1) + ")");
    }
    mListChannel.unlock();
//    foreach (ChannelOptions * ch, listChannels) {
//        listComboChannels.append(ch->GetChannelName());
//    }
//    /*


    QList<QComboBox*> listCombo;
    listCombo.append(ui->comboMathArg1);
    listCombo.append(ui->comboMathArg2);
    listCombo.append(ui->comboMathArg3);
    listCombo.append(ui->comboMathArg4);
    foreach (QComboBox * combo, listCombo) {
        combo->clear();
        combo->addItems(listComboChannels);
    }

    for(int k = 0; k < listCombo.size(); k++)
    {
        listCombo.at(k)->setCurrentIndex(math->numChannel[k] + 1); ;
    }

    ui->stackedWidget->setCurrentIndex(32);
    ui->nameSubMenu->setText("МАТКАНАЛ " + QString::number(num));
}

void dMenu::on_bAddMath_clicked()
{
    mListMath.lock();
    cMathChannel * math = new cMathChannel();
    int size = listMath.size();
    math->setNum(size+1);
    math->setName("Math " + QString::number(size+1));
    listMath.append(math);
    mListMath.unlock();

    addWidgetMath();
}

void dMenu::on_bApplayMath_clicked()
{
    cMathChannel * math = listMath.at(curMathEdit);
    math->setName(ui->nameMath->text());
    math->SetMathEquation(ui->formulaMath->text());
    math->setUnit(ui->unitMath->text());
    QList<QComboBox*> listCombo;
    listCombo.append(ui->comboMathArg1);
    listCombo.append(ui->comboMathArg2);
    listCombo.append(ui->comboMathArg3);
    listCombo.append(ui->comboMathArg4);
    for(int i = 0; i < listCombo.size(); i++)
    {
        math->numChannel[i] = listCombo.at(i)->currentIndex() - 1;
    }
    ui->stackedWidget->setCurrentIndex(20);
    ui->nameSubMenu->setText("МАТЕМАТИКА");
    addWidgetMath();
}

void dMenu::updateInterfaceWidgets()
{
    ui->comboModbusSlaveInterface->setCurrentIndex(systemOptions.extModbus.type);
    ui->modbusSlaveAdress->setValue(systemOptions.extModbus.adress);
    QStringList listStrBauds;
    int size = systemOptions.listBauds.size();
    int curIndexBaud = 0;
    for(int i=0; i<size; i++)
    {
        int baud = systemOptions.listBauds.at(i);
        listStrBauds.append(QString::number(baud));
        if(baud == systemOptions.extModbus.baud)
        {
            curIndexBaud = i;
        }
    }
    ui->comboModbusSlaveBaud->clear();
    ui->comboModbusSlaveBaud->addItems(listStrBauds);
    ui->comboModbusSlaveBaud->setCurrentIndex(curIndexBaud);
    ui->comboModbusSlaveParity->setCurrentIndex(systemOptions.extModbus.parity);
    ui->modbusSlaveStopBits->setValue(systemOptions.extModbus.stopBits);
    ui->modbusSlavePort->setText(QString::number(systemOptions.extModbus.port));
    ui->modbusSlaveDataBits->setValue(systemOptions.extModbus.dataBits);
    // принудительный вызов слота
    on_comboModbusSlaveInterface_currentIndexChanged(ui->comboModbusSlaveInterface->currentIndex());
}

void dMenu::on_comboModbusSlaveInterface_currentIndexChanged(int index)
{
    ui->frameExtModbusTCP->hide();
    ui->frameExtModbusRTU->hide();
    if(index == cSystemOptions::ExtModbus_RTU)
    {
        ui->frameExtModbusRTU->show();
    }
    else if(index == cSystemOptions::ExtModbus_TCP)
    {
        ui->frameExtModbusTCP->show();
    }

}

void dMenu::on_comboTypeFreq_currentIndexChanged(int index)
{
    ui->frameFreqCountImp->hide();
    ui->frameFreqOptions->hide();
    ui->frameMesAboutDiscrete->hide();
    if(index == ImpulseCounterMeasure)
    {
        ui->frameFreqCountImp->show();
        ui->frameFreqOptions->show();
    }
    else if(index == FrequencyMeasure)
    {
        ui->frameFreqOptions->show();
    }
    else if(index == FreqNoMeasure)
    {
        ui->frameMesAboutDiscrete->show();
    }
}

void dMenu::on_bApplyFreq_clicked()
{
    mListFreq.lock();
    cFreqChannel * freq = listFreq.at(curFreq);
    int type = ui->comboTypeFreq->currentIndex();
    freq->SetSignalType(type);
    if(type == ImpulseCounterMeasure)
    {
        freq->setImpulseDuration(ui->impulseDuration->value());
        freq->setImpulseWeight(ui->impulseWeight->value());
    }
    freq->setName(ui->nameFreqChannel->text());
    freq->SetLowerMeasureLimit(ui->scaleDownFreq->value());
    freq->SetHigherMeasureLimit(ui->scaleUpFreq->value());
    freq->setUnit(ui->unitFreq->text());
    freq->SetMeasurePeriod(ui->periodChFreq->value());
    freq->setDempher(ui->dempferFreq->value());
    mListFreq.unlock();
    on_bBackFreq_clicked();
}

void dMenu::on_bFreqResetCountImp_clicked()
{
    //Vag: убедиться что не произойдёт клин из-за Мютекса
    mListFreq.lock();
    listFreq.at(curFreq)->slotResetImpulsBuffer();
    mListFreq.unlock();
}

void dMenu::on_bCancelFreq_clicked()
{
    cFreqChannel * freq = listFreq.at(curFreq);
    ui->comboTypeFreq->setCurrentIndex(freq->GetSignalType());
    ui->impulseDuration->setValue(freq->GetImpulseDuration());
    ui->impulseWeight->setValue(freq->getImpulseWeight());
    ui->nameFreqChannel->setText(freq->GetChannelName());
    ui->scaleDownFreq->setValue(freq->GetLowerMeasureLimit());
    ui->scaleUpFreq->setValue(freq->GetHigherMeasureLimit());
    ui->unitFreq->setText(freq->getUnit());
    ui->periodChFreq->setValue(freq->getMeasurePeriod());
    ui->dempferFreq->setValue(freq->getDempher());
    on_comboTypeFreq_currentIndexChanged(ui->comboTypeFreq->currentIndex());
}

void dMenu::updateDeviceMain()
{
    if(ui->softVersion->text() != QString(CURRENT_VER))
    {
        ui->proborType->setText(getNameDevice().toUpper());
        ui->manufacturer->setText(QString(MANUFACTURER).toUpper());
        ui->site->setText(QString(SITE).toUpper());
        int sn = 132;
        QString serialNum = "%1";
        serialNum = serialNum.arg(sn, 7, 10, QChar('0'));
        ui->serialNumberDevice->setText(serialNum);
        ui->serialDateManufacture->setText("23.09.2019");
        ui->unicNumber->setText(QString::number(86237965));
        ui->softVersion->setText(QString(CURRENT_VER));
        ui->boardVersion->setText(QString(HARDWARE_VERSION));
        ui->modbusVersion->setText(QString(PROTOCOL_VER));
        ui->extModbusVersion->setText(QString(EXT_MODBUS_VER));
    }
    ui->deviceState->setText("OK");
    int timeWork = 132.45;
    ui->wirkingTime->setText(QString::number(timeWork, 'f', 2));
    ui->netErrors->setText("2");
    ui->netGoods->setText("24422");
    accessModeType access = cExpertAccess::getMode();
    QString strAccess = "ПОЛЬЗОВАТЕЛЬ";
    if(access == Access_Expert)
    {
        strAccess = "ЭКСПЕРТ";
    }
    else if(access == Access_Admin)
    {
        strAccess = "АДМИН";
    }
    ui->accessLevel->setText(strAccess);
    ui->boardModel->setText(QString(BOARD_MODEL));
    int countDev = 0;
    foreach (cDevice * dev, listDevice) {
        if (dev->getOnline()) countDev++;
    }
    ui->countModules->setText(QString::number(countDev));
}

QString dMenu::getNameDevice()
{
    QString strName = QString(TYPE_DEVICE);
    if(systemOptions.typeMultigraph == cSystemOptions::Multigraph_Steel)
    {
        strName = strName + "-Steel";
    }
    return strName;
}




void dMenu::on_bUpdateStart_pressed()
{

}

void dMenu::on_bUpdateStart_released()
{

}

void dMenu::findUpdateFales()
{
    if(cExpertAccess::getMode() != Access_User)
    {
        ui->progressBarUpdate->hide();
        dir.setPath(pathtoupdates);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);   //устанавливаем фильтр выводимых файлов/папок
        dir.setSorting(QDir::Time | QDir::Reversed);   //устанавливаем сортировку "от меньшего к большему"
        QFileInfoList list = dir.entryInfoList();     //получаем список файлов директории
        QStringList listNameFiles;
        foreach (QFileInfo file, list) {
            QStringList name = file.fileName().split('.');
            if(name.at(name.size()-1) == "hex")
            {
                listNameFiles.append(file.fileName());
            }
        }
        ui->comboUpdateFiles->clear();
        ui->comboUpdateFiles->addItem("Выбрать ПО");
        ui->comboUpdateFiles->addItems(listNameFiles);
        ui->comboUpdateFiles->setCurrentIndex(0);
        if(ui->comboUpdateFiles->count() > 1)
        {
            ui->bUpdateStart->setEnabled(true);
        }
        ui->frameSoftUpdate->show();
    }
    else
    {
        ui->frameSoftUpdate->hide();
    }

}

void dMenu::on_bUpdateStart_clicked()
{
    if(cExpertAccess::getMode() != Access_User)
    {
        if(ui->comboUpdateFiles->currentIndex() != 0)
        {
            //        if(m_serial == nullptr)
            //        {
            ui->bUpdateStart->setEnabled(false);
            QString filePatch = QString(pathtoupdates) + ui->comboUpdateFiles->currentText();
            updateFile.setFileName(filePatch);

            // подсчёт количества строк в файле
            if(updateFile.open(QIODevice::ReadOnly))
            {
                totalString = 0;
                QString line;
                do
                {
                    line = updateFile.readLine();
                    totalString++;
                }
                while(!line.isEmpty());
                updateFile.close();
            }
            else
            {
                qDebug() << "Error open file";
            }

            // Запись в журнал о начале прошивке, если файл не пустой
            if(totalString > 0)
            {
                QStringList strType;
                strType << "" << "4AI" << "8RP" << "STEEL" << "6DI6RO";
                int size = strType.size();
                int type = (int)listDevice.at(curDiagnostDevice-1)->deviceType;
                QString mess = "Start update module " + QString::number(curDiagnostDevice)\
                        + " | Type: " + strType.at(type % size);
                log->addMess(mess, cLogger::SERVICE, cLogger::DEVICE);
            }

            // инициализация прошивки
            if(updateFile.open(QIODevice::ReadOnly))
            {
                qDebug() << "File is open";
                Transaction tran(Transaction::W);
                tran.slave = curDiagnostDevice;
                tran.offset = cRegistersMap::getOffsetByName("updateSoftware");
                tran.volInt = listDevice.at(curDiagnostDevice-1)->softwareVersion;
                emit signalToWorker(tran);

                timerSoftUpdate = new QTimer();
                timerSoftUpdate->stop();
                connect(timerSoftUpdate, SIGNAL(timeout()), this, SLOT(closeSerialPort()));
                //            timerSoftUpdate->start(5000);
                QTimer::singleShot(5000, this, SLOT(startSoftUpdate()));

            }
            else
            {
                qDebug() << "Error open file";
            }
            //        }
        }
    }
}

void dMenu::startSoftUpdate()
{
    timerSoftUpdate->start(10000);
    m_serial = new QSerialPort(this);
    m_serial->setPortName(comportname);
    m_serial->setBaudRate(115200);//(comportbaud);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
//            m_serial->setFlowControl(p.flowControl);
//    connect(m_serial, &QSerialPort::errorOccurred, this, &dMenu::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &dMenu::readData);
    if(!m_serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Error open Port";
    }
    else
    {
        countString = 0;
        if(totalString != 0)
        {
            float progress = (float)countString / totalString * 100;
            ui->progressBarUpdate->setValue(progress);
            ui->progressBarUpdate->show();
        }
        sendFile();
    }
}

void dMenu::asciiToHex(QByteArray inArray, uint8_t* outArray)
{
    uint8_t tmp[43];
    for(uint8_t i = 1; i < 43; i++)
    {
        if(inArray[i] <= '9' && inArray[i] >= '0' )
        {
            tmp[i] = inArray[i];
            tmp[i] = inArray[i] - 0x30;
        }
        else
        {
            tmp[i] = inArray[i];
            tmp[i] = inArray[i] - 0x41 + 10;
        }
    }
    outArray[0] = ':';
    for(uint8_t i = 1; i < 22 ; i++)
    {
        outArray[i] = tmp[2*i - 1]*16 + tmp[2*i];
    }
}

void dMenu::handleError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}

void dMenu::closeSerialPort()
{
    timerSoftUpdate->stop();
    if (m_serial->isOpen())
    {
        m_serial->close();  //закрыть и удалить порт по завершении
    }
    qDebug() << "Disconnected";

    qDebug() << "File is closed";
//    disconnect(m_serial, &QSerialPort::errorOccurred, this, &dMenu::handleError);
    disconnect(m_serial, &QSerialPort::readyRead, this, &dMenu::readData);
    m_serial->deleteLater();
    disconnect(timerSoftUpdate, SIGNAL(timeout()), this, SLOT(startSoftUpdate()));
    timerSoftUpdate->deleteLater();

    emit signalRestartLocalModbus();
    qDebug() << "Signal Restart Modbus";
    ui->progressBarUpdate->hide();
    ui->comboUpdateFiles->setCurrentIndex(0);
}

void dMenu::readData()
{
    timerSoftUpdate->setInterval(10000);
    const QByteArray data = m_serial->readAll();
#ifdef DEBUG_UPDATE_SOFT
    qDebug() << "data" << data.toStdString().c_str();
#endif
    if (data[0] == 'O')
    {
        this->thread()->msleep(1);  // ждём между строчками 1 мсек
        sendFile();
    }
    else
    {
        m_serial->write(sendArray,22);
    }
}

void dMenu::sendFile()
{
    uint8_t outArray[22];
    QByteArray readArray = updateFile.readLine();
#ifdef DEBUG_UPDATE_SOFT
    qDebug() << "readArray" << readArray.toStdString().c_str();
#endif
    if(!readArray.isEmpty())
    {
        while(readArray.length() < 43)
        {
            readArray.append('0');
        }
        for(int i=0; i<readArray.length(); i++)
        {
            if(readArray.at(i) <= 0x20) readArray.replace(i, 1, "0");
        }
        asciiToHex(readArray, outArray);
        sendArray = QByteArray::fromRawData((char*)(outArray), 22);
        int res = m_serial->write(sendArray,22);
#ifdef DEBUG_UPDATE_SOFT
        qDebug() << "sendArray" << sendArray.toHex().toStdString().c_str();
        qDebug() << countString;
#endif
        countString++;
        float progress = (float)countString / totalString * 100;
        ui->progressBarUpdate->setValue(progress);
    }
    else
    {
        updateFile.close(); //закрыть файл по завершении прошивки
        timerSoftUpdate->stop();
//        QTimer::singleShot(5000, this, SLOT(closeSerialPort()));
        QStringList strType;
        strType << "" << "4AI" << "8RP" << "STEEL" << "6DI6RO";
        QString mess = "Finish update module " + QString::number(curDiagnostDevice)\
                + " | Type: " + strType.at((listDevice.at(curDiagnostDevice-1)->deviceType)%strType.size());
        log->addMess(mess, cLogger::SERVICE, cLogger::DEVICE);
        closeSerialPort();
    }
}

void dMenu::on_selectDigitInputAction_currentIndexChanged(int index)
{

}

void dMenu::on_selectDigitInputMode_currentIndexChanged(int index)
{
    ui->stackedWidgetTiming->setCurrentIndex(index);
}
