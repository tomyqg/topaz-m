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

void MainWindow::InitLabels()
{
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();

    for (int var = 0; var <= 10; ++var) {

        Dates.append(fisttime.addSecs((var-5)*6));// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels.append(fisttime.addSecs((var-5)*6).toString("hh:mm:ss") );
    }
}

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

void MainWindow::InitProcessor()
{
    // an object for make terminal requests
    QProcess process;

    //    Use NOPASSWD line for all commands, I mean:
    //    jenkins ALL=(ALL) NOPASSWD: ALL
    //    Put the line after all other lines in the sudoers file.

    // the maximum processor speed
    process.startDetached("sudo cpufreq-set -f 1000MHz");
}

void MainWindow::InitTimers()
{
    channeltimer1 = new QTimer();
    channeltimer2 = new QTimer();
    channeltimer3 = new QTimer();
    channeltimer4 = new QTimer();
    archivetimer = new QTimer();

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
}

void MainWindow::InitTouchScreen()
{
    // an object for make terminal requests
    QProcess process;

    // the initial touchscreen
    process.startDetached("sudo xinput set-prop 7 \"Evdev Axis Calibration\" 3383 3962 234 599"); // вручную ввели координаты тача
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

    int i = 0;
    while(i<=10)
    {
        Dates[i] = Dates[i].addMSecs(timecorrectsec );// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels[i] = Dates.at(i).toString("hh:mm:ss"); //Dates.at(i).toString("hh:mm:ss");
        i++;
    }
}
