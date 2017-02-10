#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "dialog.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>
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
#include <channel1.h>

QString inputstr = "";

QDateTime start(QDateTime::currentDateTime());

QString MainWindow::starttime = start.toString();

QString MainWindow::endtime = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("VISION"));

    QProcess process1;
    QPixmap pix("/usr/inc/logo.jpg");
    ui->label->setPixmap(pix);
    ui->customPlot->xAxis->setRange(-8, 600);
    ui->customPlot->yAxis->setRange(-5, 110);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));

    tmr = new QTimer();
    tmr->setInterval(500);

    QTimer *closetimer = new QTimer(this);

    //    closetimer->setInterval(1000*120);

    connect(tmr, SIGNAL(timeout()), this, SLOT(updategraph()));
    connect(tmr, SIGNAL(timeout()), this, SLOT(updatevalue()));
    connect(timer, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));

    //    connect(closetimer, SIGNAL(timeout()), this, SLOT(on_pushButton_3_clicked()));

    tmr->start(500);
    timer->start(1111);
    timer2->start(201);
    //    closetimer->start(1000*150);

    // a new thread that reads serial input

    QThread *thread= new QThread();

    NewThreadClass *my = new NewThreadClass();

    my->moveToThread(thread);

    connect(thread, SIGNAL(started()), my, SLOT(updatethread()));

    thread->start();
    //    ui->comboBox_13->installEventFilter( this );
    //    ui->dial->installEventFilter( this );
    //    ui->pushButton_2->installEventFilter( this );

    QProcess process;
    process.startDetached("sudo cpufreq-set -f 300MHz"); // max freq on
//    process.startDetached("sudo cpufreq-set --governor performance"); // max perfomance on
    process.startDetached("sudo cpufreq-set --governor powersave"); // max perfomance on


    process.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" 3383 3962 234 599"); // вручную ввели координаты тача
    //    process1.startDetached("xinput_calibrator"); // запускает калибратор дисплея
    //        process1.startDetached("xinput_calibrator"); // запускает калибратор дисплея
    //            process1.startDetached("sudo xinput_calibrator --list"); // вывели список таач-скринов
    //        process.startDetached("xinput set-prop 7 \"Device Accel Velocity Scaling\" 2"); // вручную ввели координаты тача
    //        process.startDetached("xinput list-props 7"); // вручную ввели координаты тача


//    QList<QLabel*> spinList = MainWindow::findChildren<QLabel*> (  );

//    // apply style to all widgets
//    for (int i = 0; i < spinList.count(); ++i) {
//        QLabel *sb = spinList.at(i);
//        sb->setText("alloha");
//    }
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
    ui->time_label->setText(local.time().toString() + local.date().toString(" dd.MM.yyyy "));
}

void MainWindow::textupdate()
{
    //    QWidget * fw = qApp->focusWidget();
    //    QObject * ob = qApp->focusObject();
    //    ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " + ob->objectName());
    //    QMouseEvent mouseEvent(QEvent::MouseButtonRelease,QCursor::pos(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    //    QCoreApplication::sendEvent(ui->pushButton_2,&mouseEvent);
}

void MainWindow::updatevalue()
{
    if (inputstr!="")
    {
        QString message =inputstr.left(4);
        QString valuestring = message.left(3);
        QString letterstring = message.right(1);
        double value1 = valuestring.toDouble();
        if (value1<=1000)
            if (value1>0)
            {
                if (letterstring=="a")
                {
                    ui->dial->setValue(value1);
                    ui->lcdNumber->display(value1);}
                if (letterstring=="b")

                    ui->lcdNumber_2->display(value1);
            }
    }
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
    //    WriteArchiveToFile();
    QApplication::exit();
}

void NewThreadClass::updatethread()
{
    while (1)
    {
        QSerialPort serial;
        serial.setPortName("/dev/ttyS1"); //usart1
        if (serial.open(QIODevice::ReadWrite))
        {
            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::NoFlowControl);
            {
                serial.write("a");
                while (serial.waitForBytesWritten(500))
                    ;

                serial.write("b");
                while (serial.waitForBytesWritten(500))
                    ;

                QByteArray requestData;
                while (serial.waitForReadyRead(10))
                    requestData = serial.readAll();
                inputstr = QTextCodec::codecForMib(106)->toUnicode(requestData);
                if(serial.bytesAvailable()>0)
                    ;

                if ((serial.bytesAvailable())>0)
                {
                    while (1)
                    {
                        while (serial.waitForReadyRead(10))
                            requestData = serial.readAll();
                        inputstr = QTextCodec::codecForMib(106)->toUnicode(requestData);
                        serial.write("c");
                        while (serial.waitForBytesWritten(20))
                            ;
                    }
                }
                while (serial.waitForReadyRead(10))
                    requestData = serial.readAll();
            }
            serial.close();
        }
    }
}

void MainWindow::delay(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    int nu = ui->horizontalSlider_2->value();
    ui->label_2->setText("Интервал " + QString::number(nu) +  " мсек");
    tmr->setInterval(500);
}

void MainWindow::on_comboBox_13_currentTextChanged(const QString &arg1)
{
    //    QMouseEvent *mEvnRelease = new QMouseEvent(QEvent::MouseButtonRelease, QCursor::pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    //    QCoreApplication::sendEvent(QWidget::focusWidget(),mEvnRelease);
    //    ui->label_7->setText("index changed");
}

void MainWindow::focusChanged(QWidget* , QWidget* )
{
    ui->textEdit_3->setText("b");
}

void MainWindow::on_comboBox_13_currentIndexChanged(int index)
{
}

void MainWindow::on_comboBox_13_activated(const QString &arg1)
{
}

void MainWindow::on_comboBox_13_currentIndexChanged(const QString &arg1)
{
}

void MainWindow::WritetoFile()
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
    ui->pushButton_2->setText("настрйки сохранены");
}

void MainWindow::on_pushButton_4_clicked()
{
    QProcess process1;
    //    process1.startDetached("xinput_calibrator"); // запускает калибратор дисплея
    //    process1.startDetached("sudo xinput_calibrator --list"); // вывели список таач-скринов
    //    process1.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" 3389 3958 252 616"); // вручную ввели координаты тача

    process1.startDetached("xinput_calibrator"); // запускает калибратор дисплея
}

void MainWindow::on_horizontalSlider_2_actionTriggered(int action)
{
}

void MainWindow::on_customPlot_destroyed()
{

}

void MainWindow::on_pushButton_5_clicked()
{
//    QList<QLabel*> spinList = MainWindow::findChildren<QLabel*> (  );

//    // apply style to all widgets
//    for (int i = 0; i < spinList.count(); ++i) {
//        QLabel *sb = spinList.at(i);
//        sb->setText("alloha");
//    }

    keyboard kb;
    kb.setModal(true);
    kb.exec();
}
