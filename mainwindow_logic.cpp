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
#include "Channels/freq_channel.h"
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
#include <QDebug>


//#include <QKeyEvent>

//int dateindex;
//int timeindex;
QStringList datestrings, timestrings;
QDateTime timeOutBuff;
cUsbFlash * flash;
QMutex mSysOpt;
QMutex mListUstvok;
QMutex mListMath;
QMutex mListChannel;
QMutex mListDev;
QMutex mListFreq;
QMutex mListRelay;

extern MainWindow * globalMainWin;
extern QList<cDevice*> listDevice;
extern QList<ChannelOptions *> listChannels;
extern QList<Ustavka *> listUstavok;
extern QList<cRelay*> listRelais;
extern QList<cSteel*> listSteel;
extern QList<cGroupChannels*> listGroup;
extern QList<cFreqChannel*> listFreq; //список частотных каналов
extern cIpController * ethernet;
extern cSystemOptions systemOptions;
//extern tDeviceBasicParams g_deviceDataStorage;

void MainWindow::MainWindowInitialization()
{
    //qRegisterMetaType<Transaction>("Transaction");
    // подготовка окна загрузки программы
    countLoader = 0;
    allDeviceStable = false;
    plotReady = false;
    ui->right->hide();
    ui->left->hide();
    ui->right->setMaximumWidth(0);
    ui->left->setMaximumWidth(0);
    ui->header->hide();
    ui->footer->hide();
    ui->splash->setMaximumWidth(16777215);

    //Получение системных настроек из файла
    updateSystemOptions();

    ui->splash->show();
    ui->frameSteel->setMaximumWidth(0);
    ui->frameSteel->hide();
    connect(&timerLoad, SIGNAL(timeout()), this, SLOT(tickLoadWidget()));
    timerLoad.start(100);
    QPixmap pixLoad(pathtologotip);
    ui->logoOnLoad->setPixmap(pixLoad);

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
    connect(this, SIGNAL(signalRestartModbus()), myWorker, SLOT(slotRestartModbus()), Qt::DirectConnection);
//    connect(sc, SIGNAL(sendRequest(Transaction)), myWorker, SLOT(getTransSlot(Transaction)), Qt::DirectConnection);
    WorkerThread->start(QThread::LowPriority); // запускаем сам поток
    // /Инициализация потока Worker ---------------------



    //инициализация виртуальных копий устройств
    InitDevices();

    //инициализация системных таймеров
    InitTimers();

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
//    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)),\
//            this, SLOT(plotPress(QMouseEvent*)));
//    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)),\
//            this, SLOT(plotReleas(QMouseEvent*)));
//    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)),\
//            this, SLOT(plotMove(QMouseEvent*)));
    connect(&timerScale, SIGNAL(timeout()), this, SLOT(updateAutoScale()));
//    timerScale.setInterval(1);
    SetXRange(XRange);
    SetYRange(YRange);
    mouseOnScalede = false;
    mouseOnMove = false;
    waitAutoScale = false;
    ui->customPlot->yAxis->setRange(-GetXRange(), 0/*GetXRange()*/);
    ui->customPlot->setAntialiasedElements(QCP::aeAll);
    pauseUpdateGraph = false;
    updateGraph = false;

    QList<QPushButton*> ButtonList = findChildren<QPushButton*> ();
    // добавляем все кнопошки в евентфильтр
    for (int i = 0; i < ButtonList.count(); ++i) {
        QPushButton *but = ButtonList.at(i);
        if(but->objectName() != "buttonInputsGraphs")
        {
            but->installEventFilter(this);
        }
    }

    /* Инициализация объектов */
    // инициализация каналов (входов)
    InitChannels();
    // инициализация частотных каналов
    InitFreq();

    //чтение настроек каналов и уставок
    int res = cFileManager::readChannelsSettings(pathtooptions);
    if(res == 4)
    {
        cLogger mk(pathtomessages, cLogger::CONFIG);
        mk.addMess("File " + QString(pathtooptions) + " is empty", cLogger::ERR);
    }
    // инициализация уставок
    InitUstavka();

    // Инициализация потока внешнего Modbus ---------------------------
    // Vag: перенести позже в отдельную функцию и выполнять при включении опции
    cExtModbus * extModbus = new cExtModbus;
    extModbusThread = new QThread;
    int typeExtModbus[3] = {cExtModbus::MB_OFF, cExtModbus::TCP, cExtModbus::RTU};
    extModbus->init(typeExtModbus[systemOptions.extModbus.type]/*cExtModbus::TCP*/);
    connect(extModbusThread, SIGNAL(started()), extModbus, SLOT(run()));
    connect(this, SIGNAL(signalToExtModbus(QString,tModbusBuffer)), extModbus, SLOT(updateData(QString,tModbusBuffer)), Qt::DirectConnection);
    connect(extModbus, SIGNAL(signalUpdateParam(QString,tModbusBuffer)), this, SLOT(slotFromExtModbus(QString,tModbusBuffer)), Qt::DirectConnection);
    connect(extModbus, SIGNAL(signalActualizeParam(QString)), this, SLOT(slotUpdateExtIntefaceData(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(signalReinitExtModbus()), extModbus, SLOT(slotReinit()), Qt::DirectConnection);
    extModbus->moveToThread(extModbusThread);
    extModbusThread->start();
    // /Инициализация потока внешнего Modbus ---------------------------

    // Инициализация внешних интерфейсов -----------------------------------------
    initExtInterface();
    // /Инициализация внешних интерфейсов -----------------------------------------


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

    //Vag: Временно(или совсем) исключаем архивацию в JSON формате
//    QTimer *tmrarchive = new QTimer(this);
//    connect(tmrarchive, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));
//    tmrarchive->start(ArchiveUpdateTimer);

    tmr = new QTimer();
    connect(tmr, SIGNAL(timeout()), this, SLOT(AddValuesToBuffer()));
    tmr->start(ValuesUpdateTimer);// этот таймер тоже за обновление значений (частота запихивания значений в буфер, оставить пока так должно быть сто

    UpdateGraficsTimer = new QTimer(this);
    connect(UpdateGraficsTimer, SIGNAL(timeout()), this, SLOT(UpdateGraphics()));
    UpdateGraficsTimer->start(GraphicsUpdateTimer); // этот таймер отвечает за обновление графика (частота отрисовки графика) должно быть 100-200 милисекунд

    //
    LabelsInit();

    // инициализация таблицы реле-слот
    InitRelaySlotTable();

    // получение значений уставок из файла
//    ReadUstavkiFromFile();

    //перехват событий на стрелках переключения групп
    ui->arrowGroupLeft->installEventFilter(this);
    ui->arrowGroupRight->installEventFilter(this);
    if(listGroup.size() == 0)
    {
        // добавление 1 группы (обязательной)
        cGroupChannels * group = new cGroupChannels();
        group->groupName = "Group 1";
#ifdef RANDOM_CHAN
        group->channel[0] = 0;
        group->typeInput[0] = 1;
        group->channel[1] = 1;
        group->typeInput[1] = 1;
        group->channel[2] = 2;
        group->typeInput[2] = 1;
        group->channel[3] = 3;
        group->typeInput[3] = 1;
#endif
        group->enabled = true;
        listGroup.append(group);
    }
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
    ui->frameSteelStatus->setMaximumWidth(16777215);
    ui->framePlotSteel->setMaximumWidth(16777215);
    ui->frameTemperature->setMaximumWidth(16777215);

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




//    Options op;
//    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)
//    op.deleteLater();

//    //Получение системных настроек из файла
//    updateSystemOptions();

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
    ui->wBar_5->changeNum(5);
    ui->wBar_6->changeNum(6);
    ui->wBar_7->changeNum(7);
    ui->wBar_8->changeNum(8);
    connect(ui->wBar_1, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_2, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_3, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_4, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_5, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_6, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_7, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->wBar_8, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    setStyleBars();
    setTextBars();

    //инициализация виджетов значений
    ui->widgetVol1->changeNum(1);
    ui->widgetVol2->changeNum(2);
    ui->widgetVol3->changeNum(3);
    ui->widgetVol4->changeNum(4);
    ui->widgetVol5->changeNum(5);
    ui->widgetVol6->changeNum(6);
    ui->widgetVol7->changeNum(7);
    ui->widgetVol8->changeNum(8);
    connect(ui->widgetVol1, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->widgetVol2, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->widgetVol3, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->widgetVol4, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->widgetVol5, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->widgetVol6, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->widgetVol7, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
    connect(ui->widgetVol8, SIGNAL(clickedLabel(int)), this, SLOT(openSettingsChannel(int)));
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

    dMenu * menu = new dMenu();
    dialogMenu = (QDialog*)menu;
    connect(menu, SIGNAL(saveButtonSignal()), this, SLOT(updateSystemOptions()));
    //сигнал из меню о создании новой уставки
    connect(menu, SIGNAL(newUstavka(int)), this, SLOT(newUstavkaConnect(int)));
    // сигналы для инициализации прошивки платы
    connect(menu, SIGNAL(signalToWorker(Transaction)), this, SLOT(retransToWorker(Transaction)));
    connect(menu, SIGNAL(signalRestartLocalModbus()), this, SLOT(slotRestartLocalModbus()));


    qDebug() << "MainWindow is init";
}


void MainWindow::updateVer()
{
    QString ver = CURRENT_VER;
    QString name = "MULTIGRAPH";
    if(systemOptions.typeMultigraph == cSystemOptions::Multigraph_Steel)
    {
        ver = ver + "S";
        name = name + "-STEEL";
    }
    ui->labelNameVersion->setText(QString(name + " " + ver));
    QString protocol = PROTOCOL_VER;
    ui->labelProtocol->setText(QString("PROTOCOL VERSION: " + protocol));
    QString revision = SOFTWARE_REVISION;
    ui->labelRevision->setText(QString("SOFTWARE REVISION: " + revision));
    ui->multigrapf->setText(name);
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
    mListUstvok.lock();
    for(int i = 0; i < listUstavok.size(); i ++)
    {
        Ustavka * ust = listUstavok.at(i);
        connect(ust, SIGNAL(workReleSignal(int, bool)), this, SLOT(sendRelayStateToWorker(int, bool)));
    }
    mListUstvok.unlock();

    // запуск обновления уставок по таймеру
    timeUpdUst = new QTimer();
    connect(timeUpdUst, SIGNAL(timeout()), this, SLOT(UpdUst()));
    timeUpdUst->start(UstavkiUpdateTimer);
}

void MainWindow::UpdUst()
{

    mListUstvok.lock();
    foreach (Ustavka * ust, listUstavok) {
        int ch = ust->getChannel();
        int i = 0;
        if(ch)
        {
            mListChannel.lock(); // Vag: Устранить мютекс в мютексе
            ChannelOptions * channel = listChannels.at(ch-1);
            double value = channel->GetCurrentChannelValue();
            mListChannel.unlock();
            ust->update(value);
        }
        i++;
    }
    mListUstvok.unlock();
}


void MainWindow::InitRelaySlotTable()
{
    mListRelay.lock();
    for(int i = 0; i < TOTAL_NUM_RELAY; i++)
    {
        cRelay * relay = new cRelay(i);
        connect(relay, SIGNAL(signalSwitch(uint8_t, uint8_t, bool)), this, SLOT(slotRelay(uint8_t, uint8_t, bool)));
        connect(relay, SIGNAL(signalGetState(uint8_t, uint8_t)), this, SLOT(slotGetRelay(uint8_t, uint8_t)));
        listRelais.append(relay);
    }
    mListRelay.unlock();
}



/*
 * слот передачи сигнала о состоянии реле в Worker
*/
void MainWindow::sendRelayStateToWorker(int relay, bool state)
{
    mListRelay.lock();
    if(relay < listRelais.size())
    {
        listRelais.at(relay)->setState(state);
    }
    mListRelay.unlock();
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
        connect(device, SIGNAL(updateParam(Transaction)), this, SLOT(retransDeviceToWorker(Transaction)), Qt::DirectConnection);
        listDevice.append(device);
    }
}

void MainWindow::InitChannels()
{
    for(int i = 0; i < TOTAL_NUM_CHAN; i++)
    {
        ChannelOptions * ch = new ChannelOptions();
        ch->SetCurrentChannelValue(0);
        ch->setNum(i+1);
//        ch->setSlot(0);     //
        ch->setSlotChannel(i%NUM_CHAN_IN_4AI);
        ch->initCalibration();
//        connect(ch, SIGNAL(updateSignal(int)), this, SLOT(updateChannelSlot(int)));
        connect(ch, SIGNAL(sendToWorker(Transaction)), this, SLOT(retransToWorker(Transaction)), Qt::DirectConnection);
//        ch->SetMeasurePeriod(1000);
        listChannels.append(ch);
    }


}

void MainWindow::InitFreq()
{
    for(int i = 0; i < TOTAL_NUM_FREQ; i++)
    {
        cFreqChannel * freq = new cFreqChannel();
        freq->SetCurrentChannelValue(0);
        freq->setNum(i+1);
        freq->setSlotChannel(i%NUM_CHAN_IN_6DI6RO);
        connect(freq, SIGNAL(sendToWorker(Transaction)), this, SLOT(retransToWorker(Transaction)));
        listFreq.append(freq);
    }
}

void MainWindow::InitTimers()
{

    archivetimer  = new QTimer();
    halfSecondTimer  = new QTimer();
    timerUpdateDevices = new QTimer();

    connect(halfSecondTimer, SIGNAL(timeout()), this, SLOT(HalfSecondGone()));
    connect(timerUpdateDevices, SIGNAL(timeout()), this, SLOT(updateDevicesComplect()));
    updateDevicesComplect();

    halfSecondTimer->start(500);
    archivetimer->start(6000); // каждые 6 минут записываем архив на флешку
    timerUpdateDevices->start(timeInitDevices);
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
    ui->date_label->setText(local.date().toString(datestrings.at(systemOptions.dateindex)));
    ui->time_label->setText(local.time().toString(timestrings.at(systemOptions.timeindex)));
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
    QList<uint8_t> list6DI6RO;
    int countStableDevice = 0;
    mListDev.lock();
    foreach (cDevice * device, listDevice) {

        if(device->getStable()) countStableDevice++;

        if(device->getOnline() || !device->getStable()){
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
            case Divece_6DI6RO:
                list6DI6RO.append(device->getSlot());
                list6DI6RO.append(device->getSlot());
                list6DI6RO.append(device->getSlot());
                list6DI6RO.append(device->getSlot());
                list6DI6RO.append(device->getSlot());
                list6DI6RO.append(device->getSlot());
                break;
            default:
                // Перенести запись в журнал в другое место кода
//                cLogger mk(pathtomessages, cLogger::DEVICE);
//                QString strSlot = QString::number(device->getSlot());
//                QString strtype = QString::number(device->deviceType);
//                mk.addMess("Slot " + strSlot + " | Device unidentified | type "\
//                           + strtype, cLogger::WARNING);
                break;
            }
        }
    }
    // все ли модули определились онлайн/оффлайн ?
    if(countStableDevice == listDevice.size())
    {
        allDeviceStable = true;
        timerUpdateDevices->setInterval(timeUpdateDevices);
    }
    else
    {
        timerUpdateDevices->setInterval(timeInitDevices);
    }
    mListDev.unlock();


    // обновление списка каналов
    int i = 0;
    mListChannel.lock();
    foreach (uint8_t slot, list4AI) {
        if(i < listChannels.size())
        {
            //обновление параметров каналов
            ChannelOptions * ch = listChannels.at(i);

            // оперативное обновление всех параметров,
            //если было отключение или переключение платы
            if((ch->getSlot() != slot) || (!ch->enable))
            {
                ch->setSlotChannel(i%NUM_CHAN_IN_4AI);
                ch->setSlot(slot);     //
                ch->enable = true;
                ch->updateParam(true);
            }

        }
        i++;
    }
    if(listChannels.size() > list4AI.size())
    {
        // плат меньше, тогда временно отключем каналы, но не удаляем
        for(int i = list4AI.size(); i < listChannels.size(); i++)
        {
            listChannels.at(i)->enable = false;
        }
    }
    mListChannel.unlock();



    // обновление списка реле
    i = 0;
    mListRelay.lock();
    foreach (uint8_t slot, list8RP) {
        if(i < listRelais.size())
        {
            //обновление параметров реле
            cRelay * r = listRelais.at(i);
            r->enable = true;
            r->mySlot = slot;     //
            r->myPhysicalNum = i%NUM_RELAY_IN_8RP;
        }
        i++;
    }
    if(listRelais.size() > list8RP.size())
    {
        // плат стало меньше, тогда удаляем объекты
        for(int i = list8RP.size(); i < listRelais.size(); i++)
        {
            listRelais.at(i)->enable = false;
        }
    }
    mListRelay.unlock();

    // обновление списка частотных каналов и твердотельных реле
    i = 0;
    int sizeList8RP = list8RP.size();
    foreach (uint8_t slot, list6DI6RO) {
        mListFreq.lock();
        if(i < listFreq.size())
        {
            //обновление параметров каналов
            cFreqChannel * ch = listFreq.at(i);
            ch->setSlot(slot);     //
            ch->setSlotChannel(i%NUM_CHAN_IN_6DI6RO);
            ch->enable = true;
        }
        mListFreq.unlock();
        mListRelay.lock();
        if((i + sizeList8RP) < listRelais.size())
        {
            //обновление параметров реле (твердортельных реле)
            cRelay * r = listRelais.at(i + sizeList8RP);
            r->enable = true;
            r->mySlot = slot;     //
            r->myPhysicalNum = i%NUM_RELAY_IN_6DI6RO;
        }
        mListRelay.unlock();
        i++;
    }
    mListFreq.lock();
    if(listFreq.size() > list6DI6RO.size())
    {
        // плат стало меньше, тогда временно отключем каналы, но не удаляем
        for(int i = list6DI6RO.size(); i < listFreq.size(); i++)
        {
            listFreq.at(i)->enable = false;
        }
    }
    mListFreq.unlock();


    mListRelay.lock();
    if(listRelais.size() > (list8RP.size() + list6DI6RO.size()))
    {
        // плат стало меньше, тогда отключаем объекты
        for(int i = (list8RP.size() + list6DI6RO.size()); i < listRelais.size(); i++)
        {
            listRelais.at(i)->enable = false;
        }
    }
    mListRelay.unlock();

    // обновление списка входных групп анализа стали
    i = 0;
    foreach (uint8_t slot, listSTEEL) {
        if(i < listSteel.size())
        {
            //обновление параметров реле
            cSteel * s = listSteel.at(i);
            s->slot = slot;     //
            s->slotIndex = i%NUM_CHAN_IN_STEEL;
            s->enable = true;
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
    mListUstvok.lock();
    Ustavka * ust = listUstavok.at(num);
    connect(ust, SIGNAL(workReleSignal(int, bool)), this, SLOT(sendRelayStateToWorker(int, bool)));
    mListUstvok.unlock();
}

void MainWindow::slotRelay(uint8_t sl, uint8_t num, bool state)
{

    Transaction tr(Transaction::W);
    int relayControlRegister = 0;
    for(int i = 0; i < NUM_RELAY_IN_8RP; i ++)
    {
        int index = getIndexRelayBySlotAndCh(sl, i);
        if(index != -1)
        {
            relayControlRegister += (listRelais.at(index)->getState() << i);
        }
    }
    tr.offset = cRegistersMap::getOffsetByName("RelayControl");
    tr.slave = sl;
    tr.volInt = relayControlRegister;
#ifdef DEBUG_RELAY
    qDebug() << "Relay:" << num << "=" << state;
#endif
    emit sendTransToWorker(tr);
}

void MainWindow::slotGetRelay(uint8_t sl, uint8_t num)
{

    Transaction tr(Transaction::R);
    int relayControlRregister = 0;
    for(int i = 0; i < NUM_RELAY_IN_8RP; i ++)
    {
        int index = getIndexRelayBySlotAndCh(sl, i);
        if(index != -1)
        {
            relayControlRregister += (listRelais.at(index)->getState() << i);
        }
    }
    tr.offset = cRegistersMap::getOffsetByName("RelayControl");
    tr.slave = sl;
    tr.volInt = relayControlRregister;
    tr.slave = sl;
//#ifdef DEBUG_RELAY
//    qDebug() << "Relay:" << num << "=" << state;
//#endif
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
    mListDev.lock();
    foreach (cDevice * dev, listDevice) {
        dev->pause(f);
    }
    mListDev.unlock();
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
    if((tr.slave <= listDevice.size()) && (tr.slave != 0))
    {
        mListDev.lock();
        deviceTypeEnum deviceType = listDevice.at(tr.slave - 1)->deviceType;
        mListDev.unlock();
        if(tr.offset >= 1000)
        {
            //параметры каналов фильтруются в зависимости от типа модуля и типа прибора
            if(systemOptions.typeMultigraph == cSystemOptions::Multigraph)
            {
                if((deviceType == Device_4AI) || (deviceType == Device_8RP)  || (deviceType == Divece_6DI6RO))
                    emit sendTransToWorker(tr);
            }
            else if((systemOptions.typeMultigraph == cSystemOptions::Multigraph_Steel))
            {
                if((deviceType == Device_STEEL) || (deviceType == Device_8RP))
                    emit sendTransToWorker(tr);
            }
        }
        else
        {
            // параметры модулей не фильруются
            emit sendTransToWorker(tr);
        }
    }
}

void MainWindow::retransDeviceToWorker(Transaction tr)
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
    deviceTypeEnum deviceType;
    ChannelOptions * channel = listChannels.at(0);
    int ch;

    mQTr->lock();
    int counter = 0;
    while(!queueTransaction.isEmpty() && (counter++ < 4))
    {
        tr = queueTransaction.dequeue();
//        int typeDevice = sc->getTypeDevice(tr.slave);
        if((tr.slave == 0) || (tr.slave > TOTAL_NUM_DEVICES)) return;
        int indexDev = tr.slave - 1;
        mListDev.lock();
        cDevice * device = listDevice.at(indexDev);
        deviceType = device->deviceType;
        device->setOnline();
        mListDev.unlock();
        paramName = cRegistersMap::getNameByOffset(tr.offset);

        if(paramName.contains("chan0"))
            // канал 1
        {
            ch = 0;
            if(deviceType == Device_4AI)
            {
                int index = getIndexAnalogBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListChannel.lock();
                    channel = listChannels.at(index);
                    channel->parserChannel(tr);
                    mListChannel.unlock();
                }
            }
            else if(deviceType == Device_STEEL)
            {
                int index = getIndexSteelBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    listSteel.at(index)->parserSteel(tr);
                }
            }
            else if(deviceType == Divece_6DI6RO)
            {
                int index = getIndexFreqBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListFreq.lock();
                    listFreq.at(index)->parserChannel(tr);
                    mListFreq.unlock();
                }
            }
        }
        else if(paramName.contains("chan1"))
            // канал 2
        {
            ch = 1;
            if(deviceType == Device_4AI)
            {
                int index = getIndexAnalogBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListChannel.lock();
                    channel = listChannels.at(index);
                    channel->parserChannel(tr);
                    mListChannel.unlock();
                }
            }
            else if(deviceType == Device_STEEL)
            {
                int index = getIndexSteelBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    listSteel.at(index)->parserSteel(tr);
                }
            }
            else if(deviceType == Divece_6DI6RO)
            {
                int index = getIndexFreqBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListFreq.lock();
                    listFreq.at(index)->parserChannel(tr);
                    mListFreq.unlock();
                }
            }

        }
        else if(paramName.contains("chan2"))
            // канал 3
        {
            ch = 2;
            if(deviceType == Device_4AI)
            {
                int index = getIndexAnalogBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListChannel.lock();
                    channel = listChannels.at(index);
                    channel->parserChannel(tr);
                    mListChannel.unlock();
                }
            }
            else if(deviceType == Divece_6DI6RO)
            {
                int index = getIndexFreqBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListFreq.lock();
                    listFreq.at(index)->parserChannel(tr);
                    mListFreq.unlock();
                }
            }
        }
        else if(paramName.contains("chan3"))
            // канал 4
        {
            ch = 3;
            if(deviceType == Device_4AI)
            {
                int index = getIndexAnalogBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListChannel.lock();
                    channel = listChannels.at(index);
                    channel->parserChannel(tr);
                    mListChannel.unlock();
                }
            }
            else if(deviceType == Divece_6DI6RO)
            {
                int index = getIndexFreqBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListFreq.lock();
                    listFreq.at(index)->parserChannel(tr);
                    mListFreq.unlock();
                }
            }
        }
        else if(paramName.contains("chan4"))
            // канал 5
        {
            ch = 4;
            if(deviceType == Divece_6DI6RO)
            {
                int index = getIndexFreqBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListFreq.lock();
                    listFreq.at(index)->parserChannel(tr);
                    mListFreq.unlock();
                }
            }
        }
        else if(paramName.contains("chan5"))
            // канал 6
        {
            ch = 5;
            if(deviceType == Divece_6DI6RO)
            {
                int index = getIndexFreqBySlotAndCh(tr.slave, ch);
                if(index != -1)
                {
                    mListFreq.lock();
                    listFreq.at(index)->parserChannel(tr);
                    mListFreq.unlock();
                }
            }
        }
        else
        {
            mListDev.lock();
            device->parseDeviceParam(tr);
            mListDev.unlock();

            //для реле регистр выходов как параметр платы
            if(paramName == "RelayControl")
            {
                if((deviceType == Device_8RP)\
                        || (deviceType == Divece_6DI6RO))
                {
                    for(int i = 0; i < NUM_RELAY_IN_8RP; i++)
                    {
                        int index = getIndexRelayBySlotAndCh(tr.slave, i);
                        if(index != -1)
                        {
                            bool state = (bool)((tr.volInt >> i) & 1);
                            mListRelay.lock();
                            listRelais.at(index)->setCurState(state);
                            mListRelay.unlock();
                        }
                    }

                }
            }
            // Vag: уместить всё внутри обработчика устройства cDevice (вверху)
            else if(paramName == "deviceType")
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

int MainWindow::getIndexAnalogBySlotAndCh(int slot, int ch)
{
    int ret = -1;
    mListChannel.lock();
    for(int i = 0; i < listChannels.size(); i++)
    {
        ChannelOptions * channel = listChannels.at(i);
        if((channel->getSlot() == slot) && (channel->getSlotChannel() == ch))
        {
            ret = i;
            break;
        }
    }
    mListChannel.unlock();
    return ret;
}

int MainWindow::getIndexFreqBySlotAndCh(int slot, int ch)
{
    int ret = -1;
    mListFreq.lock();
    for(int i = 0; i < listFreq.size(); i++)
    {
        cFreqChannel * freq = listFreq.at(i);
        if((freq->getSlot() == slot) && (freq->getSlotChannel() == ch))
        {
            ret = i;
            break;
        }
    }
    mListFreq.unlock();
    return ret;
}

int MainWindow::getIndexRelayBySlotAndCh(int slot, int ch)
{
    int ret = -1;
//    mListRelay.lock();
    for(int i = 0; i < listRelais.size(); i++)
    {
        cRelay * r = listRelais.at(i);
        if((r->mySlot == slot) && (r->myPhysicalNum == ch))
        {
            ret = i;
            break;
        }
    }
//    mListRelay.unlock();
    return ret;
}

bool MainWindow::isChannelInMaxNow(int ch)
{
    //индекс -> номер канала [1...]
    ch = ch + 1;

    mListUstvok.lock();
    foreach (Ustavka * u, listUstavok) {
        if(u->getChannel() == ch)
        {
            if(u->isUp())
            {
                mListUstvok.unlock();
                return true;
            }
        }
    }
    mListUstvok.unlock();
    return false;
}

bool MainWindow::isChannelInMinNow(int ch)
{
    //индекс -> номер канала [1...]
    ch = ch + 1;

    mListUstvok.lock();
    foreach (Ustavka * u, listUstavok) {
        if(u->getChannel() == ch)
        {
//            if(u->isDown()) return true;
        }
    }
    mListUstvok.unlock();
    return false;
}


void MainWindow::sendConfigChannelsToSlave()
{

    Transaction tr;
    tr.dir = Transaction::W;
    QString str;

    if(slotAnalogOnline)
    {

    }

    if(slotSteelOnline)
    {
        for(int i = 0; i < listSteel.size(); i++)
        {
            cSteel * st = listSteel.at(i);

            if(!st->enable) continue;

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
            str = "chan" + QString::number(devS) + "TimeMeasure";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->tPt;
            emit sendTransToWorker(tr);

            // запись Low_lim_temp
            str = "chan" + QString::number(devS) + "LowTemp";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->LPtl;
            emit sendTransToWorker(tr);

            // запись Hi_lim_temp
            str = "chan" + QString::number(devS) + "HiTemp";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->LPth;
            emit sendTransToWorker(tr);

            // запись Sensor_Type_Activty
            str = "chan" + QString::number(devS) + "SensorType";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->COH;
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
            tr.volFlo = st->technology->b1;
            emit sendTransToWorker(tr);

            // запись Mass_coefficient
            str = "chan" + QString::number(devS) + "MassCoeff";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->b2;
            emit sendTransToWorker(tr);

            // запись Final_oxidation
            str = "chan" + QString::number(devS) + "FinalOx";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->O;
            emit sendTransToWorker(tr);

            // запись Assimilation of aluminum
            str = "chan" + QString::number(devS) + "Assimilation";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->Y;
            emit sendTransToWorker(tr);

            // запись Mass_melting
            str = "chan" + QString::number(devS) + "MassMelting";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.volFlo = st->technology->G;
            emit sendTransToWorker(tr);

            // запись AdditionalParameter
            str = "chan" + QString::number(devS) + "AdditionalParameter1";
            tr.offset = cRegistersMap::getOffsetByName(str);
            tr.paramA12[0] = st->technology->nSt;
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
