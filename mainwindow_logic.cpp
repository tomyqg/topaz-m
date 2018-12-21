#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "Channels/channelOptions.h"
#include "uartdriver.h"
#include "stackedoptions.h"
#include "worker.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"
#include "registersmap.h"
#include "Channels/group_channels.h"
#include "device_slot.h"

#include <filemanager.h>
#include <assert.h>

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPen>
#include <QVector>
#include <QFile>
#include <QDataStream>

#ifndef Q_OS_WIN32
#include <QtScript/QScriptEngine>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/input.h>
#endif

#include <QtSerialPort/QtSerialPort>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QPoint>
#include <QMetaType>
#include "defines.h"

//#include <QKeyEvent>

int dateindex;
int timeindex;
QStringList datestrings, timestrings;
cChannelSlotController csc;
cSteelController ssc;
QDateTime timeOutBuff;
cUsbFlash * flash;

extern MainWindow * globalMainWin;
extern QList<cDevice*> listDevice;
extern QList<ChannelOptions *> listChannels;
extern QList<Ustavka *> listUstavok;
extern QList<cRelay*> listRelais;
extern QList<cSteel*> listSteel;
extern QList<cGroupChannels*> listGroup;
extern cIpController * ethernet;
//extern tDeviceBasicParams g_deviceDataStorage;

void MainWindow::MainWindowInitialization()
{
    //qRegisterMetaType<Transaction>("Transaction");
    // подготовка окна загрузки программы
    countLoader = 0;
    ui->right->hide();
    ui->left->hide();
    ui->right->setMaximumWidth(0);
    ui->left->setMaximumWidth(0);
    ui->header->hide();
    ui->footer->hide();
    ui->splash->setMaximumWidth(10000000);
    QString ver = CURRENT_VER;
    ui->labelNameVersion->setText(QString("MULTIGRAPH " + ver));
    QString revision = SOFTWARE_REVISION;
    ui->labelRevision->setText(QString("SOFTWARE REVISION: " + revision));
    ui->splash->show();
    ui->frameSteel->setMaximumWidth(0);
    ui->frameSteel->hide();
    connect(&timerLoad, SIGNAL(timeout()), this, SLOT(tickLoadWidget()));
    timerLoad.start(20);
    QPixmap pixLoad(pathtologotip);
    ui->logoOnLoad->setPixmap(pixLoad);


//    ui->unvisible_block->setHidden(true);

    datestrings.append("dd.MM.yyyy ");
    datestrings.append("MM.dd.yyyy ");
    datestrings.append("MM-dd-yyyy ");
    datestrings.append("dd-MM-yyyy ");
    datestrings.append("dd/MM/yyyy ");
    datestrings.append("MM/dd/yyyy ");
    datestrings.append("yyyy-MM-dd ");

    timestrings.append("hh:mm:ss ");
    timestrings.append("hh.mm.ss ");
    timestrings.append("hh,mm,ss ");

    dateindex = 0 ;

    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("VISION"));

    ethernet = new cIpController(ETH_NAME);

    scene = new QGraphicsScene();   // Init graphic scene

    // находим все com - порты
//    int portIndex = 0;
//    int i = 0;
//    QSettings s;
//    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
//    {
//                qDebug() << port.portName;
//        if( port.friendName == s.value( "serialinterface" ) )
//        {
//            portIndex = i;
//        }
//        ++i;
//    }

    QPixmap pix(pathtologotip);
    ui->logo->setPixmap(pix);
    ui->logo->setScaledContents(true);

    // нужно установить евент филтер чтобы отрисовывалась графика
    ui->MessagesWidget->installEventFilter(this);
    ui->customPlot->installEventFilter(this); // если закоментить то не будет уставок и цифр внизу
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)),\
            this, SLOT(plotPress(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)),\
            this, SLOT(plotReleas(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)),\
            this, SLOT(plotMove(QMouseEvent*)));
    connect(&timerScale, SIGNAL(timeout()), this, SLOT(updateAutoScale()));
//    timerScale.setInterval(1);

    SetXRange(XRange);
    SetYRange(YRange);
    mouseOnScalede = false;
    mouseOnMove = false;
    waitAutoScale = false;


    QList<QPushButton*> ButtonList = findChildren<QPushButton*> ();
    // добавляем все кнопошки в евентфильтр
    for (int i = 0; i < ButtonList.count(); ++i) {
        QPushButton *but = ButtonList.at(i);
        if(but->objectName() != "buttonInputsGraphs")
        {
            but->installEventFilter(this);
        }
    }

    // инициализация каналов (входов)
    InitChannels();

    // инициализация уставок
    InitUstavka();

    ui->customPlot->yAxis->setRange(-GetXRange(), GetXRange());
    ui->customPlot->setAntialiasedElements(QCP::aeNone);

    //инициализация журнала событий
    logger = new cLogger(pathtomessages, cLogger::DEVICE);
    logger->addMess("Programm Started", cLogger::STATISTIC);
    //    messwrite.LogAddMessage("Programm Started");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDateLabel()));
    timer->start(DateLabelUpdateTimer);

    displayrefreshtimer = new QTimer();
//    displayrefreshtimer->setInterval(3000);
    displayrefreshtimer->start(30000);
    connect( displayrefreshtimer, SIGNAL(timeout()), this, SLOT(RefreshScreen()));
    RefreshScreen();

    QTimer *tmrarchive = new QTimer(this);
    connect(tmrarchive, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));
    tmrarchive->start(ArchiveUpdateTimer);

    tmr = new QTimer();
    connect(tmr, SIGNAL(timeout()), this, SLOT(AddValuesToBuffer()));
    tmr->start(ValuesUpdateTimer);// этот таймер тоже за обновление значений (частота запихивания значений в буфер, оставить пока так должно быть сто

    UpdateGraficsTimer = new QTimer(this);
    connect(UpdateGraficsTimer, SIGNAL(timeout()), this, SLOT(UpdateGraphics()));
    UpdateGraficsTimer->start(GraphicsUpdateTimer); // этот таймер отвечает за обновление графика (частота отрисовки графика) должно быть 100-200 милисекунд


    //инициализация виртуальных копий устройств
    InitDevices();

    //инициализация системных таймеров
    InitTimers();

    //
    LabelsInit();

    // инициализация таблицы реле-слот
    InitRelaySlotTable();

    //инициализация таблицы сталь-слот
    InitSteelSlotTable();


    // получение значений уставок из файла
//    ReadUstavkiFromFile();

    //перехват событий на стрелках переключения групп
    ui->arrowGroupLeft->installEventFilter(this);
    ui->arrowGroupRight->installEventFilter(this);
    // добавление 1 группы (обязательной)
    cGroupChannels * group = new cGroupChannels();
    group->groupName = "Group 1";
    group->channel[0] = listChannels.at(0);
    group->typeInput[0] = 1;
    group->channel[1] = listChannels.at(1);
    group->typeInput[1] = 1;
    group->channel[2] = listChannels.at(2);
    group->typeInput[2] = 1;
    group->channel[3] = listChannels.at(3);
    group->typeInput[3] = 1;
//    group->channel[3] = listChannels.at(3);
//    group->typeInput[3] = 1;
    group->enabled = true;
    listGroup.append(group);
    curGroupChannel = 0;        //по-умолчанию активная группа - первая

    //инициализация параметров измерения стали
    initSteel();
    slotSteelOnline = false;
    slotAnalogOnline = false;
