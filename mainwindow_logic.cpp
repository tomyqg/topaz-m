#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
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
#include <filemanager.h>

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

extern MainWindow * globalMainWin;
//extern tDeviceBasicParams g_deviceDataStorage;

void MainWindow::MainWindowInitialization()
{
    //qRegisterMetaType<Transaction>("Transaction");

    ui->unvisible_block->setHidden(true);

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


    scene = new QGraphicsScene();   // Init graphic scene

    // находим все com - порты
//    int portIndex = 0;
//    int i = 0;
//    QSettings s;
//    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
//    {
//        //        qDebug() << port.portName;
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
        but->installEventFilter(this);
    }

    listCh.append(&channel1);
    listCh.append(&channel2);
    listCh.append(&channel3);
    listCh.append(&channel4);


    //перед рисованием графиков записать нули в первый элемент вектора
    channel1.SetCurrentChannelValue(0);
    channel2.SetCurrentChannelValue(0);
    channel3.SetCurrentChannelValue(0);
    channel4.SetCurrentChannelValue(0);

    ui->customPlot->yAxis->setRange(-GetXRange(), GetXRange());
    ui->customPlot->setAntialiasedElements(QCP::aeNone);

    //инициализация журнала событий
    logger = new cLogger(pathtomessages);
    logger->addMess("Programm Started");
    //    messwrite.LogAddMessage("Programm Started");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDateLabel()));


    displayrefreshtimer = new QTimer();
//    displayrefreshtimer->setInterval(3000);
    displayrefreshtimer->start(30000);
    connect( displayrefreshtimer, SIGNAL(timeout()), this, SLOT(RefreshScreen()) );

    QTimer *tmrarchive = new QTimer(this);
    connect(tmrarchive, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));
    tmrarchive->start(ArchiveUpdateTimer);

    tmr = new QTimer();
    connect(tmr, SIGNAL(timeout()), this, SLOT(AddValuesToBuffer()));
    tmr->start(ValuesUpdateTimer);// этот таймер тоже за обновление значений (частота запихивания значений в буфер, оставить пока так должно быть сто

    UpdateGraficsTimer = new QTimer(this);
    connect(UpdateGraficsTimer, SIGNAL(timeout()), this, SLOT(UpdateGraphics()));
    UpdateGraficsTimer->start(GraphicsUpdateTimer); // этот таймер отвечает за обновление графика (частота отрисовки графика) должно быть 100-200 милисекунд

    timer->start(DateLabelUpdateTimer);

//    updLogTimer = new QTimer(this);
//    connect(updLogTimer, SIGNAL(timeout()), this, SLOT(UpdateLog()));
//    updLogTimer->start(LogUpdTimer);

    InitTimers();
    LabelsInit();

    // инициализация таблицы канал-слот
    InitChannelSlotTable();

    // инициализация таблицы реле-слот
    InitRelaySlotTable();

    // инициализация объектов уставок
    InitUstavka();
    // получение значений уставок из файла
//    ReadUstavkiFromFile();

