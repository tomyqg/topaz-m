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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

void MainWindow::on_dial_sliderMoved(int position)
{
    ui->lcdNumber->display(ui->dial->value());
}

void MainWindow::on_lcdNumber_overflow()
{
}

void MainWindow::on_dial_actionTriggered(int action)
{
}

void MainWindow::updateCaption()
{
    QDateTime local(QDateTime::currentDateTime());
    ui->time_label->setText(local.time().toString() + local.date().toString(" dd.MM.yyyy "));
    mathresolver mr;
    //ui->textEdit_2->setText(QString::number( mr.Solve("sqrt(abs(x)) / sqrt(abs(x-5))", 9) ) ); //"sin(2) + cos(2)"+ cos(2)
}

void MainWindow::touchupdate()
{
    QProcess process;
    process.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" " + Options::calibrationprm); // каждую секунду вводим координаты тача вдруг чтобы не отвалился
}

void MainWindow::paintEvent(QPaintEvent *e)
{
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

void MainWindow::on_pushButton_2_pressed()
{
    ui->customPlot->clearGraphs();
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
    ui->customPlot->xAxis->setRange(-8, 600);
    ui->customPlot->yAxis->setRange(-200, 200);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));

    QTimer *timetouch = new QTimer(this);
    connect(timetouch, SIGNAL(timeout()), this, SLOT(touchupdate()));

    tmr = new QTimer();
    tmr->setInterval(500);

    QTimer *tmrarchive = new QTimer(this);
    tmrarchive->setInterval(5000);

    connect(tmrarchive, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));
    tmrarchive->start(5000);

    QTimer *closetimer = new QTimer(this);

    connect(tmr, SIGNAL(timeout()), this, SLOT(updategraph()));
    //connect(tmr, SIGNAL(timeout()), this, SLOT(updatevalue()));

    tmr->start(20);// этот таймер тоже отвечает за обновление графика
    timer->start(1111);
    timer2->start(20); // этот таймер отвечает за обновление графика
    timetouch->start(5000);

    QThread *thread= new QThread();
    UartDriver *UD = new UartDriver();
    ModBus *MB = new ModBus();

    UD->SetRTSPinDirection();

//    UD->moveToThread(thread);

    MB->moveToThread(thread);

    connect(thread, SIGNAL(started()), MB, SLOT(ReadAllChannelsThread()));

    channeltimer1 = new QTimer();
    channeltimer1->setInterval(100);
    channeltimer2 = new QTimer();
    channeltimer2->setInterval(300);
    channeltimer3 = new QTimer();
    channeltimer3->setInterval(2000);
    channeltimer4 = new QTimer();
    channeltimer4->setInterval(5000);

    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateDataChannel111()));
    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateDataChannel222()));
    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateDataChannel333()));
    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateDataChannel444()));
//    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateDataChannel1()));
//    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateDataChannel2()));
//    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateDataChannel3()));
//    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateDataChannel4()));

    channeltimer1->start(100);
    channeltimer2->start(100);
    channeltimer3->start(100);
    channeltimer4->start(100);

    thread->start();

    QProcess process;

    process.startDetached("sudo cpufreq-set -f 1000MHz"); // max freq on
    //process.startDetached("sudo cpufreq-set --governor powersave"); // min perfomance on
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
}