//    steelReady = false;
//    steelMeasure = false;
    stateWidgetSteel = STEEL_WAIT;
    steelReadyNum = 0;
    indexSteel = 0;
    timerUpdateSteel = new QTimer(this);
    timerUpdateSteel->start(UpdateSteelTime);
    connect(timerUpdateSteel, SIGNAL(timeout()), this, SLOT(updateSteel()));
    ui->plotSteel->yAxis2->setVisible(true);
    ui->plotSteel->yAxis2->setTickLabels(true);
    ui->frameSteelStatus->setMaximumWidth(1000000000);
    ui->framePlotSteel->setMaximumWidth(100000000);
    ui->frameTemperature->setMaximumWidth(100000000);

    steelSelectFrame = false;
    ui->buttonInputsGraphs->setColorText(ColorBlue);
    ui->buttonInputsGraphs->setColorBg(QColor(0xf0,0xf0,0xf0));
    ui->buttonInputsGraphs->setFontSize(16);
    cFileManager::readSteelsSettings(pathtosteeloptions);

    SetWindowWidthPixels(1024);
    SetWindowHeightPixels(768);

    mQTr = new QMutex();
    timerQueueTrans = new QTimer();
    connect(timerQueueTrans, SIGNAL(timeout()), this, SLOT(parseWorkerReceive()));
    timerQueueTrans->start(ParsingReceiveTrans);


    // Инициализация потока Worker ---------------------
    WorkerThread = new QThread;
    worker* myWorker = new worker;
    connect(myWorker, SIGNAL(ModbusConnectionError()), this, SLOT(ModbusConnectionErrorSlot()) );
    myWorker->moveToThread(WorkerThread);
    connect(WorkerThread, SIGNAL(started()), myWorker, SLOT(run()));
//    connect(ui->EcoCheckBox, SIGNAL(clicked(bool)), this, SLOT(ChangePalette(bool)) );
    connect(this, SIGNAL(sendTransToWorker(Transaction)), myWorker, SLOT(getTransSlot(Transaction)), Qt::DirectConnection);
    connect(myWorker, SIGNAL(sendTrans(Transaction)), this, SLOT(getTransFromWorkerSlot(Transaction)), Qt::DirectConnection);
    connect(myWorker, SIGNAL(sendMessToLog(QString)), this, SLOT(WorkerMessSlot(QString)), Qt::DirectConnection);
//    connect(sc, SIGNAL(sendRequest(Transaction)), myWorker, SLOT(getTransSlot(Transaction)), Qt::DirectConnection);
    WorkerThread->start(QThread::LowPriority); // запускаем сам поток
    // /Инициализация потока Worker ---------------------

//    Options op;
//    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)
//    op.deleteLater();

    //Получение системных настроек из файла
    updateSystemOptions();

    // сразу активируем отладку по USB
    QProcess process;
    process.startDetached("ifconfig usb0 192.168.1.115");

    // включаем эко режим
    SetEcoMode(false);
    ClearPolarCoords();


    //инициализация архиватора

    QListIterator<ChannelOptions*> li(listChannels);
    arch = new cArchivator(pathtoarchivedata, li);

    //инициализация Anybus
    //вероятно придётся вынести в отдельный поток
    timerAnybusEv = new QTimer();
//    connect(timerAnybusEv, SIGNAL(timeout()), this, SLOT(askAnybusIRQ()));
//    timerAnybusEv->start(1000);  //проверка IRQ каждые 100 мс

    //инициализация баров
    ui->wBar_1->changeNum(1);
    ui->wBar_2->changeNum(2);
    ui->wBar_3->changeNum(3);
    ui->wBar_4->changeNum(4);
    connect(ui->wBar_1, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_2, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_3, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_4, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    setStyleBars();
    setTextBars();

    //инициализация виджетов значений
    ui->widgetVol1->changeNum(1);
    ui->widgetVol2->changeNum(2);
    ui->widgetVol3->changeNum(3);
    ui->widgetVol4->changeNum(4);
    updateWidgetsVols();

//    ui->wBar_1->setBarDiapazon(1000);
//    ui->wBar_2->setBarDiapazon(200);

#ifndef Q_OS_WIN
    //запуск Хост приложения Anybus
//    comm = new cCommunicator();
//    commRun = new QTimer();
//    connect(commRun, SIGNAL(timeout()), comm, SLOT(run()));
//    commRun->start(TIMEOUT_COMMUNICATOR_MS);
#endif

    ui->stackedWidget->setCurrentIndex(1);

    //инициализация сервера HTTP
    QNetworkProxyFactory::setUseSystemConfiguration(false); //не использовать системный прокси
//    server = new cServerHttp(this);   //создание сервера

    flash = new cUsbFlash(this);



}

static QString descriptiveDataTypeName( int funcCode )
{
    switch( funcCode )
    {
    case _FC_READ_COILS:
    case _FC_WRITE_SINGLE_COIL:
    case _FC_WRITE_MULTIPLE_COILS:
        return "Coil (binary)";
    case _FC_READ_DISCRETE_INPUTS:
        return "Discrete Input (binary)";
    case _FC_READ_HOLDING_REGISTERS:
    case _FC_WRITE_SINGLE_REGISTER:
    case _FC_WRITE_MULTIPLE_REGISTERS:
        return "Holding Register (16 bit)";
    case _FC_READ_INPUT_REGISTERS:
        return "Input Register (16 bit)";
    default:
        break;
    }
    return "Unknown";
}

void MainWindow::LabelsInit()
{
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();
    // очищаем dates

    Dates.clear();
    Labels.clear();

    int half = GetTotalLabelsCount()/2;
    int timeperiodseconds = GetTimePeriodSecs();

    for (int var = 0; var <= GetTotalLabelsCount(); ++var) {
        Dates.append(fisttime.addSecs((var-half)*timeperiodseconds));// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels.append(fisttime.addSecs((var-half)*timeperiodseconds).toString("hh:mm:ss") );
    }
}

/*
 * Создание уставок и упаковка в список
 * */
void MainWindow::InitUstavka()
{
    // установки сигнал/слотов для новых уставок
    for(int i = 0; i < listUstavok.size(); i ++)
    {
        Ustavka * ust = listUstavok.at(i);
        connect(ust, SIGNAL(workReleSignal(int, bool)), this, SLOT(sendRelayStateToWorker(int, bool)));
    }

    // запуск обновления уставок по таймеру
    timeUpdUst = new QTimer();
    connect(timeUpdUst, SIGNAL(timeout()), this, SLOT(UpdUst()));
    timeUpdUst->start(UstavkiUpdateTimer);
}

void MainWindow::UpdUst()
{
//    QList<ChannelOptions *> ChannelsObjectsList;
//    ChannelsObjectsList.append(&channel1);
//    ChannelsObjectsList.append(&channel2);
//    ChannelsObjectsList.append(&channel3);
//    ChannelsObjectsList.append(&channel4);

    foreach (Ustavka * ust, listUstavok) {
        int ch = ust->getChannel();
        int i = 0;
        if(ch)
        {
            ChannelOptions * channel = listChannels.at(ch-1);
            ust->update(channel->GetCurrentChannelValue());
//            ust->setNameCh(channel->GetChannelName());
//            ust->setNum(i+1);
//            ust->setIdentifikator("Limit" + QString::number(i+1));
        }
        i++;
    }
}

