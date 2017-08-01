#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "channel1.h"
#include "uartdriver.h"
#include "worker.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPen>
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QtScript/QScriptEngine>
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

#define MYD
#define MultiThread // если убрать , то приложение будет однопоточное (пока многопоточное предпочтительнее по скорости и т.п.)

#define ValuesUpdateTimer 100
#define GraphicsUpdateTimer 500
#define ArchiveUpdateTimer 5000
#define DateLabelUpdateTimer 1000
#define XRange 300
#define YRange 180

void MainWindow::MainWindowInitialization()
{
    //    CreateMODBusConfigFile();

    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("VISION"));

    QPixmap pix("/usr/logo.jpg");
    ui->label->setPixmap(pix);

    // нужно установить евент филтер чтобы отрисовывалась графика
    ui->MessagesWidget->installEventFilter(this); // если закоментить то не будет уставок и цифр внизу

    SetXRange(XRange);
    SetYRange(YRange);

    ui->customPlot->yAxis->setRange(-GetXRange(), GetXRange());
    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);
    MessageWrite mr ;
    mr.LogAddMessage("Programm Started");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDateLabel()));

    UpdateGraficsTimer = new QTimer(this);

    connect(UpdateGraficsTimer, SIGNAL(timeout()), this, SLOT(UpdateGraphics()));

    QTimer *timetouch = new QTimer(this);
    connect(timetouch, SIGNAL(timeout()), this, SLOT(NewTouchscreenCalibration()));

    tmr = new QTimer();
    tmr->setInterval(ValuesUpdateTimer);

    QTimer *tmrarchive = new QTimer(this);
    tmrarchive->setInterval(ArchiveUpdateTimer);

    connect(tmrarchive, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));
    tmrarchive->start(ArchiveUpdateTimer);

    connect(tmr, SIGNAL(timeout()), this, SLOT(AddValuesToBuffer()));

    tmr->start(ValuesUpdateTimer);// этот таймер тоже за обновление значений (частота запихивания значений в буфер, оставить пока так должно быть сто
    UpdateGraficsTimer->start(GraphicsUpdateTimer); // этот таймер отвечает за обновление графика (частота отрисовки графика) должно быть 100-200 милисекунд

    timer->start(DateLabelUpdateTimer);
    timetouch->start(ArchiveUpdateTimer);

    InitPins();
    InitTouchScreen();
    InitProcessorMaxFreq();
    InitTimers();
    LabelsInit();

    channel1object.ReadSingleChannelOptionFromFile(1);
    channel2object.ReadSingleChannelOptionFromFile(2);
    channel3object.ReadSingleChannelOptionFromFile(3);
    channel4object.ReadSingleChannelOptionFromFile(4);

    //SetWindowHeightPixels(GetMonitorHeightPixels());
    //SetWindowWidthPixels(GetMonitorWidthPixels());

    SetWindowWidthPixels(1280);
    SetWindowHeightPixels(720);

    //SetWindowWidthPixels(1024);
    //SetWindowHeightPixels(768);

#ifdef MultiThread
    thread= new QThread();
    ModBus *MB;
    MB = new ModBus();
    MB->moveToThread(thread);
    connect(thread, SIGNAL(started()), MB, SLOT(ReadAllChannelsThread()) );
    //    connect( thread, SIGNAL(started()), MB, SLOT(ThreadReact(ChannelOptions&)) ); //
    //    connect(MB, SIGNAL(finished()), MB, SLOT(ThreadReact()));
    //    connect(MB, SIGNAL(finished()), thread, SLOT(quit()));
    //    connect(MB, SIGNAL(finished()), MB, SLOT(deleteLater()));

    //    connect(ui->pushButton_4, SIGNAL(clicked(bool)), thread, SLOT(quit()));
    thread->start();

    WorkerThread = new QThread;
    worker* myWorker = new worker;
    myWorker->moveToThread(WorkerThread);

    connect( this, SIGNAL(startWork()), myWorker, SLOT(StartWork()) );
    connect( this, SIGNAL(stopWork()), myWorker, SLOT(StopWork()) );

    WorkerThread ->start();

    startWork();


    Sleep(1000);
    stopWork();

#endif

    Options op;
    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)

    //    op.deleteLater();
    InitPins(); // почему-то нужно дважды вызывать эту функцию - нужно узнать - почему
    needConfirmation = 1;

    // connection for accessing to UI from another class
    objectwithsignal = new ChannelOptions;

    //    qRegisterMetaType<ChannelOptions&>("ChannelOptions");

    //    connect( this, SIGNAL(ThreadSignal( ChannelOptions* )), MB, SLOT(ThreadReact( ChannelOptions*)) ); //

    //    ThreadSignal(&channel1object);

    //    connect( options1, SIGNAL(destroyed(QObject*)), this, SLOT( updateText(const QString) ) ); //
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

