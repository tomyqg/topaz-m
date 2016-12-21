#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>
#include <QFile>
#include <QDataStream>
#include <QtMath>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include "math.h"
#include <QtScript/QScriptEngine>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QMutex>

QString inputstr = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);

    setWindowTitle(tr("ANGSTREM"));

    QPixmap pix("/usr/inc/logo.jpg");
    ui->label->setPixmap(pix);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));

    QTimer *timer3 = new QTimer(this);
    connect(timer3, SIGNAL(timeout()), this, SLOT(updategraph()));


    QTimer *timer4 = new QTimer(this);
    connect(timer4, SIGNAL(timeout()), this, SLOT(updatevalue()));

    timer->start(888);
    timer2->start(201);
    timer3->start(101);
    timer4->start(50);

    ui->customPlot->xAxis->setRange(-8, 600);
    ui->customPlot->yAxis->setRange(-5, 110);

    // a new thread that reads serial input

    QThread *thread= new QThread();
    NewThreadClass *my = new NewThreadClass();

    my->moveToThread(thread);

    connect(thread, SIGNAL(started()), my, SLOT(updatethread()));

    thread->start();
}

MainWindow::~MainWindow()
{
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
    ui->textEdit_2->setText(local.toString());
}

void MainWindow::updatevalue()
{
    ui->label_7->setText(inputstr);
    if (inputstr!="")
    {
        QString valuestring = inputstr.left(3);
        QString letterstring = inputstr.right(1);
        double value1 = valuestring.toDouble();
        if (value1<=1000)
            if (value1>0)
            {
                if (letterstring=="a")
                {
                    /*  ui->dial->setValue(value1);
                    ui->lcdNumber->display(value1);*/}
                if (letterstring=="b")
                {}
                //ui->lcdNumber_2->display(value1);
            }


        //        ui->label_7->setText(inputstr);

        //inputstr="";

    }
}


void MainWindow::paintEvent(QPaintEvent *e)
{
}

void MainWindow::on_pushButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_pushButton_2_pressed()
{
    ui->customPlot->clearGraphs();
    ui->label_4->setText("");
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    QFile filedir("/sys/class/gpio/gpio69/direction");

    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);

    outdir << "out";
    filedir.close();

    QFile file("/sys/class/gpio/gpio69/value");

    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    if (1)
    {
        out << "1";
    }

    else
    {
        out << "0";
    }
    file.close();
}

double MainWindow::returnmathresult(double dval)
{
    QString inn = ui->textEdit->toPlainText();

    QScriptEngine myEngine;

    QString vaal = QString::number(dval);

    QString replaced=inn;

    replaced.replace(QString("sin"), QString("Math.sin"));

    replaced.replace(QString("cos"), QString("Math.cos"));

    replaced.replace(QString("sqrt"), QString("Math.sqrt"));

    replaced.replace(QString("pow"), QString("Math.pow"));

    replaced.replace(QString("x"), QString(vaal));

    double Result = myEngine.evaluate(replaced).toNumber();

    return Result;
}

void MainWindow::on_textEdit_textChanged()
{
    QString inn = ui->textEdit->toPlainText();

    QScriptEngine myEngine;

    QString vaal = QString::number(ui->dial->value());

    QString replaced=inn;

    replaced.replace(QString("sin"), QString("Math.sin"));

    replaced.replace(QString("cos"), QString("Math.cos"));

    replaced.replace(QString("sqrt"), QString("Math.sqrt"));

    replaced.replace(QString("pow"), QString("Math.pow"));

    replaced.replace(QString("x"), QString(vaal));

    double Result = myEngine.evaluate(replaced).toNumber();
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
    ui->label_6->setText(QString::number(ui->horizontalSlider->value()));
}

void MainWindow::on_dial_valueChanged(int value)
{

}

void MainWindow::on_pushButton_3_clicked()
{
    QProcess process;
    process.startDetached("python /usr/pythscr.py"); // start script
}

void NewThreadClass::updatethread()
{
    QByteArray requestData;

    QSerialPort serial;
    QTime dieTime;
    serial.setPortName("/dev/ttyS1"); //usart1

    while (1)
    {

        if (serial.open(QIODevice::ReadWrite))
        {
            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::NoFlowControl);

            {
                //                QByteArray requestData;// = serial.readAll();
                //                while (serial.waitForReadyRead(200))
                //                    requestData += serial.readAll();
                //                inputstr = QTextCodec::codecForMib(106)->toUnicode(requestData);
                //                if(serial.bytesAvailable()>0)
                //                    ;
                //                inputstr="ololo";

                //                while (serial.waitForReadyRead(200))
                //                    requestData = serial.readAll();
            }
            //inputstr =(QString::number(serial.bytesAvailable()));
            serial.close();
        }

        inputstr =("321");

        dieTime= QTime::currentTime().addSecs(2);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

        inputstr =("999");

        //inputstr="ololo";
    }
}

void MainWindow::delay(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