//-----Временная реализация соединения слотов----
void MainWindow::InitChannelSlotTable()
{
    csc.addChannelSlot(0, 0, CONST_SLAVE_ADC);
    csc.addChannelSlot(1, 1, CONST_SLAVE_ADC);
    csc.addChannelSlot(2, 2, CONST_SLAVE_ADC);
    csc.addChannelSlot(3, 3, CONST_SLAVE_ADC);
//    csc.addChannelSlot(4, 0, CONST_SLAVE_ADC_2);
//    csc.addChannelSlot(5, 1, CONST_SLAVE_ADC_2);
//    csc.addChannelSlot(2, 2, CONST_SLAVE_ADC_2);
//    csc.addChannelSlot(3, 3, CONST_SLAVE_ADC_2);
}

void MainWindow::InitRelaySlotTable()
{
//    //временный механизм создания связей
//    rsc.addRelaySlot(0, 0, CONST_SLAVE_RELAY);
//    rsc.addRelaySlot(1, 1, CONST_SLAVE_RELAY);
//    rsc.addRelaySlot(2, 2, CONST_SLAVE_RELAY);
//    rsc.addRelaySlot(3, 3, CONST_SLAVE_RELAY);
//    rsc.addRelaySlot(4, 7, CONST_SLAVE_RELAY);
//    rsc.addRelaySlot(5, 6, CONST_SLAVE_RELAY);
//    rsc.addRelaySlot(6, 5, CONST_SLAVE_RELAY);
//    rsc.addRelaySlot(7, 4, CONST_SLAVE_RELAY);
    for(int i = 0; i < TOTAL_NUM_RELAIS; i++)
    {
        cRelay * relay = new cRelay(i, CONST_SLAVE_RELAY);
        connect(relay, SIGNAL(signalSwitch(uint8_t, uint8_t, bool)), this, SLOT(slotRelay(uint8_t, uint8_t, bool)));

        listRelais.append(relay);
    }
}

void MainWindow::InitSteelSlotTable()
{
    ssc.addSteelSlot(0, 0, CONST_SLAVE_STEEL);
    ssc.addSteelSlot(1, 1, CONST_SLAVE_STEEL);
    ssc.addSteelSlot(2, 2, CONST_SLAVE_STEEL);
    ssc.addSteelSlot(3, 3, CONST_SLAVE_STEEL);

}

//-----/Временная реализация соединения слотов----


/*
 * слот передачи сигнала о состоянии реле в Worker
*/
void MainWindow::sendRelayStateToWorker(int relay, bool state)
{

    listRelais.at(relay)->setState(state);

//    // номер реле -> индекс реле
//    relay -= 1;

//    // получить номер слота платы реле
////    int slot = rsc.getSlotByRelay(relay);
//    int slot = listRelais.at(relay)->mySlot;

//    // получить индекс реле на плате (канал)
////    int devRelay = rsc.getDevRelay(relay);
//    int devRelay = listRelais.at(relay)->myPhysicalNum;

//    // определение адреса параметра в соответствии с интексом реле
//    // это пока временная реализация --------------
//    uint32_t relayOffset;
//    if(devRelay%2)
//    {
//        relayOffset = ChannelOptions::chanReleyHi;
//    } else {
//        relayOffset = ChannelOptions::chanReleyLow;
//    }
//    //---------------------------------------------

//    uint16_t offset = getDevOffsetByChannel(devRelay>>1, relayOffset);

//    Transaction tr(Transaction::W, slot, offset, 0);
//    // значение 1 в регистре замыкает реле
//    // тут нужно вставить инверсию, если выход нормально замкнут
//    if(state) tr.volInt = 1;
//    else tr.volInt = 0;
//#ifdef DEBUG_RELAY
//    qDebug() << "Relay:" << relay << "(DevRalay:" << devRelay << ")" << "=" << state;
//#endif
//    emit sendTransToWorker(tr);
}

void MainWindow::InitPins()
{
#ifdef MYD // если плата MYD то ничего нам с пинами инициализировать не нужно.
    return;
#endif
}

void MainWindow::OpenMessagesWindow()
{
//    messwrite.WriteAllLogToFile();
//    Messages *messages = new Messages;
//    connect( messages, SIGNAL(destroyed(QObject*)), this, SLOT( destroyedslot(QObject*)) ); //
//    this->resizeWindow(*messages,this->GetWindowWidthPixels(),this->GetWindowHeightPixels());
//    messages->setModal(true);
//    messages->exec();
//    messages->deleteLater();
}


