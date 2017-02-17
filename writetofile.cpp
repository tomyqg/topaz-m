#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
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

void Options::WriteSystemOptionsToFile()
{
    QJsonObject systemoptions;

    QDateTime local(QDateTime::currentDateTime());

    systemoptions["Time"] = local.time().toString();
    systemoptions["Date"] = local.date().toString();
    systemoptions["Calibration"] = Options::calibrationprm;

    QString setstr = QJsonDocument(systemoptions).toJson(QJsonDocument::Compact);

    QFile file("/usr/systemoptions.txt");

    file.open(QIODevice::ReadWrite);

    file.resize(0); // clear file

    QTextStream out(&file);
    out << setstr;
    file.close();
}



void Options::WriteOptionsToFile()
{
    QJsonObject channel1;
    QJsonObject channel2;
    QJsonObject channel3;
    QJsonObject channel4;
    QJsonObject channels;
    QJsonArray settings;

    channel1["Type"] = options1.GetSignalType();
    channel1["Units"] = options1.GetUnitsName();
    channel1["HigherLimit"] = options1.GetHigherLimit();
    channel1["LowerLimit"] = options1.GetLowerLimit();
    channel1["HigherMeasLimit"] = options1.GetHigherMeasureLimit();
    channel1["LowerMeasLimit"] = options1.GetLowerMeasureLimit();

    settings.append(channel1);

    channel2["Type"] = options2.GetSignalType();
    channel2["Units"] = options2.GetUnitsName();
    channel2["HigherLimit"] = options2.GetHigherLimit();
    channel2["LowerLimit"] = options2.GetLowerLimit();
    channel2["HigherMeasLimit"] = options2.GetHigherMeasureLimit();
    channel2["LowerMeasLimit"] = options2.GetLowerMeasureLimit();

    settings.append(channel2);

    channel3["Type"] = options3.GetSignalType();
    channel3["Units"] = options3.GetUnitsName();
    channel3["HigherLimit"] = options3.GetHigherLimit();
    channel3["LowerLimit"] = options3.GetLowerLimit();
    channel3["HigherMeasLimit"] = options3.GetHigherMeasureLimit();
    channel3["LowerMeasLimit"] = options3.GetLowerMeasureLimit();

    settings.append(channel3);

    channel4["Type"] = options4.GetSignalType();
    channel4["Units"] = options4.GetUnitsName();
    channel4["HigherLimit"] = options4.GetHigherLimit();
    channel4["LowerLimit"] = options4.GetLowerLimit();
    channel4["HigherMeasLimit"] = options4.GetHigherMeasureLimit();
    channel4["LowerMeasLimit"] = options4.GetLowerMeasureLimit();

    settings.append(channel4);

    channels["count"] = 4;
    channels["channels"] = settings;

    QString setstr = QJsonDocument(channels).toJson(QJsonDocument::Compact);
    QFile file("/usr/options.txt");
    //        QFile file("C:/Work/options.txt");
    file.open(QIODevice::ReadWrite);

    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
}


void MessageWrite::LogMessageWrite(QString nm)
{
    QJsonObject themessage;
    QJsonObject archive;

    //    for(int y=0; y<12; y++)
    //    {
    //        archivechannel1["Date"] = y*2.12;
    //        archivechannel1["Message"] = nm;

    //        valuesarray.append(archivechannel1);
    //        messagequeue.append(archivechannel2);
    //    }
    //    int a;

    QDateTime local (QDateTime::currentDateTime());
    themessage["Time"] = local.time().toString();
    themessage["Date"] = local.date().toString("dd/MM/yy");
    themessage["Message"] = nm;

    for(int y=0; y<100; y++)
    {
        themessage["Time"] = local.time().toString();
        themessage["Date"] = local.date().toString("dd/MM/yy");
        themessage["Message"] = nm;
        messagesqueue.append(themessage);
    }

    messagesqueue.append(themessage);



    Options opt;
    double maxmes  = opt.getmaxmessageslimit();


    while (messagesqueue.count()>maxmes)
    {
        messagesqueue.removeFirst();
    }

    archive["messagesqueue"] = messagesqueue;
    archive["totalmsg"] = messagesqueue.count();

    QString setstr = QJsonDocument(archive).toJson(QJsonDocument::Compact);

    //    QFile file("C:/Work/Log.txt");
    QFile file("/usr/Log.txt");

    file.open(QIODevice::ReadWrite);

    file.resize(0); // clear file

    QTextStream out(&file);

    out << setstr;

    file.close();
}

void MessageWrite::LogClear()
{

    QFile file("/usr/Log.txt");

    file.open(QIODevice::ReadWrite);

    file.resize(0); // clear file

    file.close();

}
