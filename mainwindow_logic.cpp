#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "channel1.h"
#include "uartdriver.h"

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

void MainWindow::MainWindowInitialization()
{
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("VISION"));

    QPixmap pix("/usr/logo.jpg");
    ui->label->setPixmap(pix);
    ui->MessagesWidget->installEventFilter(this);
    ui->centralWidget->installEventFilter(this);

    ui->customPlot->yAxis->setRange(-200, 200);
    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);
    MessageWrite mr ;
    mr.LogAddMessage("Programm Started");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDateLabel()));

    UpdateGraficsTimer = new QTimer(this);
    //connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));
    connect(UpdateGraficsTimer, SIGNAL(timeout()), this, SLOT(UpdateGraphics()));

    QTimer *timetouch = new QTimer(this);
    connect(timetouch, SIGNAL(timeout()), this, SLOT(NewTouchscreenCalibration()));

    tmr = new QTimer();
    tmr->setInterval(500);

    QTimer *tmrarchive = new QTimer(this);
    tmrarchive->setInterval(5000);

    connect(tmrarchive, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));
    tmrarchive->start(5000);

    connect(tmr, SIGNAL(timeout()), this, SLOT(AddValuesToBuffer()));

    tmr->start(100);// этот таймер тоже за обновление значений (частота запихивания значений в буфер, оставить пока так
    timer->start(1111);
    UpdateGraficsTimer->start(200); // этот таймер отвечает за обновление графика (частота отрисовки графика)
    timetouch->start(5000);


    InitPins();
    InitTouchScreen();
    InitProcessorMaxFreq();
    InitTimers();
    LabelsInit();

    thread= new QThread();
    ModBus *MB = new ModBus();
    MB->moveToThread(thread);
    connect(thread, SIGNAL(started()), MB, SLOT(ReadAllChannelsThread()));
    //connect(thread, SIGNAL(finished()), MB, SLOT(deleteLater()) );

    channel1object.ReadSingleChannelOptionFromFile(1);
    channel2object.ReadSingleChannelOptionFromFile(2);
    channel3object.ReadSingleChannelOptionFromFile(3);
    channel4object.ReadSingleChannelOptionFromFile(4);

    Options op;
    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)

    InitPins(); // почему-то нужно дважды вызывать эту функцию - нужно узнать - почему

    // connection for accessing to UI from another class
    ChannelOptions * objectwithsignal = new ChannelOptions;
    connect( objectwithsignal, SIGNAL(updateUI(const QString)), this, SLOT( updateText(const QString) ) ); //
    objectwithsignal->updateUI("NEW UI TEXT");
}

void MainWindow::LabelsInit()
{
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();

    int half = GetTotalLabelsCount()/2;
    int timeperiodseconds = GetTimePeriodSecs();

    for (int var = 0; var <= GetTotalLabelsCount(); ++var) {
        Dates.append(fisttime.addSecs((var-half)*timeperiodseconds));// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels.append(fisttime.addSecs((var-half)*timeperiodseconds).toString("hh:mm:ss") );
    }

    //    for (int var = 0; var <= GetTotalLabelsCount(); ++var) {
    //        Labels.append(QString::number(var));
    //    }

}

/*void MainWindow::InitLabels()
{
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();

    int half = GetTotalLabelsCount()/2;
    int timeperiodseconds = GetTimePeriodSecs();

    for (int var = 0; var <= GetTotalLabelsCount(); ++var) {
        Dates.append(fisttime.addSecs((var-half)*timeperiodseconds));// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels.append(fisttime.addSecs((var-half)*timeperiodseconds).toString("hh:mm:ss") );
    }
}*/

void MainWindow::InitPins()
{
    // an object for make terminal requests
    QProcess process;

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
    Messages messages;
    messages.setModal(true);
    messages.exec();
}

void MainWindow::DelaySec(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::OpenOptionsWindow()
{
    Options options;
    options.setModal(true);
    options.exec();
    //читаем параметры каналов прямо после закрытия окна настроек и перехода в меню режима работы
    channel1object.ReadSingleChannelOptionFromFile(1);
    channel2object.ReadSingleChannelOptionFromFile(2);
    channel3object.ReadSingleChannelOptionFromFile(3);
    channel4object.ReadSingleChannelOptionFromFile(4);
}

void MainWindow::PowerOff()
{
    QProcess process;
    process.startDetached("sudo poweroff");
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

    //connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateDataChannel111()));
    //connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateDataChannel222()));
    //connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateDataChannel333()));
    //connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateDataChannel444()));

    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateDataChannel1()));
    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateDataChannel2()));
    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateDataChannel3()));
    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateDataChannel4()));

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
    halfSecondflag++;
    halfSecondflag = halfSecondflag%2;
}

void MainWindow::NewTouchscreenCalibration()
{
    QProcess process;
    process.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" " + Options::calibrationprm); // каждую секунду вводим координаты тача вдруг чтобы не отвалился
    
    // следующий код почему-то не работает
    /*
    QProcess process1;

    //    process1.start("xinput set-prop 7 \"Evdev Axis Calibration\" " + Options::calibrationprm); // max perfomance on
    //    QString reqst = "xinput set-prop 7 \"Evdev Axis Calibration\" " + Options::calibrationprm;

    //    QString reqst = "xinput set-prop 7 \"Evdev Axis Calibration\" 3383 3962 234 599 ";

    //    QString reqst = "sudo xinput_calibrator";

    QString reqst = "ls";

    process1.start(reqst); // max perfomance on

    QString stringtofind = "unable to find device";
    process1.waitForFinished();

    QString output;

    while (process1.bytesAvailable()>0)
    {
        output = QTextCodec::codecForMib(106)->toUnicode(process1.readAll());
    }

    QString pice1 = output;

    if (output.indexOf(stringtofind)>=0)
    {
        QString pice = output;

        //ui->textEdit->setText(Options::calibrationprm);
        //QString a = Options::calibrationprm;
        //QString pice = output.remove(0,(output.indexOf(stringtofind ) ) );
        //pice = pice.remove(pice.indexOf(stringtofind2), pice.length() - pice.indexOf(stringtofind2) );
        //pice = pice.simplified();
        //pice = pice.remove(0, stringtofind.length() );
        //Options::calibrationprm = pice.remove('\"');
    }*/
}
