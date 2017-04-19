#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "dialog.h"
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

QString inputstr = "";
QDateTime start(QDateTime::currentDateTime());
QString MainWindow::startdate = start.toString("dd/MM/yy");
QString MainWindow::starttime = start.toString("hh:mm:ss");
QString MainWindow::endtime = "";

QVector<QDateTime> MainWindow::Dates;

void MainWindow::LabelsInit()
{
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();

    for (int var = 0; var <= 10; ++var) {

        Dates.append(fisttime.addSecs((var-5)*6));// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels.append(fisttime.addSecs((var-5)*6).toString("hh:mm:ss") );
    }
}

void MainWindow::LabelsUpdate()
{
    int i = 0;
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();
    
    while(i<=10) {
        Dates[i] = Dates[i].addSecs(6);// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels[i] = Dates.at(i).toString("hh:mm:ss"); //Dates.at(i).toString("hh:mm:ss");
        i++;
    }
}

void MainWindow::LabelsCorrect()
{
    QDateTime fisttime;
    qint16 timecorrectsec;
    fisttime = QDateTime::currentDateTime();
    timecorrectsec = Dates[5].msecsTo(fisttime);
    //    qDebug() << timecorrectsec;

    int i = 0;
    while(i<=10)
    {
        Dates[i] = Dates[i].addMSecs(timecorrectsec );// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels[i] = Dates.at(i).toString("hh:mm:ss"); //Dates.at(i).toString("hh:mm:ss");
        i++;
    }
    //    qDebug() << "Labels Correct";
}

MainWindow::MainWindow(QWidget *parent) :    QMainWindow(parent),  ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("VISION"));
    Initialization();
}

MainWindow::~MainWindow()
{
    MessageWrite mr1 ("Programm Closed");
    mr1.WriteAllLogToFile();
    delete ui;
}

void MainWindow::on_lcdNumber_overflow()
{
}

void MainWindow::updateDateLabel()
{
    QDateTime local(QDateTime::currentDateTime());
    ui->time_label->setText(local.time().toString() + local.date().toString(" dd.MM.yyyy"));
}

void MainWindow::touchupdate()
{
    QProcess process;
    process.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" " + Options::calibrationprm); // каждую секунду вводим координаты тача вдруг чтобы не отвалился
}

void MainWindow::on_pushButton_clicked()
{
    QProcess process;
    process.startDetached("sudo poweroff");
    QApplication::exit();
}

void MainWindow::on_pushButton_2_clicked()
{
    Options options;
    options.setModal(true);
    options.exec();
    //читаем параметры каналов прямо после закрытия окна настроек и перехода в меню режима работы
    ch1.readoptionsfromfile(1);
    ch2.readoptionsfromfile(2);
    ch3.readoptionsfromfile(3);
    ch4.readoptionsfromfile(4);
    //qDebug() << "readoptionsfromfile" ;
}

void MainWindow::on_dial_valueChanged(int value)
{
}

void MainWindow::on_pushButton_3_clicked()
{
    QApplication::exit();
}

void MainWindow::delay(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::on_pushButton_4_clicked()
{
    mr.WriteAllLogToFile();
    Messages messages;
    messages.setModal(true);
    messages.exec();
}

void MainWindow::on_radioButton_clicked()
{
    QProcess process;
    process.startDetached("sudo cpufreq-set -f 1000MHz"); // max freq on
    process.startDetached("sudo cpufreq-set --governor performance"); // max perfomance on
}

void MainWindow::on_radioButton_2_clicked()
{
    QProcess process;
    process.startDetached("sudo cpufreq-set -f 300MHz"); // max freq on
    process.startDetached("sudo cpufreq-set --governor powersave"); // min perfomance on
}

void MainWindow::Initialization()
{
    QProcess process1;
    QPixmap pix("/usr/logo.jpg");
    ui->label->setPixmap(pix);
    ui->MessagesWidget->installEventFilter(this);
    ui->customPlot->xAxis->setRange(-8, 600);
    ui->customPlot->yAxis->setRange(-200, 200);
    
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDateLabel()));
    
    QTimer *timer2 = new QTimer(this);
    //    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(UpdateGraphics()));
    
    QTimer *timetouch = new QTimer(this);
    connect(timetouch, SIGNAL(timeout()), this, SLOT(touchupdate()));
    
    tmr = new QTimer();
    tmr->setInterval(500);
    
    QTimer *tmrarchive = new QTimer(this);
    tmrarchive->setInterval(5000);
    
    connect(tmrarchive, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));
    tmrarchive->start(5000);
    
    QTimer *closetimer = new QTimer(this);
    
    connect(tmr, SIGNAL(timeout()), this, SLOT(AddValuesToBuffer()));
    //connect(tmr, SIGNAL(timeout()), this, SLOT(updatevalue()));
    
    tmr->start(100);// этот таймер тоже за обновление значений
    timer->start(1111);
    timer2->start(100); // этот таймер отвечает за обновление графика
    timetouch->start(5000);
    
    thread= new QThread();
    UartDriver *UD = new UartDriver();
    ModBus *MB = new ModBus();
    
    UD->SetRTSPinDirection();
    //UD->moveToThread(thread);
    MB->moveToThread(thread);
    connect(thread, SIGNAL(started()), MB, SLOT(ReadAllChannelsThread()));
    //connect(thread, SIGNAL(finished()), MB, SLOT(deleteLater()) );
    channeltimer1 = new QTimer();
    channeltimer1->setInterval(100);
    channeltimer2 = new QTimer();
    channeltimer2->setInterval(300);
    channeltimer3 = new QTimer();
    channeltimer3->setInterval(2000);
    channeltimer4 = new QTimer();
    channeltimer4->setInterval(5000);
    archivetimer = new QTimer();
    archivetimer->setInterval(6000);
    
    //connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateDataChannel111()));
    //connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateDataChannel222()));
    //connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateDataChannel333()));
    //connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateDataChannel444()));

    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateDataChannel1()));
    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateDataChannel2()));
    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateDataChannel3()));
    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateDataChannel4()));
    
    channeltimer1->start(100);
    channeltimer2->start(100);
    channeltimer3->start(100);
    channeltimer4->start(100);
    archivetimer->start(6000); // каждые 6 минут записываем архив на флешку
    
    QProcess process;
    process.startDetached("sudo cpufreq-set -f 1000MHz"); // max freq on
    process.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" 3383 3962 234 599"); // вручную ввели координаты тача
    process.startDetached("config-pin P9.24 uart");
    process.startDetached("config-pin P9.26 uart");
    process.startDetached("config-pin P8.7 gpio_pd");
    
    MessageWrite mr ;
    mr.LogMessageWrite("Programm Started");

    ch1.readoptionsfromfile(1);
    ch2.readoptionsfromfile(2);
    ch3.readoptionsfromfile(3);
    ch4.readoptionsfromfile(4);

    LabelsInit();

    Options::DisplayParametr = Options::Polar;

    ChannelOptions * asddddd = new ChannelOptions;
    connect( asddddd, SIGNAL(updateUI(const QString)), this, SLOT( updateText(const QString) ) ); //
    asddddd->updateUI("NEW UI TEXT");
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->MessagesWidget && event->type() == QEvent::Paint) {
        PaintOnWidget();
        return true; // return true if you do not want to have the child widget paint on its own afterwards, otherwise, return false.
    }
    return false;
}

void  MainWindow::updateText( const QString text ) // const QString & newText
{
    ui->textEdit_2->setText(text);
}