void MainWindow::InitPins()
{

#ifdef MYD // если плата MYD то ничего нам с пинами инициализировать не нужно.
    return;
#endif
    // an object for make terminal requests
    QProcess process;

    //    importante!
    //    Use NOPASSWD line for all commands, I mean:
    //    debian ALL=(ALL) NOPASSWD: ALL
    //    Put the line after all other lines in the sudoers file.

    // allow to pin use
    process.startDetached("sudo chmod 777 /sys/class/gpio/gpio66/value");
    process.startDetached("sudo chmod 777 /sys/class/gpio/gpio66/direction");

    // config pins as uart
    process.startDetached("sudo config-pin P9.24 uart");
    process.startDetached("sudo config-pin P9.26 uart");

    //config RTS pin as gpio pull-down
    process.startDetached("sudo config-pin P8.7 gpio_pd");

    QFile filedir("/sys/class/gpio/gpio66/direction");
    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);
    outdir << "out";
    filedir.close();
}

void MainWindow::OpenMessagesWindow()
{

    mr.WriteAllLogToFile();

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


void MainWindow::OptionsWindowThread()
{

}

void MainWindow::OpenOptionsWindow()
{
    Options *optionsobj = new Options;
    this->resizeWindow(*optionsobj,this->GetWindowWidthPixels(),this->GetWindowHeightPixels());

    GetMonitorWidthPixels();
    optionsobj->exec();
    //читаем параметры каналов прямо после закрытия окна настроек и перехода в меню режима работы
    channel1object.ReadSingleChannelOptionFromFile(1);
    channel2object.ReadSingleChannelOptionFromFile(2);
    channel3object.ReadSingleChannelOptionFromFile(3);
    channel4object.ReadSingleChannelOptionFromFile(4);

    //    если вдруг поменялось время то нужно обновить лейблы
    LabelsInit();
    LabelsCorrect();

    // если что меняем разрешение
    if (Options::displayResolution == "1024x768")
    {
        resizeSelf(1024,768);
    }

    if (Options::displayResolution == "1280x800")
    {
        resizeSelf(1280,720);
    }

    optionsobj->deleteLater(); // удаляем объект опций
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

#ifdef MultiThread

    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateChannel1Slot()));
    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateChannel2Slot()));
    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateChannel3Slot()));
    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateChannel4Slot()));

#endif

    connect(halfSecondTimer, SIGNAL(timeout()), this, SLOT(HalfSecondGone()));
    channeltimer1->start(100);
    channeltimer2->start(100);
    channeltimer3->start(100);
    channeltimer4->start(100);
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

void MainWindow::DateUpdate()
{
    QDateTime local(QDateTime::currentDateTime());
    ui->time_label->setText(local.time().toString() + local.date().toString(" dd.MM.yyyy"));
    objectwithsignal->updateUI(local.time().toString());
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

void MainWindow::HalfSecondGone()
{
    InvertHalfSecFlag();
}

char MainWindow::GetHalfSecFlag()
{
    return halfSecondflag;
}

void MainWindow::InvertHalfSecFlag()
{
    halfSecondflag ++;
    halfSecondflag = halfSecondflag%2;
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
    ui->textEdit_2->setText(infosss);
}

void MainWindow::CheckState(ChannelOptions&  channel)
{
    channel.GetCurrentChannelValue();
    double channelcurrentvalue = channel.GetCurrentChannelValue();
    double channelstate1value = channel.GetState1Value();
    double channelstate2value = channel.GetState2Value();
    QString channelstringvalue = (QString::number( channelcurrentvalue, 'f', 3)) + " " + channel.GetUnitsName();

    //    превысили верхнюю уставку
    if ( (channelcurrentvalue>channelstate1value) && ( channel.HighState1Setted == false ) )
    {
        channel.LowState1Setted = false;
        channel.HighState1Setted = true;
        mr.LogAddMessage (channel.GetChannelName() + ":" + channel.GetState1HighMessage() + ":" + channelstringvalue);
    }

    // было превышение а стало норма
    else if ( (channelcurrentvalue>=channelstate2value) && (channelcurrentvalue<=channelstate1value) && ( channel.HighState1Setted == true ) )
    {
        channel.HighState1Setted = false;
        mr.LogAddMessage (channel.GetChannelName() + ":" + channel.GetState1LowMessage() + ":" + channelstringvalue);
    }

    // было уменьшение а стало норма
    else if ( (channelcurrentvalue>=channelstate2value) && (channelcurrentvalue<=channelstate1value) && ( channel.LowState1Setted == true ) )
    {
        channel.LowState1Setted = false;
        mr.LogAddMessage (channel.GetChannelName() + ":" + channel.GetState2HighMessage() + ":" + channelstringvalue);
    }

    //стало ниже нижней уставки
    else if ( (channelcurrentvalue<channelstate2value) && ( channel.LowState1Setted == false ) )
    {
        channel.LowState1Setted = true;
        channel.HighState1Setted = false;
        mr.LogAddMessage (channel.GetChannelName() + ":" + channel.GetState2LowMessage() + ":" + channelstringvalue);
    }
}