//    channel1.ReadSingleChannelOptionFromFile(1);
//    channel2.ReadSingleChannelOptionFromFile(2);
//    channel3.ReadSingleChannelOptionFromFile(3);
//    channel4.ReadSingleChannelOptionFromFile(4);

    int i = cFileManager::readChannelsSettings(pathtooptions, listCh, ustavkaObjectsList);

    channel1.SetNormalColor(ColorCh1);
    channel2.SetNormalColor(ColorCh2);
    channel3.SetNormalColor(ColorCh3);
    channel4.SetNormalColor(ColorCh4);

    channel1.SetMaximumColor(QColor(ColorCh1.red() - 20, ColorCh1.green() - 20, ColorCh1.blue() - 20));
    channel2.SetMaximumColor(QColor(ColorCh2.red() - 20, ColorCh2.green() - 20, ColorCh2.blue() - 20));
    channel3.SetMaximumColor(QColor(ColorCh3.red() - 20, ColorCh3.green() - 20, ColorCh3.blue() - 20));
    channel4.SetMaximumColor(QColor(ColorCh4.red() - 20, ColorCh4.green() - 20, ColorCh4.blue() - 20));

    channel1.SetMinimumColor(QColor(ColorCh1.red() + 20, ColorCh1.green() + 20, ColorCh1.blue() + 20));
    channel2.SetMinimumColor(QColor(ColorCh2.red() + 20, ColorCh2.green() + 20, ColorCh2.blue() + 20));
    channel3.SetMinimumColor(QColor(ColorCh3.red() + 20, ColorCh3.green() + 20, ColorCh3.blue() + 20));
    channel4.SetMinimumColor(QColor(ColorCh4.red() + 20, ColorCh4.green() + 20, ColorCh4.blue() + 20));

    SetWindowWidthPixels(1024);
    SetWindowHeightPixels(768);

    // создание конфигуратора слотов
    sc = new cSlotsConfig(this);
    connect(this, SIGNAL(retransToSlotConfig(Transaction)), sc, SLOT(receiveConf(Transaction)));

    //
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
    connect(ui->EcoCheckBox, SIGNAL(clicked(bool)), this, SLOT(ChangePalette(bool)) );
    connect(this, SIGNAL(sendTransToWorker(Transaction)), myWorker, SLOT(getTransSlot(Transaction)), Qt::DirectConnection);
    connect(myWorker, SIGNAL(sendTrans(Transaction)), this, SLOT(getTransFromWorkerSlot(Transaction)), Qt::DirectConnection);
    connect(myWorker, SIGNAL(sendMessToLog(QString)), this, SLOT(WorkerMessSlot(QString)), Qt::DirectConnection);
    connect(sc, SIGNAL(sendRequest(Transaction)), myWorker, SLOT(getTransSlot(Transaction)), Qt::DirectConnection);
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

    QListIterator<ChannelOptions*> li(listCh);
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

    //инициализация виджетов значений
    ui->widgetVol1->changeNum(1);
    ui->widgetVol2->changeNum(2);
    ui->widgetVol3->changeNum(3);
    ui->widgetVol4->changeNum(4);

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
    for(int i = 0; i < TOTAL_NUM_USTAVKI; i ++)
    {
        Ustavka *ust = new Ustavka(this);
        connect(ust, SIGNAL(workReleSignal(int, bool)), this, SLOT(sendRelayStateToWorker(int, bool)));
        connect(ust, SIGNAL(messToLogSignal(int,QString)), this, SLOT(logginStates(int,QString)));
        ustavkaObjectsList.append(ust);
    }

    // запуск обновления уставок по таймеру
    timeUpdUst = new QTimer();
    connect(timeUpdUst, SIGNAL(timeout()), this, SLOT(UpdUst()));
    timeUpdUst->start(UstavkiUpdateTimer);

    //Vag:тест
    Ustavka * u = ustavkaObjectsList.at(1);
    u->setKvitirUp(true);
    u->setKvitirDown(true);
    //----
}

void MainWindow::UpdUst()
{
    QList<ChannelOptions *> ChannelsObjectsList;
    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);

    foreach (Ustavka * ust, ustavkaObjectsList) {
        int ch = ust->getChannel();
        if(ch)
        {
            ChannelOptions * channel = ChannelsObjectsList.at(ch-1);
            ust->setNameCh(channel->GetChannelName());
            ust->update(channel->GetCurrentChannelValue());

        }
    }
}

//-----Временная реализация соединения слотов----

#ifdef Q_OS_WIN32
#define CONST_SLAVE_ADC     7
#define CONST_SLAVE_RELAY   7
#else
#define CONST_SLAVE_ADC     5
#define CONST_SLAVE_RELAY   6
#endif

void MainWindow::InitChannelSlotTable()
{
    csc.addChannelSlot(0, 0, CONST_SLAVE_ADC);
    csc.addChannelSlot(1, 1, CONST_SLAVE_ADC);
    csc.addChannelSlot(2, 2, CONST_SLAVE_ADC);
    csc.addChannelSlot(3, 3, CONST_SLAVE_ADC);
}

void MainWindow::InitRelaySlotTable()
{
    //временный механизм создания связей
    rsc.addRelaySlot(0, 0, CONST_SLAVE_RELAY);
    rsc.addRelaySlot(1, 1, CONST_SLAVE_RELAY);
    rsc.addRelaySlot(2, 2, CONST_SLAVE_RELAY);
    rsc.addRelaySlot(3, 3, CONST_SLAVE_RELAY);
    rsc.addRelaySlot(4, 7, CONST_SLAVE_RELAY);
    rsc.addRelaySlot(5, 6, CONST_SLAVE_RELAY);
    rsc.addRelaySlot(6, 5, CONST_SLAVE_RELAY);
    rsc.addRelaySlot(7, 4, CONST_SLAVE_RELAY);
}
//-----/Временная реализация соединения слотов----


