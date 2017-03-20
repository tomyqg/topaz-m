#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "ui_options.h"
#include "messages.h"
#include "keyboard.h"
#include "dialog.h"
#include "channel1.h"

extern QVector<double> xx1;
extern QVector<double> yy1;
extern QVector<double> yy2;
extern QVector<double> yy3;
extern QVector<double> yy4;

extern QString pathtofile ;

void MainWindow::WriteGpio(int num, bool val)
{
    QFile filedir("/sys/class/gpio/gpio" + QString::number(num) + "/direction");

    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);

    outdir << "out";
    filedir.close();

    QFile file("/sys/class/gpio/gpio69/value");

    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    out << val;

    file.close();
}

void Options::WriteSystemOptionsToFile()
{
    QJsonObject systemoptions;

    QDateTime local(QDateTime::currentDateTime());

    //    systemoptions["Time"] = local.time().toString();
    //    systemoptions["Date"] = local.date().toString();

    systemoptions["Time"] = ui->timeEdit->time().toString();
    systemoptions["Date"] = ui->dateEdit->date().toString();

    systemoptions["Calibration"] = Options::calibrationprm;

    QString setstr = QJsonDocument(systemoptions).toJson(QJsonDocument::Compact);

    QFile file(pathtofile + "systemoptions.txt");

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
    channel1["Name"] = options1.GetChannelName();
    channel1["Units"] = options1.GetUnitsName();
    channel1["HigherLimit"] = options1.GetHigherLimit();
    channel1["LowerLimit"] = options1.GetLowerLimit();
    channel1["HigherMeasLimit"] = options1.GetHigherMeasureLimit();
    channel1["LowerMeasLimit"] = options1.GetLowerMeasureLimit();
    channel1["Period"] = options1.GetMeasurePeriod();
    channel1["State1HighMessage"] = options1.GetState1HighMessage();
    channel1["State1LowMessage"] = options1.GetState1LowMessage();
    channel1["State2HighMessage"] = options1.GetState2HighMessage();
    channel1["State2LowMessage"] = options1.GetState2LowMessage();
    channel1["State1Value"] = options1.GetState1Value();
    channel1["State2Value"] = options1.GetState2Value();


    settings.append(channel1);

    channel2["Type"] = options2.GetSignalType();
    channel2["Name"] = options2.GetChannelName();
    channel2["Units"] = options2.GetUnitsName();
    channel2["HigherLimit"] = options2.GetHigherLimit();
    channel2["LowerLimit"] = options2.GetLowerLimit();
    channel2["HigherMeasLimit"] = options2.GetHigherMeasureLimit();
    channel2["LowerMeasLimit"] = options2.GetLowerMeasureLimit();
    channel2["Period"] = options2.GetMeasurePeriod();
    channel2["State1HighMessage"] = options2.GetState1HighMessage();
    channel2["State1LowMessage"] = options2.GetState1LowMessage();
    channel2["State2HighMessage"] = options2.GetState2HighMessage();
    channel2["State2LowMessage"] = options2.GetState2LowMessage();
    channel2["State1Value"] = options2.GetState1Value();
    channel2["State2Value"] = options2.GetState2Value();

    settings.append(channel2);

    channel3["Type"] = options3.GetSignalType();
    channel3["Name"] = options3.GetChannelName();
    channel3["Units"] = options3.GetUnitsName();
    channel3["HigherLimit"] = options3.GetHigherLimit();
    channel3["LowerLimit"] = options3.GetLowerLimit();
    channel3["HigherMeasLimit"] = options3.GetHigherMeasureLimit();
    channel3["LowerMeasLimit"] = options3.GetLowerMeasureLimit();
    channel3["Period"] = options3.GetMeasurePeriod();
    channel3["State1HighMessage"] = options3.GetState1HighMessage();
    channel3["State1LowMessage"] = options3.GetState1LowMessage();
    channel3["State2HighMessage"] = options3.GetState2HighMessage();
    channel3["State2LowMessage"] = options3.GetState2LowMessage();
    channel3["State1Value"] = options3.GetState1Value();
    channel3["State2Value"] = options3.GetState2Value();

    settings.append(channel3);

    channel4["Type"] = options4.GetSignalType();
    channel4["Name"] = options4.GetChannelName();
    channel4["Units"] = options4.GetUnitsName();
    channel4["HigherLimit"] = options4.GetHigherLimit();
    channel4["LowerLimit"] = options4.GetLowerLimit();
    channel4["HigherMeasLimit"] = options4.GetHigherMeasureLimit();
    channel4["LowerMeasLimit"] = options4.GetLowerMeasureLimit();
    channel4["Period"] = options4.GetMeasurePeriod();
    channel4["State1HighMessage"] = options4.GetState1HighMessage();
    channel4["State1LowMessage"] = options4.GetState1LowMessage();
    channel4["State2HighMessage"] = options4.GetState2HighMessage();
    channel4["State2LowMessage"] = options4.GetState2LowMessage();
    channel4["State1Value"] = options4.GetState1Value();
    channel4["State2Value"] = options4.GetState2Value();

    settings.append(channel4);

    channels["count"] = 4;
    channels["channels"] = settings;

    QString setstr = QJsonDocument(channels).toJson(QJsonDocument::Compact);
    QFile file(pathtofile + "options.txt");
    QTextStream out(&file);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    out << setstr;
    file.close();
}

