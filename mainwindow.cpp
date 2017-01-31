#include "mainwindow.h"
#include "options.h"
#include "ui_mainwindow.h"
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
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QtTest/QTestEvent>
#include <QtTest/QTestEventList>
#include <QtTest/QTest>
#include <QPoint>
#include <QEvent>
#include <QMouseEvent>

#include <QFocusEvent>

#include <channel1.h>

QString inputstr = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("VISION"));
    QPixmap pix("/usr/inc/logo.jpg");
    ui->label->setPixmap(pix);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));

    tmr = new QTimer();
    tmr->setInterval(1000);
    connect(tmr, SIGNAL(timeout()), this, SLOT(updategraph()));
    connect(tmr, SIGNAL(timeout()), this, SLOT(updatevalue()));

    tmr->start(100);

    timer->start(888);
    timer2->start(201);

    ui->customPlot->xAxis->setRange(-8, 600);
    ui->customPlot->yAxis->setRange(-5, 110);

    // a new thread that reads serial input

    QThread *thread= new QThread();

    NewThreadClass *my = new NewThreadClass();

    my->moveToThread(thread);

    connect(thread, SIGNAL(started()), my, SLOT(updatethread()));

    thread->start();
    ui->comboBox_13->installEventFilter( this );
    ui->dial->installEventFilter( this );
    ui->pushButton_2->installEventFilter( this );
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
//    Channel1Options eeeeee;
//    eeeeee.SetUnitsName("loh");
//   qDebug() << eeeeee.GetUnitsName();
}

void MainWindow::textupdate()
{
    QWidget * fw = qApp->focusWidget();
    QObject * ob = qApp->focusObject();
    ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " + ob->objectName());
    QMouseEvent mouseEvent(QEvent::MouseButtonRelease,QCursor::pos(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QCoreApplication::sendEvent(ui->pushButton_2,&mouseEvent);
    //    QCoreApplication::postEvent(ui->textEdit_3,&mouseEvent);
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
//    Channel1Options a;

//    Channel1Options b;

////    int adasd = a.Ret();
////    qDebug() << adasd;
////    adasd = b.Ret();
////    qDebug() << adasd;

////    b.setprop();

////    adasd = a.Ret();
////    qDebug() << adasd;
////    adasd = b.Ret();
////    qDebug() << adasd;

//    qDebug() << a.GetUnitsName();

//    a.SetUnitsName("ohhh yeeees");

//    qDebug() << b.GetUnitsName();

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
    tmr->setInterval(nu);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->buttons() == Qt::RightButton)
        ui->label_7->setText("Only right button");
    if(event->buttons() == Qt::LeftButton)
        ui->label_7->setText("Only left button");
    QMouseEvent *mEvnRelease = new QMouseEvent(QEvent::MouseButtonRelease, QCursor::pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(QWidget::focusWidget(),mEvnRelease);
}

void MainWindow::touchReleaseEvent(QTouchEvent *event)
{
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    ui->label_7->setText("button Released");
    //    this->focusNextChild();
    //    QCursor::setPos(200,200);
}

void MainWindow::on_comboBox_13_currentTextChanged(const QString &arg1)
{
    QMouseEvent *mEvnRelease = new QMouseEvent(QEvent::MouseButtonRelease, QCursor::pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(QWidget::focusWidget(),mEvnRelease);
    ui->label_7->setText("index changed");
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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    switch( event->type() ){
    case QEvent::MouseButtonDblClick:
        //        qDebug() << "Mouse Button Double Clicked";
        ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " + "Mouse Double Clicked");
        break;
    case QEvent::MouseButtonPress:
        //        qDebug() << "Mouse Button Pressed";
        ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " +"Mouse Button Pressed aaaaa");
        focusNextChild();
        break;
    case QEvent::MouseButtonRelease:
        //        qDebug() << "Mouse Button Released";
        ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " +"Mouse Button Released");
        break;

    case QEvent::TouchBegin:
        //        qDebug() << "TouchBegin";
        ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " +"TouchBegin");
        break;

    case QEvent::TouchEnd:
        //        qDebug() << "TouchEnd";
        ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " +"TouchEnd");
        break;

    case QEvent::TouchCancel:
        //        qDebug() << "TouchCancel";
        ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " +"TouchCancel");
        break;

    case QEvent::TouchUpdate:
        //qDebug() << "TouchUpdate";
        ui->textEdit_3->setText(ui->textEdit_3->toPlainText() + " | " +"TouchUpdate");
        break;

    default:
        break;
    }
    // pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
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