void MainWindow::DelaySec(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(n);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::OpenOptionsWindow( int index )
{
//    // Vag: сохранить все настройки девайсов и каналов в файл перед открытием окна настроек
//    WriteAllChannelsOptionsToFile();

//    //здесь запускаем меню обновленное как в эндресе
//    StackedOptions * sw= new StackedOptions(index, 0);

//    sw->exec();

//    //читаем системные настройки из файла и применяем тут же
//    Options op;
//    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)
//    op.deleteLater();

//    // получение значений уставок из файла
//    ReadUstavkiFromFile();

//    //читаем параметры каналов прямо после закрытия окна настроек и перехода в меню режима работы
//    channel1.ReadSingleChannelOptionFromFile(1);
//    channel2.ReadSingleChannelOptionFromFile(2);
//    channel3.ReadSingleChannelOptionFromFile(3);
//    channel4.ReadSingleChannelOptionFromFile(4);
//    // после чтения параметров сразу запихиваем их в сигнал для воркера (передаем воркеру значения каждого канала )
//    sendConfigChannelsToSlave();

//    //если вдруг поменялось время то нужно обновить лейблы
//    LabelsInit();
//    LabelsCorrect();
//    sw->deleteLater();
//    resizeSelf(1024,768);

////    SetPolarAngle(0);

//    return;
}

void MainWindow::OpenArchiveWindow()
{
    OpenOptionsWindow(23);
}


void MainWindow::OpenWorkWindow()
{
    OpenOptionsWindow(2);
}

void MainWindow::PowerOff()
{
    QProcess process;
    process.startDetached("poweroff");
    QApplication::exit();
}

void MainWindow::CloseApplication()
{
    QApplication::exit();
}

void MainWindow::InitProcessorMaxFreq()
{
    // an object for make terminal requests
    QProcess process;

    // the maximum processor speed
    process.startDetached("sudo cpufreq-set -f 1000MHz");
}

void MainWindow::InitProcessorMinFreq()
{
    // an object for make terminal requests
    QProcess process;
    // the maximum processor speed
    process.startDetached("sudo cpufreq-set -f 300MHz");
    process.startDetached("sudo cpufreq-set --governor powersave"); // min perfomance on
}

void MainWindow::InitDevices()
{
    for(int i = 0; i < TOTAL_NUM_DEVICES; i++)
    {
        cDevice * device = new cDevice();
        connect(device, SIGNAL(updateParam(Transaction)), this, SLOT(retransToWorker(Transaction)));
        listDevice.append(device);
    }
}

void MainWindow::InitChannels()
{
    for(int i = 0; i < NUM_CHAN_DEFAULT; i++)
    {
        ChannelOptions * ch = new ChannelOptions();
        ch->SetCurrentChannelValue(0);
        ch->setNum(i+1);
        ch->setSlot(CONST_SLAVE_ADC);     //
        ch->setSlotChannel(i);
        connect(ch, SIGNAL(updateSignal(int)), this, SLOT(updateChannelSlot(int)));
//        ch->SetMeasurePeriod(1000);
        listChannels.append(ch);
    }

    //чтение настроек каналов и уставок
    int countChannels = listChannels.size();
    cFileManager::readChannelsSettings(pathtooptions);
    int newCountChannels = listChannels.size();
    for(int i = countChannels; i < newCountChannels; i++)
    {
        connect(listChannels.at(i), SIGNAL(updateSignal(int)), this, SLOT(updateChannelSlot(int)));
    }
}

void MainWindow::InitTimers()
{
//    channeltimer1 = new QTimer();
//    channeltimer2 = new QTimer();
//    channeltimer3 = new QTimer();
//    channeltimer4 = new QTimer();
    archivetimer  = new QTimer();
    halfSecondTimer  = new QTimer();
    timerUpdateDevices = new QTimer();

//    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateChannel1Slot()));
//    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateChannel2Slot()));
//    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateChannel3Slot()));
//    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateChannel4Slot()));

    connect(halfSecondTimer, SIGNAL(timeout()), this, SLOT(HalfSecondGone()));
    connect(timerUpdateDevices, SIGNAL(timeout()), this, SLOT(updateDevicesComplect()));
    updateDevicesComplect();
//    channeltimer1->start(1000);
//    channeltimer2->start(1000);
//    channeltimer3->start(1000);
//    channeltimer4->start(1000);
    halfSecondTimer->start(500);
    archivetimer->start(6000); // каждые 6 минут записываем архив на флешку
    timerUpdateDevices->start(timeUpdateDevices);
}

void MainWindow::InitTouchScreen()
{
    // an object for make terminal requests
    QProcess process;
    // the initial touchscreen
    process.startDetached("sudo xinput set-prop 7 \"Evdev Axis Calibration\" 3383 3962 234 599"); // вручную ввели координаты тача
}

void MainWindow::DateUpdate() // каждую секунду обновляем значок времени
{
    QDateTime local(QDateTime::currentDateTime());
    ui->date_label->setText(local.date().toString(datestrings.at(dateindex)));
    ui->time_label->setText(local.time().toString(timestrings.at(0)));
    resizeSelf(1024,768);
}

void MainWindow::LabelsUpdate()
{
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();

    int i = 0;
    while(i<=GetTotalLabelsCount()) {
        Dates[i] = Dates[i].addSecs(GetTimePeriodSecs());// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels[i] = Dates.at(i).toString("hh:mm:ss"); //Dates.at(i).toString("hh:mm:ss");
        i++;
    }
}

void MainWindow::LabelsCorrect()
{
    QDateTime currnttime;
    qint16 timecorrectsec;
    currnttime = QDateTime::currentDateTime();
    int middlelabelnumber = Dates.length()/2;
    timecorrectsec = Dates[middlelabelnumber].secsTo(currnttime);

    int i = 0;
    while(i<=GetTotalLabelsCount())
    {
        Dates[i] = Dates[i].addSecs(timecorrectsec);// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels[i] = Dates.at(i).toString("hh:mm:ss"); //Dates.at(i).toString("hh:mm:ss");
        i++;
    }
}

void MainWindow::ModbusConnectionErrorSlot()
{
    //qDebug() << "Sss" ;
    QMessageBox::critical( this, tr( "Connection Error" ),
                           tr( "Could not connect serial port!" ) );
}

void MainWindow::SetEcoMode(bool EcoMode)
{
    this->EcoMode = EcoMode;
    QColor newlabelscolor;
    if (!this->EcoMode)
    {
        ui->customPlot->setBackground(QBrush(NotEcoColor));
        newlabelscolor = ColorButtonNormal; //QColor(Qt::black);
    }
    else
    {
        ui->customPlot->setBackground(QBrush(EcoColor));
        newlabelscolor = QColor(Qt::white);
    }

    ui->customPlot->xAxis->setTickLabelFont(QFont(Font, 12, QFont::ExtraBold));
    ui->customPlot->yAxis->setTickLabelFont(QFont(Font, 12, QFont::ExtraBold));
    ui->customPlot->xAxis->setTickLabelColor(newlabelscolor);
    ui->customPlot->yAxis->setTickLabelColor(newlabelscolor);
}

void MainWindow::updateDevicesComplect()
{
    QList<uint8_t> list4AI;
    QList<uint8_t> list8RP;
    QList<uint8_t> listSTEEL;
    foreach (cDevice * device, listDevice) {
        if(device->getOnline()){
            switch(device->deviceType)
            {
            case Device_4AI:
                list4AI.append(device->getSlot());
                list4AI.append(device->getSlot());
                list4AI.append(device->getSlot());
                list4AI.append(device->getSlot());
                break;
            case Device_8RP:
                list8RP.append(device->getSlot());
                list8RP.append(device->getSlot());
                list8RP.append(device->getSlot());
                list8RP.append(device->getSlot());
                list8RP.append(device->getSlot());
                list8RP.append(device->getSlot());
                list8RP.append(device->getSlot());
                list8RP.append(device->getSlot());
                break;
            case Device_STEEL:
                listSTEEL.append(device->getSlot());
                listSTEEL.append(device->getSlot());
                break;
            default:
                cLogger mk(pathtomessages, cLogger::DEVICE);
                QString strSlot = QString::number(device->getSlot());
                QString strtype = QString::number(device->deviceType);
                mk.addMess("Slot " + strSlot + " | Device unidentified | type "\
                           + strtype, cLogger::WARNING);
                break;
            }
        }
    }
    // обновление списка каналов
    int i = 0;
    foreach (uint8_t slot, list4AI) {
        if(i < listChannels.size())
        {
            //обновление параметров каналов
            ChannelOptions * ch = listChannels.at(i);
            ch->setSlot(slot);     //
            ch->setSlotChannel(i%4);
            ch->enable = true;
        }
        else
        {   // добавить каналы, если плат стало больше
            ChannelOptions * ch = new ChannelOptions();
            ch->SetCurrentChannelValue(0);
            ch->setNum(i+1);
            ch->setSlot(slot);     //
            ch->setSlotChannel(i%4);
            connect(ch, SIGNAL(updateSignal(int)), this, SLOT(updateChannelSlot(int)));
            listChannels.append(ch);
        }
        i++;
    }
    if(listChannels.size() > list4AI.size())
    {
        // плат стало меньше, тогда временно отключем каналы, но не удаляем
        for(int i = list4AI.size(); i < listChannels.size(); i++)
        {
            listChannels.at(i)->enable = false;
        }
    }

    // обновление списка реле
    i = 0;
    foreach (uint8_t slot, list8RP) {
        if(i < listRelais.size())
        {
            //обновление параметров реле
            cRelay * r = listRelais.at(i);
            r->mySlot = slot;     //
            r->myPhysicalNum = i%8;
        }
        else
        {   // добавить каналы, если соответствующих плат стало больше
            cRelay * r = new cRelay(i%8, slot);
//            r->mySlot = slot;     //
//            r->myPhysicalNum = i%8;
            connect(r, SIGNAL(signalSwitch(uint8_t,uint8_t,bool)), this, SLOT(slotRelay(uint8_t,uint8_t,bool)));
            listRelais.append(r);
        }
        i++;
    }
    if(listRelais.size() > list8RP.size())
    {
        // плат стало меньше, тогда удаляем объекты
        for(int i = list8RP.size(); i < listRelais.size(); i++)
        {
            listRelais.removeAt(i);
        }
    }

    // обновление списка входных групп анализа стали
    i = 0;
    foreach (uint8_t slot, listSTEEL) {
        if(i < listSteel.size())
        {
            //обновление параметров реле
            cSteel * s = listSteel.at(i);
            s->slot = slot;     //
            s->slotIndex = i%2;
            s->enable = true;
        }
        else
        {   // добавить каналы, если соответствующих плат стало больше
            cSteel * s = new cSteel(this);
            s->slot = slot;     //
            s->slotIndex = i%2;
            s->enable = true;
            s->technology = NULL;
            listSteel.append(s);
        }
        i++;
    }
    if(listSteel.size() > listSTEEL.size())
    {
        // плат стало меньше, тогда временно отключем каналы, но не удаляем
        for(int i = listSTEEL.size(); i < listSteel.size(); i++)
        {
            listSteel.at(i)->enable = false;
        }
    }
}



bool MainWindow::GetEcoMode()
{
    return this->EcoMode;
}

void MainWindow::HalfSecondGone()
{
    if (halfSecondflag == 1)
        halfSecondflag = 0;
    else
        halfSecondflag = 1;
}

uint8_t MainWindow::GetHalfSecFlag()
{
    return halfSecondflag;
}

int MainWindow::GetPolarAngle()
{
    return polar_angle;
}

void MainWindow::ClearPolarCoords()
{
    SetPolarAngle(0);
    PolarChartPointsChannel1.clear();
    PolarChartPointsChannel2.clear();
    PolarChartPointsChannel3.clear();
    PolarChartPointsChannel4.clear();
}

void MainWindow::SetPolarAngle(int newangle)
{
    polar_angle = newangle;
}

//void MainWindow::NewTouchscreenCalibration()
//{
//    QProcess process;
//    process.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" " + Options::calibrationprm); // каждую секунду вводим координаты тача вдруг чтобы не отвалился
//}

void MainWindow::paintEvent(QPaintEvent *)
{
    int i = 0;
    i++;
}

void MainWindow::GetAllUartPorts()
{
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    QString infosss;
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        infosss += serialPortInfo.portName();
    }
}

