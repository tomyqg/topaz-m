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
    DateUpdate();
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
    }

    if (watched == ui->centralWidget && event->type() == QEvent::MouseButtonPress) {
        ReactOnTouch();
    }
    return true;
}

void MainWindow::ShowMessageBox (QString title,QString message)
{
    QMessageBox::information(this, title, message);
}