/*
 * слот передачи сигнала о состоянии реле в Worker
*/
void MainWindow::sendRelayStateToWorker(int relay, bool state)
{

    // номер реле -> индекс реле
    relay -= 1;

    // получить номер слота платы реле
    int slot = rsc.getSlotByRelay(relay);

    // получить индекс реле на плате (канал)
    int devRelay = rsc.getDevRelay(relay);

    // определение адреса параметра в соответствии с интексом реле
    // это пока временная реализация --------------
    uint32_t relayOffset;
    if(devRelay%2)
    {
        relayOffset = ChannelOptions::chanReleyHi;
    } else {
        relayOffset = ChannelOptions::chanReleyLow;
    }
    //---------------------------------------------

    uint16_t offset = getDevOffsetByChannel(devRelay>>1, relayOffset);

    Transaction tr(Transaction::W, slot, offset, 0);
    // значение 1 в регистре замыкает реле
    // тут нужно вставить инверсию, если выход нормально замкнут
    if(state) tr.volInt = 1;
    else tr.volInt = 0;
#ifdef DEBUG_RELAY
    qDebug() << "Relay:" << relay << "(DevRalay:" << devRelay << ")" << "=" << state;
#endif
    emit sendTransToWorker(tr);
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
    Messages *messages = new Messages;
    connect( messages, SIGNAL(destroyed(QObject*)), this, SLOT( destroyedslot(QObject*)) ); //
    this->resizeWindow(*messages,this->GetWindowWidthPixels(),this->GetWindowHeightPixels());
    messages->setModal(true);
    messages->exec();
    messages->deleteLater();
}


void MainWindow::DelaySec(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(n);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::OpenOptionsWindow( int index )
{
    // Vag: сохранить все настройки девайсов и каналов в файл перед открытием окна настроек
    WriteAllChannelsOptionsToFile();

    //здесь запускаем меню обновленное как в эндресе
    StackedOptions * sw= new StackedOptions(index, 0);

    sw->exec();

    //читаем системные настройки из файла и применяем тут же
    Options op;
    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)
    op.deleteLater();

    // получение значений уставок из файла
    ReadUstavkiFromFile();

    //читаем параметры каналов прямо после закрытия окна настроек и перехода в меню режима работы
    channel1.ReadSingleChannelOptionFromFile(1);
    channel2.ReadSingleChannelOptionFromFile(2);
    channel3.ReadSingleChannelOptionFromFile(3);
    channel4.ReadSingleChannelOptionFromFile(4);
    // после чтения параметров сразу запихиваем их в сигнал для воркера (передаем воркеру значения каждого канала )
    sendConfigChannelsToSlave();

    //если вдруг поменялось время то нужно обновить лейблы
    LabelsInit();
    LabelsCorrect();
    sw->deleteLater();
    resizeSelf(1024,768);

//    SetPolarAngle(0);

    return;
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

void MainWindow::InitTimers()
{
    channeltimer1 = new QTimer();
    channeltimer2 = new QTimer();
    channeltimer3 = new QTimer();
    channeltimer4 = new QTimer();
    archivetimer  = new QTimer();
    halfSecondTimer  = new QTimer();

    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateChannel1Slot()));
    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateChannel2Slot()));
    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateChannel3Slot()));
    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateChannel4Slot()));

    connect(halfSecondTimer, SIGNAL(timeout()), this, SLOT(HalfSecondGone()));
    channeltimer1->start(1000);
    channeltimer2->start(1000);
    channeltimer3->start(1000);
    channeltimer4->start(1000);
    halfSecondTimer->start(500);
    archivetimer->start(6000); // каждые 6 минут записываем архив на флешку
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