void MainWindow::logginStates(int channel, QString mess)
{
//    ChannelOptions * ch = listCh.at(channel);
//    double cur = ch->GetCurrentChannelValue();
//    QString channelstringvalue = (QString::number( cur, 'f', 3)) + ch->GetUnitsName();
//    //messwrite.LogAddMessage (ch->GetChannelName() + ":" + mess + ":" + channelstringvalue);
//    logger->addMess(ch->GetChannelName() + ":" + mess + ":" + channelstringvalue,\
//                    cLogger::LIMIT, cLogger::USTAVKA);

}

void MainWindow::newUstavkaConnect(int num)
{
    Ustavka * ust = listUstavok.at(num);
    connect(ust, SIGNAL(workReleSignal(int, bool)), this, SLOT(sendRelayStateToWorker(int, bool)));
}

void MainWindow::slotRelay(uint8_t sl, uint8_t num, bool state)
{

    Transaction tr(Transaction::W);
    int index = num >> 1;
    int level = num & 1;
    QString strLevel = (level ? "ReleyLo" : "ReleyHi");
    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(index) + strLevel);
    tr.slave = sl;
    tr.volInt = state;
#ifdef DEBUG_RELAY
    qDebug() << "Relay:" << num << "=" << state;
#endif
    emit sendTransToWorker(tr);
}

void MainWindow::logginSteel(int numSteel)
{
    cSteel * steel = listSteel.at(numSteel);
    QString string;
    string = string + steel->technology->name + \
            " " + QString::number(steel->temp, 'f', 0) + "°C" + \
            " " + QString::number(steel->eds, 'f', 0) + "mV" + \
            " " + QString::number(steel->ao, 'f', 0) + "ppm" +\
            steel->timeUpdateData.toString(" hh:mm:ss");
    logger->addMess(string);
}

void MainWindow::devicesPause(bool f)
{
    foreach (cDevice * dev, listDevice) {
        dev->pause(f);
    }
}

extern "C" {
void busMonitorAddItem( uint8_t isRequest, uint8_t slave, uint8_t func, uint16_t addr, uint16_t nb, uint16_t expectedCRC, uint16_t actualCRC )
{

}

void busMonitorRawData( uint8_t * data, uint8_t dataLen, uint8_t addNewline )
{

}
}

void MainWindow::ChangePalette(bool i)
{

    QColor Channel1Color;
    QColor Channel2Color;
    QColor Channel3Color;
    QColor Channel4Color;
    QColor Channel1ColorNormal;
    QColor Channel2ColorNormal ;
    QColor Channel3ColorNormal;
    QColor Channel4ColorNormal ;

    QColor Channel1ColorMaximum,Channel2ColorMaximum,Channel3ColorMaximum,Channel4ColorMaximum;
    QColor Channel1ColorMinimum,Channel2ColorMinimum,Channel3ColorMinimum,Channel4ColorMinimum;

//    if (ui->EcoCheckBox->checkState())
//    {
//        Channel1Color = Channel1ColorNormal = ColorCh1Light;
//        Channel2Color = Channel2ColorNormal = ColorCh2Light;
//        Channel3Color = Channel3ColorNormal = ColorCh3Light;
//        Channel4Color = Channel4ColorNormal = ColorCh4Light;
//    }
//    else
//    {
//        Channel1Color = Channel1ColorNormal = ColorCh1;
//        Channel2Color = Channel2ColorNormal = ColorCh2;
//        Channel3Color = Channel3ColorNormal = ColorCh3;
//        Channel4Color = Channel4ColorNormal = ColorCh4;
//    }

    Channel1ColorMaximum = QColor(Channel1Color.red() - 20,Channel1Color.green() - 20,Channel1Color.blue() - 20);
    Channel2ColorMaximum = QColor(Channel2Color.red() - 20,Channel2Color.green() - 20,Channel2Color.blue() - 20);
    Channel3ColorMaximum = QColor(Channel3Color.red() - 20,Channel3Color.green() - 20,Channel3Color.blue() - 20);
    Channel4ColorMaximum = QColor(Channel4Color.red() - 20,Channel4Color.green() - 20,Channel4Color.blue() - 20);

    Channel1ColorMinimum = QColor(Channel1Color.red() + 20,Channel1Color.green() + 20,Channel1Color.blue() + 20);
    Channel2ColorMinimum = QColor(Channel2Color.red() + 20,Channel2Color.green() + 20,Channel2Color.blue() + 20);
    Channel3ColorMinimum = QColor(Channel3Color.red() + 20,Channel3Color.green() + 20,Channel3Color.blue() + 20);
    Channel4ColorMinimum = QColor(Channel4Color.red() + 20,Channel4Color.green() + 20,Channel4Color.blue() + 20);

//    channel1.SetNormalColor(Channel1ColorNormal);
//    channel2.SetNormalColor(Channel2ColorNormal);
//    channel3.SetNormalColor(Channel3ColorNormal);
//    channel4.SetNormalColor(Channel4ColorNormal);

//    channel1.SetMaximumColor(Channel1ColorMaximum);
//    channel2.SetMaximumColor(Channel2ColorMaximum);
//    channel3.SetMaximumColor(Channel3ColorMaximum);
//    channel4.SetMaximumColor(Channel4ColorMaximum);

//    channel1.SetMinimumColor(Channel1ColorMinimum);
//    channel2.SetMinimumColor(Channel2ColorMinimum);
//    channel3.SetMinimumColor(Channel3ColorMinimum);
//    channel4.SetMinimumColor(Channel4ColorMinimum);

}

