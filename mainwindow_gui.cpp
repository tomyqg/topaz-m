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

QString inputstr = "";
QDateTime start(QDateTime::currentDateTime());
QString MainWindow::startdate = start.toString("dd/MM/yy");
QString MainWindow::starttime = start.toString("hh:mm:ss");
QString MainWindow::endtime = "";
QVector<QDateTime> MainWindow::Dates;

extern QColor Channel1Color;
extern QColor Channel2Color;
extern QColor Channel3Color;
extern QColor Channel4Color;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindowInitialization();
}

MainWindow::~MainWindow()
{
    MessageWrite mr1 ("Programm Closed");
    mr1.WriteAllLogToFile();
    delete ui;
}

QColor MainWindow::GetChannel1Color()
{
    return Channel1Color;
}

void MainWindow::SetChannel1Color(QColor newcolor)
{
     Channel1Color = newcolor;
}

void MainWindow::SetChannel2Color(QColor newcolor)
{
     Channel2Color = newcolor;
}

QColor MainWindow::GetChannel2Color()
{
    return Channel2Color;
}

void MainWindow::SetChannel3Color(QColor newcolor)
{
     Channel3Color = newcolor;
}

QColor MainWindow::GetChannel3Color()
{
    return Channel3Color;
}

void MainWindow::SetChannel4Color(QColor newcolor)
{
     Channel4Color = newcolor;
}

QColor MainWindow::GetChannel4Color()
{
    return Channel4Color;
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
UartDiriver *UD = new UartDriver();
    UD->SetRTSPinDirection();*/
}


void MainWindow::on_pushButton_clicked()
{
    PowerOff();
}

void MainWindow::on_pushButton_2_clicked()
{
    OpenOptionsWindow();
}

void MainWindow::on_pushButton_3_clicked()
{
    CloseApplication();
}

void MainWindow::on_pushButton_4_clicked()
{
    OpenMessagesWindow();
}

void MainWindow::on_radioButton_clicked()
{
    InitProcessorMaxFreq();
}

void MainWindow::on_radioButton_2_clicked()
{
    InitProcessorMinFreq();
}

void  MainWindow::updateText( const QString text ) // этот слот обновляет текстовое окно, когда мы эмитируем сигнал
{
    ui->textEdit_2->setText(text);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->MessagesWidget && event->type() == QEvent::Paint) {
        PaintOnWidget();
        return true; // return true if you do not want to have the child widget paint on its own afterwards, otherwise, return false.
    }
    return false;
}