void MainWindow::NewTouchscreenCalibration()
{
    QProcess process;
    process.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" " + Options::calibrationprm); // каждую секунду вводим координаты тача вдруг чтобы не отвалился
}

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
    ChannelOptions * ch;
    switch(channel)
    {
    case 1:
        ch = &channel1;
        break;
    case 2:
        ch = &channel2;
        break;
    case 3:
        ch = &channel3;
        break;
    case 4:
        ch = &channel4;
        break;
    default:
        break;
    }

    double cur = ch->GetCurrentChannelValue();
    QString channelstringvalue = (QString::number( cur, 'f', 3)) + ch->GetUnitsName();
    //messwrite.LogAddMessage (ch->GetChannelName() + ":" + mess + ":" + channelstringvalue);
    logger->addMess(ch->GetChannelName() + ":" + mess + ":" + channelstringvalue);

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

    if (ui->EcoCheckBox->checkState())
    {
        Channel1Color = Channel1ColorNormal = ColorCh1Light;
        Channel2Color = Channel2ColorNormal = ColorCh2Light;
        Channel3Color = Channel3ColorNormal = ColorCh3Light;
        Channel4Color = Channel4ColorNormal = ColorCh4Light;
    }
    else
    {
        Channel1Color = Channel1ColorNormal = ColorCh1;
        Channel2Color = Channel2ColorNormal = ColorCh2;
        Channel3Color = Channel3ColorNormal = ColorCh3;
        Channel4Color = Channel4ColorNormal = ColorCh4;
    }

    Channel1ColorMaximum = QColor(Channel1Color.red() - 20,Channel1Color.green() - 20,Channel1Color.blue() - 20);
    Channel2ColorMaximum = QColor(Channel2Color.red() - 20,Channel2Color.green() - 20,Channel2Color.blue() - 20);
    Channel3ColorMaximum = QColor(Channel3Color.red() - 20,Channel3Color.green() - 20,Channel3Color.blue() - 20);
    Channel4ColorMaximum = QColor(Channel4Color.red() - 20,Channel4Color.green() - 20,Channel4Color.blue() - 20);

    Channel1ColorMinimum = QColor(Channel1Color.red() + 20,Channel1Color.green() + 20,Channel1Color.blue() + 20);
    Channel2ColorMinimum = QColor(Channel2Color.red() + 20,Channel2Color.green() + 20,Channel2Color.blue() + 20);
    Channel3ColorMinimum = QColor(Channel3Color.red() + 20,Channel3Color.green() + 20,Channel3Color.blue() + 20);
    Channel4ColorMinimum = QColor(Channel4Color.red() + 20,Channel4Color.green() + 20,Channel4Color.blue() + 20);

    channel1.SetNormalColor(Channel1ColorNormal);
    channel2.SetNormalColor(Channel2ColorNormal);
    channel3.SetNormalColor(Channel3ColorNormal);
    channel4.SetNormalColor(Channel4ColorNormal);

    channel1.SetMaximumColor(Channel1ColorMaximum);
    channel2.SetMaximumColor(Channel2ColorMaximum);
    channel3.SetMaximumColor(Channel3ColorMaximum);
    channel4.SetMaximumColor(Channel4ColorMaximum);

    channel1.SetMinimumColor(Channel1ColorMinimum);
    channel2.SetMinimumColor(Channel2ColorMinimum);
    channel3.SetMinimumColor(Channel3ColorMinimum);
    channel4.SetMinimumColor(Channel4ColorMinimum);

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
    ChannelOptions * channel;
    int ch;
    bool isDeviceParam = false;

    mQTr->lock();
    int counter = 0;
    while(!queueTransaction.isEmpty() && (counter++ < 4))
    {
        tr = queueTransaction.dequeue();
        paramName = cRegistersMap::getNameByOffset(tr.offset);
        isDeviceParam = false;
        if(tr.offset < BASE_OFFSET_DEVICE)
        {
            //пока ничего не делать
        }
        else if((tr.offset >= BASE_OFFSET_DEVICE) && (tr.offset < BASE_OFFSET_CHANNEL_1))
        {
            isDeviceParam = true;
        }
        else if((tr.offset >= BASE_OFFSET_CHANNEL_1) && (tr.offset < BASE_OFFSET_CHANNEL_2))
            // канал 1
        {
            channel = &channel1;
            ch = 0;
        }
        else if((tr.offset >= BASE_OFFSET_CHANNEL_2) && (tr.offset < BASE_OFFSET_CHANNEL_3))
            // канал 2
        {
            channel = &channel2;
            ch = 1;
        }
        else if((tr.offset >= BASE_OFFSET_CHANNEL_3) && (tr.offset < BASE_OFFSET_CHANNEL_4))
            // канал 3
        {
            channel = &channel3;
            ch = 2;
        }
        else if((tr.offset >= BASE_OFFSET_CHANNEL_4) && (tr.offset < (BASE_OFFSET_CHANNEL_4 + 128)))
            // канал 4
        {
            channel = &channel4;
            ch = 3;
        }

        if(!isDeviceParam)
        {
//            emit retransToSlotConfig(tr);
            if(paramName == QString("chan" + QString::number(ch) + "SignalType"))
            {
                if(tr.slave == 1)   //контроллировать источник нужно во всех "else if"
                {
                    channel->SetSignalType(tr.volInt);
                    channel->SetCurSignalType(tr.volInt);
                    emit retransToSlotConfig(tr);
                }
            }
            else if(paramName == QString("chan" + QString::number(ch) + "AdditionalParameter1"))
            {
                if(tr.slave == 1)
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
            else if(paramName == "DataChan0")
            {
                channel1.SetCurrentChannelValue((double)tr.volFlo);
                ui->wBar_1->setVolue((double)tr.volFlo);
                ui->widgetVol1->setVol((double)tr.volFlo);
            }
            else if(paramName == "DataChan1")
            {
                channel2.SetCurrentChannelValue((double)tr.volFlo);
                ui->wBar_2->setVolue((double)tr.volFlo);
                ui->widgetVol2->setVol((double)tr.volFlo);
            }
            else if(paramName == "DataChan2")
            {
                channel3.SetCurrentChannelValue((double)tr.volFlo);
                ui->wBar_3->setVolue((double)tr.volFlo);
                ui->widgetVol3->setVol((double)tr.volFlo);
            }
            else if(paramName == "DataChan3")
            {
                channel4.SetCurrentChannelValue((double)tr.volFlo);
                ui->wBar_4->setVolue((double)tr.volFlo);
                ui->widgetVol4->setVol((double)tr.volFlo);
            }
            else if(paramName == QString("chan" + QString::number(ch) + "Data"))
            {
//                 Vag: времено или совсем не использовать этот параметр для построения графика
//                channel->SetCurrentChannelValue((double)tr.volFlo);
            }
            else
            {
                emit retransToSlotConfig(tr);
            }
        }
        else
        {
            emit retransToSlotConfig(tr);
        }

    }
    mQTr->unlock();
    timerQueueTrans->start(ParsingReceiveTrans);
}