void MainWindow::changeTranslator(int langindex)
{
    QApplication::removeTranslator(translator);
    translator = new QTranslator();

    switch (langindex) {
    case 0:
        translator->load("untitled2_en_EN");
        break;
    case 1:
        translator->load("untitled2_ru_RU");
        break;
    case 2:
        translator->load("untitled2_de_DE");
        break;
    default:
        return;
    }
    QApplication::installTranslator(translator);

}

void MainWindow::WorkerMessSlot(QString mess)
{
//    messwrite.LogAddMessage(mess);
    logger->addMess(mess);
}

/*
 * Преобразование адреса параметра в канале в адрес
 * параметра в девайсе с учётом карты регистров
 */
uint32_t MainWindow::getDevOffsetByChannel(int ch, uint32_t offset)
{
    return offset + 128 * ch + BASE_CHANNELS_OFFSET;
}

void MainWindow::retransToWorker(Transaction tr)
{
    emit sendTransToWorker(tr);
}

void MainWindow::getTransFromWorkerSlot(Transaction tr)
{
    mQTr->lock();
    queueTransaction.enqueue(tr);
    mQTr->unlock();
}

void MainWindow::parseWorkerReceive()
{
    timerQueueTrans->stop();
    Transaction tr;
    QString paramName;
    ChannelOptions * channel = listChannels.at(0);
    int ch;
    bool isDeviceParam = false;

    mQTr->lock();
    int counter = 0;
    while(!queueTransaction.isEmpty() && (counter++ < 4))
    {
        tr = queueTransaction.dequeue();
//        int typeDevice = sc->getTypeDevice(tr.slave);
        if((tr.slave == 0) || (tr.slave > TOTAL_NUM_DEVICES)) return;
        int indexDev = tr.slave - 1;
        cDevice * device = listDevice.at(indexDev);
        device->setOnline();
        paramName = cRegistersMap::getNameByOffset(tr.offset);
        isDeviceParam = false;
        if(tr.offset < BASE_OFFSET_DEVICE)
        {
            isDeviceParam = true;
        }
        else if((tr.offset >= BASE_OFFSET_DEVICE) && (tr.offset < BASE_OFFSET_CHANNEL_1))
        {
            isDeviceParam = true;
            device->parseDeviceParam(tr);
        }
        else if((tr.offset >= BASE_OFFSET_CHANNEL_1) && (tr.offset < BASE_OFFSET_CHANNEL_2))
            // канал 1
        {
            ch = 0;
            channel = listChannels.at(ch);
        }
        else if((tr.offset >= BASE_OFFSET_CHANNEL_2) && (tr.offset < BASE_OFFSET_CHANNEL_3))
            // канал 2
        {
            ch = 1;
            channel = listChannels.at(ch);
        }
        else if((tr.offset >= BASE_OFFSET_CHANNEL_3) && (tr.offset < BASE_OFFSET_CHANNEL_4))
            // канал 3
        {
            ch = 2;
            channel = listChannels.at(ch);
        }
        else if((tr.offset >= BASE_OFFSET_CHANNEL_4) && (tr.offset < (BASE_OFFSET_CHANNEL_4 + 128)))
            // канал 4
        {
            ch = 3;
            channel = listChannels.at(ch);
        }

        if(!isDeviceParam)
        {
//            emit retransToSlotConfig(tr);
            if(paramName == QString("chan" + QString::number(ch) + "SignalType"))
            {
                if(device->deviceType == Device_4AI)   //контроллировать источник нужно во всех "else if"
                {
                    channel->SetSignalType(tr.volInt);
                    channel->SetCurSignalType(tr.volInt);
                    emit retransToSlotConfig(tr);
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "Status"))
            {
                if(device->deviceType == Device_STEEL)
                {
                    int index = getIndexSteelBySlotAndCh(tr.slave, ch);
                    if(index != -1) listSteel.at(index)->status = tr.volInt;
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "AdditionalParameter1"))
            {
                if(device->deviceType == Device_4AI)
                {
                    if((channel->GetSignalType() == ChannelOptions::MeasureTermoResistance) ||
                            (channel->GetSignalType() == ChannelOptions::MeasureTC))
                    {
                        channel->SetDiapason(tr.paramA12[1]);
                        emit retransToSlotConfig(tr);
                    }
                    else if(channel->GetSignalType() == ChannelOptions::MeasureVoltage)
                    {
                        channel->SetDiapason(tr.paramA12[0]);
                        emit retransToSlotConfig(tr);
                    }

                }
            }
            else if((paramName == "DataChan0") || (paramName == "chan0Data"))
            {
                if(device->deviceType == Device_4AI)
                {

#ifndef RANDOM_CHAN
                    listChannels.at(0)->SetCurrentChannelValue((double)tr.volFlo);
                    ui->wBar_1->setVolue((double)tr.volFlo);
                    ui->widgetVol1->setVol((double)tr.volFlo);
#endif
                }
//                else if(device->deviceType == Device_STEEL)
//                {
//                    listSteel.at(0)->temp = tr.volFlo;
//                }
            }
            else if((paramName == "DataChan1") || (paramName == "chan1Data"))
            {
                if(device->deviceType == Device_4AI)
                {
#ifndef RANDOM_CHAN
                    listChannels.at(1)->SetCurrentChannelValue((double)tr.volFlo);
                    ui->wBar_2->setVolue((double)tr.volFlo);
                    ui->widgetVol2->setVol((double)tr.volFlo);
#endif
                }
//                else if(device->deviceType == Device_STEEL)
//                {
//                    listSteel.at(1)->temp = tr.volFlo;
//                }
            }
            else if((paramName == "DataChan2") || (paramName == "chan2Data"))
            {
                if(device->deviceType == Device_4AI)
                {
#ifndef RANDOM_CHAN
                    listChannels.at(2)->SetCurrentChannelValue((double)tr.volFlo);
                    ui->wBar_3->setVolue((double)tr.volFlo);
                    ui->widgetVol3->setVol((double)tr.volFlo);
#endif
                }
//                else if(device->deviceType == Device_STEEL)
//                {
//                    listSteel.at(2)->temp = tr.volFlo;
//                }
            }
            else if((paramName == "DataChan3") || (paramName == "chan3Data"))
            {
                if(device->deviceType == Device_4AI)
                {
#ifndef RANDOM_CHAN
                    listChannels.at(3)->SetCurrentChannelValue((double)tr.volFlo);
                    ui->wBar_4->setVolue((double)tr.volFlo);
                    ui->widgetVol4->setVol((double)tr.volFlo);
#endif
                }
//                else if(device->deviceType == Device_STEEL)
//                {
//                    listSteel.at(3)->temp = tr.volFlo;
//                }
            }
            else if((paramName == "DataChan" + QString("chan" + QString::number(ch))) || \
                    paramName == QString("chan" + QString::number(ch) + "Data"))
            {
                if(device->deviceType == Device_STEEL)
                {
                    int index = getIndexSteelBySlotAndCh(tr.slave, ch);
                    if(index != -1) listSteel.at(index)->temp = tr.volFlo;
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "OxActivity"))
            {
                if(device->deviceType == Device_STEEL)
                {
                    int index = getIndexSteelBySlotAndCh(tr.slave, ch);
                    if(index != -1) listSteel.at(index)->ao = tr.volInt & 0xFFFF;
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "MassAl"))
            {
                if(device->deviceType == Device_STEEL)
                {
                    int index = getIndexSteelBySlotAndCh(tr.slave, ch);
                    if(index != -1) listSteel.at(index)->alg = tr.volInt & 0xFFFF;
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "Carbon"))
            {
                if(device->deviceType == Device_STEEL)
                {
                    //получены все данные по площадке - это последний параметр
                    int index = getIndexSteelBySlotAndCh(tr.slave, ch);
                    if(index != -1)
                    {
                        listSteel.at(index)->allVectorsReceived = true;
                        if(tr.paramA12[0] != 0x7FFF)
                            listSteel.at(index)->cl = (float)tr.paramA12[0] / 1000;
                        else
                            listSteel.at(index)->cl = NAN;
                    }
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "PrimaryActivity"))
            {
                if(device->deviceType == Device_STEEL)
                {
                    int index = getIndexSteelBySlotAndCh(tr.slave, ch);
                    if(index != -1) listSteel.at(index)->eds = tr.volFlo;
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "ReleyHi"))
            {
                if(device->deviceType == Device_8RP)
                {
                    int num = ch << 1;
                    foreach (cRelay * r, listRelais) {
                        if((tr.slave == r->mySlot) && (r->myPhysicalNum == num) )
                        {
                            r->setCurState(tr.volInt);
                        }
                    }
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "ReleyLo"))
            {
                if(device->deviceType == Device_8RP)
                {
                    int num = (ch << 1) + 1;
                    foreach (cRelay * r, listRelais) {
                        if((tr.slave == r->mySlot) && (r->myPhysicalNum == num) )
                        {
                            r->setCurState(tr.volInt);
                        }
                    }
                }
            }
            else
            {
                emit retransToSlotConfig(tr);
            }
        }
        else    //isDeviceParam
        {

            // Vag: уместить всё внутри обработчика устройства cDevice (вверху)
            if(paramName == "deviceType")
            {
                if(tr.volInt == Device_STEEL)
                {
                    if(!slotSteelOnline)
                    {
                        //Vag: тут вставить инициализацию списка входных групп
                    }
                    slotSteelOnline = true;
                }
                else if(tr.volInt == Device_4AI)
                {
                    if(!slotAnalogOnline)
                    {
                        //Vag: тут вставить инициализацию списка входных групп
                    }
                    slotAnalogOnline = true;
                }
            }
            else if(paramName.contains("DataArray"))
            {
                if(device->deviceType == Device_STEEL)
                {
                    bool needNextArray = true;  //признак необходимости запроса следующего массива
                    int curArray = 0;
                    for(int i = 1; i <= 5; i++)
                    {   //перебор массивов по номерам в карте регистров
                        if(paramName == QString("DataArray" + QString::number(i)))
                        {   //подобран номер массива
                            listSteel.at(steelReadyNum)->vectorEdsReceived = true;
                            listSteel.at(steelReadyNum)->lastItemEds = true;
                            for(int j = 0; j < 32; j++)
                            {   //поэлементное копирование полученного массива в соответствующий объект стали
                                curArray = i;
                                int indexVec = j + (i - 1) * 32;
                                if(indexVec < SIZE_ARRAY) //проверка на всякий случай, чтобы не выйти за пределы массива
                                {
                                    if(tr.paramInt16[j] != 0x7FFF)
                                    {
                                        if(listSteel.at(steelReadyNum)->technology->COH != 0)
                                        {
                                            listSteel.at(steelReadyNum)->vectorEds.replace(indexVec, (double)tr.paramInt16[j]);
                                        }
                                    }
                                }
                            }
                            if(tr.paramInt16[31] == 0x7FFF)
                            {
                                listSteel.at(steelReadyNum)->lastItemEds = false;
                            }
                            break;
                        }
                        else if(paramName == QString("DataArray" + QString::number(i+5)))
                        {
                            listSteel.at(steelReadyNum)->vectorTempReceived = true;
                            listSteel.at(steelReadyNum)->lastItemTemp = true;
                            for(int j = 0; j < 32; j++)
                            {   //поэлементное копирование полученного массива в соответствующий объект стали
                                curArray = i;
                                int indexVec = j + (i - 1) * 32;
                                if(indexVec < SIZE_ARRAY) //проверка на всякий случай, чтобы не выйти за пределы массива
                                {
                                    if(tr.paramInt16[j] != 0x7FFF)
                                    {
                                        listSteel.at(steelReadyNum)->vectorTemp.replace(indexVec, (double)tr.paramInt16[j]);
                                    }
                                }
                            }
                            if(tr.paramInt16[31] == 0x7FFF)
                            {
                                listSteel.at(steelReadyNum)->lastItemTemp = false;
                            }
                            break;
                        }
                    }

                    //оба масива после запроса
                    if(((listSteel.at(steelReadyNum)->vectorEdsReceived) || \
                        (listSteel.at(steelReadyNum)->technology->COH == 0)) && \
                            (listSteel.at(steelReadyNum)->vectorTempReceived))
                    {
                        askNewArray = true;
                        listSteel.at(steelReadyNum)->vectorTempReceived = false;
                        listSteel.at(steelReadyNum)->vectorEdsReceived = false;
                    }

                    //если оба массива полные значениями,
                    // или только температуры при отключенной окисленности
                    //массив получен, можно запросить ещё
                    if((listSteel.at(steelReadyNum)->lastItemEds || \
                        (listSteel.at(steelReadyNum)->technology->COH == 0)) && \
                            listSteel.at(steelReadyNum)->lastItemTemp)
                    {
                        numArraySteel = curArray;
                    }
//                    if(needNextArray)
//                    {   //если массив заполнен, то переход на следующий
//                            numArraySteel = curArray;
////                                assert(numArraySteel < 5);  //номер массива не может быть равен 5 или больше
//                    }
                }
            }
            emit retransToSlotConfig(tr);
        }

    }
    mQTr->unlock();
    timerQueueTrans->start(ParsingReceiveTrans);
}