void MessageWrite::WriteAllLogToFile()
{
    QJsonObject archive;
    Options opt;
    double maxmes  = opt.getmaxmessageslimit();

    while (messagesqueue.count()>maxmes) // удаляем все значения что были раньше чем нужно
    {
        messagesqueue.removeFirst();
    }
    archive["messagesqueue"] = messagesqueue;
    archive["totalmsg"] = messagesqueue.count();
    QString setstr = QJsonDocument(archive).toJson(QJsonDocument::Compact);
    QFile file(pathtofile + "Log.txt");
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
}

void MessageWrite::LogMessageWrite(QString nm)
{
    QJsonObject themessage;
    QDateTime local (QDateTime::currentDateTime());
    themessage["Time"] = local.time().toString();
    themessage["Date"] = local.date().toString("dd/MM/yy");
    themessage["Message"] = nm;
    messagesqueue.append(themessage);
}

void MessageWrite::LogClear()
{
    QFile file(pathtofile + "Log.txt");
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    file.close();
}

void MainWindow::WriteArchiveToFile()
{
    QJsonObject archivechannel1;
    QJsonObject archivechannel2;
    QJsonObject archivechannel3;
    QJsonObject archivechannel4;
    QJsonObject archive;
    QJsonArray archives;
    QJsonArray valuesarray1;
    QJsonArray valuesarray2;
    QJsonArray valuesarray3;
    QJsonArray valuesarray4;

    for(int y=0; y<yy1.size(); y++)
        valuesarray1.append(yy1.at(y)*1.11);

    archivechannel1["size"] = valuesarray1.size();
    archivechannel1["values"] = valuesarray1;
    archivechannel1["name"] = "channel_1";

    for(int y=0; y<yy2.size(); y++)
        valuesarray2.append(yy2.at(y)*1.22);

    archivechannel2["size"] = valuesarray2.size();
    archivechannel2["values"] = valuesarray2;
    archivechannel2["name"] = "channel_2";

    for(int y=0; y<yy3.size(); y++)
        valuesarray3.append(yy3.at(y)*1.33);

    archivechannel3["size"] = valuesarray3.size();
    archivechannel3["values"] = valuesarray3;
    archivechannel3["name"] = "channel_3";

    for(int y=0; y<yy4.size(); y++)
        valuesarray1.append(yy4.at(y)*1.44);

    archivechannel4["size"] = valuesarray4.size();
    archivechannel4["values"] = valuesarray4;
    archivechannel4["name"] = "channel_4";

    archives.append(archivechannel1);
    archives.append(archivechannel2);
    archives.append(archivechannel3);
    archives.append(archivechannel4);

    QDateTime end(QDateTime::currentDateTime());
    archive["Archives"] = archives;
    archive["StartDate"] = MainWindow::startdate;
    archive["StartTime"] = MainWindow::starttime;
    archive["EndDate"] = end.toString("dd/MM/yy");
    archive["EndTime"] = end.toString("hh:mm:ss");

    QString setstr = QJsonDocument(archive).toJson(QJsonDocument::Compact);
    QFile file(pathtofile + "archive.txt");
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
//    qDebug() << "writearchive";
}