bool MainWindow::isChannelInMaxNow(int ch)
{
    //индекс -> номер канала [1...]
    ch = ch + 1;

    foreach (Ustavka * u, ustavkaObjectsList) {
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

    foreach (Ustavka * u, ustavkaObjectsList) {
        if(u->getChannel() == ch)
        {
            if(u->isDown()) return true;
        }
    }
    return false;
}


void MainWindow::sendConfigChannelsToSlave()
{
    QList<ChannelOptions *> ChannelsObjectsList;
    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);

    Transaction tr;
    tr.dir = Transaction::W;
    QString str;
    int devCh;

    for(int i = 0; i < ChannelsObjectsList.size(); i++)
    {
        devCh = csc.getDevChannel(i);
        tr.slave = csc.getSlotByChannel(devCh);

        // запись актуального значения SignalType
        str = "chan" + QString::number(devCh) + "SignalType";
        tr.offset = cRegistersMap::getOffsetByName(str);
        tr.volInt = ChannelsObjectsList.at(i)->GetSignalType();
        emit sendTransToWorker(tr);

        // запись актуального Additional parameter1
        str = "chan" + QString::number(devCh) + "AdditionalParameter1";
        tr.offset = cRegistersMap::getOffsetByName(str);
//        tr.volInt = 0;
        int size = sizeof(tr.paramA12);
        memset(tr.paramA12, 0, size);
        if(ChannelsObjectsList.at(i)->GetSignalType() == ModBus::VoltageMeasure)
        {
            tr.paramA12[0] = (uint16_t)ChannelsObjectsList.at(i)->GetDiapason();
        }
        else if(ChannelsObjectsList.at(i)->GetSignalType() == ModBus::TermoResistanceMeasure)
        {
            tr.paramA12[0] = (uint16_t)ChannelsObjectsList.at(i)->getShema();
            tr.paramA12[1] = (uint16_t)ChannelsObjectsList.at(i)->GetDiapason();
        }
        else if((ChannelsObjectsList.at(i)->GetSignalType() == ModBus::TermoCoupleMeasure))
        {
            tr.paramA12[0] = 1;
            tr.paramA12[1] = (uint16_t)ChannelsObjectsList.at(i)->GetDiapason();
            tr.paramA12[2] = 2;
        }
        emit sendTransToWorker(tr);
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