int MainWindow::getIndexSteelBySlotAndCh(int slot, int ch)
{
    int ret = -1;
    for(int i = 0; i < listSteel.size(); i++)
    {
        cSteel * s = listSteel.at(i);
        if((s->slot == slot) && (s->slotIndex == ch))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

int MainWindow::getIndexRelayBySlotAndCh(int slot, int ch)
{
    int ret = -1;
    for(int i = 0; i < listRelais.size(); i++)
    {
        cRelay * r = listRelais.at(i);
        if((r->mySlot == slot) && (r->myPhysicalNum == ch))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

bool MainWindow::isChannelInMaxNow(int ch)
{
    //индекс -> номер канала [1...]
    ch = ch + 1;

    foreach (Ustavka * u, listUstavok) {
        if(u->getChannel() == ch)
        {
            if(u->isUp()) return true;
        }
    }
    return false;
}

bool MainWindow::isChannelInMinNow(int ch)
{
    //индекс -> номер канала [1...]
    ch = ch + 1;

    foreach (Ustavka * u, listUstavok) {
        if(u->getChannel() == ch)
        {
//            if(u->isDown()) return true;
        }
    }
    return false;
}


void MainWindow::sendConfigChannelsToSlave()
{
//    QList<ChannelOptions *> ChannelsObjectsList;
//    ChannelsObjectsList.append(&channel1);
//    ChannelsObjectsList.append(&channel2);
//    ChannelsObjectsList.append(&channel3);
//    ChannelsObjectsList.append(&channel4);

    Transaction tr;
    tr.dir = Transaction::W;
    QString str;

    if(slotAnalogOnline)
    {
        for(int i = 0; i < listChannels.size(); i++)
        {
            ChannelOptions * channel = listChannels.at(i);
            int devCh = channel->getSlotChannel();// csc.getDevChannel(i);
            tr.slave = channel->getSlot(); //csc.getSlotByChannel(devCh);

            // запись актуального значения SignalType
            str = "chan" + QString::number(devCh) + "SignalType";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = channel->GetSignalType();
            emit sendTransToWorker(tr);

            // запись актуального Additional parameter1
            str = "chan" + QString::number(devCh) + "AdditionalParameter1";
            tr.offset = cRegistersMap::getOffsetByName(str);
            //        tr.volInt = 0;
            int size = sizeof(tr.paramA12);
            memset(tr.paramA12, 0, size);
            if(channel->GetSignalType() == ModBus::VoltageMeasure)
            {
                tr.paramA12[0] = (uint16_t)channel->GetDiapason();
            }
            else if(channel->GetSignalType() == ModBus::TermoResistanceMeasure)
            {
                tr.paramA12[0] = (uint16_t)channel->getShema();
                tr.paramA12[1] = (uint16_t)channel->GetDiapason();
            }
            else if((channel->GetSignalType() == ModBus::TermoCoupleMeasure))
            {
                tr.paramA12[0] = 1;
                tr.paramA12[1] = (uint16_t)channel->GetDiapason();
                tr.paramA12[2] = 2;
            }
            emit sendTransToWorker(tr);
        }
    }

    if(slotSteelOnline)
    {
        for(int i = 0; i < listSteel.size(); i++)
        {
            cSteel * st = listSteel.at(i);
            int devS = st->slotIndex;
            tr.slave = st->slot;

            // запись Time_square_temperature
            str = "chan" + QString::number(devS) + "TimeSquareTemp";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->dSt;
            emit sendTransToWorker(tr);

            // запись Range_temperature
            str = "chan" + QString::number(devS) + "RangeTemp";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->dt;
            emit sendTransToWorker(tr);

            // запись Time_measure_temperature
            str = "chan" + QString::number(devS) + "TimeMeasureTemp";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->tPt;
            emit sendTransToWorker(tr);

            // запись Low_lim_temp
            str = "chan" + QString::number(devS) + "LowTemp";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = st->technology->LPtl;
            emit sendTransToWorker(tr);

            // запись Hi_lim_temp
            str = "chan" + QString::number(devS) + "HiTemp";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = st->technology->LPth;
            emit sendTransToWorker(tr);

            // запись Sensor_Type_Activty
            str = "chan" + QString::number(devS) + "SensorType";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = st->technology->COH;
            emit sendTransToWorker(tr);

            // запись Time_square_EDS
            str = "chan" + QString::number(devS) + "TimeSquareEDS";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->dSE;
            emit sendTransToWorker(tr);

            // запись Range_EDS
            str = "chan" + QString::number(devS) + "RangeEDS";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->dE;
            emit sendTransToWorker(tr);

            // запись Time_measure_EDS
            str = "chan" + QString::number(devS) + "TimeMeasureEDS";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->tPE;
            emit sendTransToWorker(tr);

            // запись Сrystallization_temperature
            str = "chan" + QString::number(devS) + "Crystallization";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = st->technology->b1;
            emit sendTransToWorker(tr);

            // запись Mass_coefficient
            str = "chan" + QString::number(devS) + "MassCoeff";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = st->technology->b2;
            emit sendTransToWorker(tr);

            // запись Final_oxidation
            str = "chan" + QString::number(devS) + "FinalOx";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = st->technology->O;
            emit sendTransToWorker(tr);

            // запись Assimilation of aluminum
            str = "chan" + QString::number(devS) + "Assimilation";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = st->technology->Y;
            emit sendTransToWorker(tr);

            // запись Mass_melting
            str = "chan" + QString::number(devS) + "MassMelting";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volInt = st->technology->G;
            emit sendTransToWorker(tr);

            // запись Mass_melting
            str = "chan" + QString::number(devS) + "AdditionalParameter1";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.paramA12[1] = st->technology->nSt;
            emit sendTransToWorker(tr);
        }
    }
}


//void MainWindow::askAnybusIRQ()
//{
//    input_event ev;
//    unsigned int key_code;
//    size_t size;
//    int tmp;



//    tmp = open("/dev/input/event0", O_RDONLY);
//    if (tmp < 0) {
//        qDebug() << "\nOpen /dev/input/event0 failed!\n";
//        return;
//    }
//    if (read(tmp, &ev, size) < size) {
//        qDebug() << "\nReading from /dev/input/event0 failed!\n";
//        return;
//    }

////    qDebug() << "askAnybusIRQ: size =" << ev.code
////             << "value =" << ev.value
////             << "type =" << ev.type;

//    if (ev.value == 1 && ev.type == 1) {    /* Down press only */
//        key_code = ev.code;
//        if (key_code == KEY_HOME) {    /* lower speed */
//            qDebug() << "KEY_HOME";
//        } else if (key_code == KEY_ESC) {    /* raise speed */
//            qDebug() << "KEY_ESC";
//        } else {
//            qDebug() << QString::number(key_code);
//        }
//    }

////    QFile eventIRQ("/dev/input/event0");
////    if(!eventIRQ.open(QIODevice::ReadOnly))
////    {
////        qDebug() << "Error open file /dev/input/event0";
////    }

////    QTextStream event(&eventIRQ);
////    QString str = event.readAll();

////    if(str != "")
////    {
////        qDebug() << str;
////    }

////    eventIRQ.close();
//}

//void MainWindow::keyPressEvent(QKeyEvent *event)
//{
//    qDebug() << QString::number(event->key());
//    switch(event->key())
//    {
//    case Qt::Key_Escape:
//        qDebug() << "Key_Escape";
//        break;
//    case Qt::Key_Home:
//        qDebug() << "Key_Home";
//        break;
//    }
//}
