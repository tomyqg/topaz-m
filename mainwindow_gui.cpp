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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("VISION"));
    MainWindowInitialization();
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
/*
    QProcess process;


//    QProcess process1;
//    process1.start("sudo su"); // root permission
//    process1.waitForFinished();
//    QString output = QTextCodec::codecForMib(106)->toUnicode(process1.readAll());
//    process1.start("temppwd"); // root permission
//    process1.waitForFinished();

    process.startDetached("sudo chmod 777 /sys/class/gpio/gpio66/value");
    process.startDetached("sudo chmod 777 /sys/class/gpio/gpio66/direction");
    process.startDetached("sudo cpufreq-set -f 1000MHz");
    process.startDetached("sudo xinput set-prop 7 \"Evdev Axis Calibration\" 3383 3962 234 599"); // вручную ввели координаты тача
    process.startDetached("sudo config-pin P9.24 uart");
    process.startDetached("sudo config-pin P9.26 uart");
    process.startDetached("sudo config-pin P8.7 gpio_pd");
UartDriver *UD = new UartDriver();
    UD->SetRTSPinDirection();*/
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
    ch1.ReadSingleChannelOptionFromFile(1);
    ch2.ReadSingleChannelOptionFromFile(2);
    ch3.ReadSingleChannelOptionFromFile(3);
    ch4.ReadSingleChannelOptionFromFile(4);
}

void MainWindow::on_pushButton_3_clicked()
{
    QApplication::exit();
}

void MainWindow::DelaySec(int n)
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
