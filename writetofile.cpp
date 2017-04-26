#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "ui_options.h"
#include "messages.h"
#include "keyboard.h"
#include "dialog.h"
#include "channel1.h"

extern QVector<double> X_Coordinates,Y_coordinates_Chanel_1,Y_coordinates_Chanel_2,Y_coordinates_Chanel_3,Y_coordinates_Chanel_4;

extern QString pathtofile ;

void MainWindow::WriteGpio(int num, bool val)
{
    QFile filedir("/sys/class/gpio/gpio" + QString::number(num) + "/direction");
    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);
    outdir << "out";
    filedir.close();
    QFile file("/sys/class/gpio/gpio" + QString::number(num) + "/value");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << val;
    file.close();
}

void Options::WriteSystemOptionsToFile()
{
    QJsonObject systemoptions;
    QDateTime local(QDateTime::currentDateTime());
    systemoptions["Time"] = ui->timeEdit->time().toString();
    systemoptions["Date"] = ui->dateEdit->date().toString();

    if (ui->SetTrendsBtn->isChecked())
    {
        systemoptions["Display"] = Options::Trends;
        Options::DisplayParametr = Options::Trends;
    }
    if (ui->SetPolarBtn->isChecked())
    {
        systemoptions["Display"] = Options::Polar;
        Options::DisplayParametr = Options::Polar;
    }
    if (ui->SetBarsBtn->isChecked())
    {
        systemoptions["Display"] = Options::Bars;
        Options::DisplayParametr = Options::Bars;
    }
    if (ui->SetTrendsBarsBtn->isChecked())
    {
        systemoptions["Display"] = Options::TrendsBars;
        Options::DisplayParametr = Options::TrendsBars;
    }

    if (ui->SetCyfraBtn->isChecked())
    {
        systemoptions["Display"] = Options::Cyfra;
        Options::DisplayParametr = Options::Cyfra;
    }

    if (ui->SetTrendsBarsCyfraBtn->isChecked())
    {
        systemoptions["Display"] = Options::TrendsCyfraBars;
        Options::DisplayParametr = Options::TrendsCyfraBars;
    }

    if (ui->SetTrendsCyfraBtn->isChecked())
    {
        systemoptions["Display"] = Options::TrendsCyfra;
        Options::DisplayParametr = Options::TrendsCyfra;
    }

    if (ui->SetBarsCyfraBtn->isChecked())
    {
        systemoptions["Display"] = Options::BarsCyfra;
        Options::DisplayParametr = Options::BarsCyfra;
    }

    systemoptions["Calibration"] = Options::calibrationprm;
    QString setstr = QJsonDocument(systemoptions).toJson(QJsonDocument::Compact);
    QFile file(pathtofile + "systemoptions.txt");
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
}

void Options::WriteAllChannelsOptionsToFile()
{
    QJsonObject channel1,channel2,channel3,channel4,channels;
    QJsonArray settings;

    channel1["Type"] = options_channel1.GetSignalType();
    channel1["Name"] = options_channel1.GetChannelName();
    channel1["Units"] = options_channel1.GetUnitsName();
    channel1["HigherLimit"] = options_channel1.GetHigherLimit();
    channel1["LowerLimit"] = options_channel1.GetLowerLimit();
    channel1["HigherMeasLimit"] = options_channel1.GetHigherMeasureLimit();
    channel1["LowerMeasLimit"] = options_channel1.GetLowerMeasureLimit();
    channel1["Period"] = options_channel1.GetMeasurePeriod();
    channel1["State1HighMessage"] = options_channel1.GetState1HighMessage();
    channel1["State1LowMessage"] = options_channel1.GetState1LowMessage();
    channel1["State2HighMessage"] = options_channel1.GetState2HighMessage();
    channel1["State2LowMessage"] = options_channel1.GetState2LowMessage();
    channel1["State1Value"] = options_channel1.GetState1Value();
    channel1["State2Value"] = options_channel1.GetState2Value();
    channel1["MathString"] = options_channel1.GetMathString();
    channel1["MathWork"] = options_channel1.IsChannelMathematical();

    settings.append(channel1);

    channel2["Type"] = options_channel2.GetSignalType();
    channel2["Name"] = options_channel2.GetChannelName();
    channel2["Units"] = options_channel2.GetUnitsName();
    channel2["HigherLimit"] = options_channel2.GetHigherLimit();
    channel2["LowerLimit"] = options_channel2.GetLowerLimit();
    channel2["HigherMeasLimit"] = options_channel2.GetHigherMeasureLimit();
    channel2["LowerMeasLimit"] = options_channel2.GetLowerMeasureLimit();
    channel2["Period"] = options_channel2.GetMeasurePeriod();
    channel2["State1HighMessage"] = options_channel2.GetState1HighMessage();
    channel2["State1LowMessage"] = options_channel2.GetState1LowMessage();
    channel2["State2HighMessage"] = options_channel2.GetState2HighMessage();
    channel2["State2LowMessage"] = options_channel2.GetState2LowMessage();
    channel2["State1Value"] = options_channel2.GetState1Value();
    channel2["State2Value"] = options_channel2.GetState2Value();
    channel2["MathString"] = options_channel2.GetMathString();
    channel2["MathWork"] = options_channel2.IsChannelMathematical();

    settings.append(channel2);

    channel3["Type"] = options_channel3.GetSignalType();
    channel3["Name"] = options_channel3.GetChannelName();
    channel3["Units"] = options_channel3.GetUnitsName();
    channel3["HigherLimit"] = options_channel3.GetHigherLimit();
    channel3["LowerLimit"] = options_channel3.GetLowerLimit();
    channel3["HigherMeasLimit"] = options_channel3.GetHigherMeasureLimit();
    channel3["LowerMeasLimit"] = options_channel3.GetLowerMeasureLimit();
    channel3["Period"] = options_channel3.GetMeasurePeriod();
    channel3["State1HighMessage"] = options_channel3.GetState1HighMessage();
    channel3["State1LowMessage"] = options_channel3.GetState1LowMessage();
    channel3["State2HighMessage"] = options_channel3.GetState2HighMessage();
    channel3["State2LowMessage"] = options_channel3.GetState2LowMessage();
    channel3["State1Value"] = options_channel3.GetState1Value();
    channel3["State2Value"] = options_channel3.GetState2Value();
    channel3["MathString"] = options_channel3.GetMathString();
    channel3["MathWork"] = options_channel3.IsChannelMathematical();

    settings.append(channel3);

    channel4["Type"] = options_channel4.GetSignalType();
    channel4["Name"] = options_channel4.GetChannelName();
    channel4["Units"] = options_channel4.GetUnitsName();
    channel4["HigherLimit"] = options_channel4.GetHigherLimit();
    channel4["LowerLimit"] = options_channel4.GetLowerLimit();
    channel4["HigherMeasLimit"] = options_channel4.GetHigherMeasureLimit();
    channel4["LowerMeasLimit"] = options_channel4.GetLowerMeasureLimit();
    channel4["Period"] = options_channel4.GetMeasurePeriod();
    channel4["State1HighMessage"] = options_channel4.GetState1HighMessage();
    channel4["State1LowMessage"] = options_channel4.GetState1LowMessage();
    channel4["State2HighMessage"] = options_channel4.GetState2HighMessage();
    channel4["State2LowMessage"] = options_channel4.GetState2LowMessage();
    channel4["State1Value"] = options_channel4.GetState1Value();
    channel4["State2Value"] = options_channel4.GetState2Value();
    channel4["MathString"] = options_channel4.GetMathString();
    channel4["MathWork"] = options_channel4.IsChannelMathematical();

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

void MessageWrite::LogAddMessage(QString nm)
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

    for(int y=0; y<Y_coordinates_Chanel_1.size(); y++)
        valuesarray1.append(Y_coordinates_Chanel_1.at(y));

    archivechannel1["size"] = valuesarray1.size();
    archivechannel1["values"] = valuesarray1;
    archivechannel1["name"] = "channel_1";

    for(int y=0; y<Y_coordinates_Chanel_2.size(); y++)
        valuesarray2.append(Y_coordinates_Chanel_2.at(y));

    archivechannel2["size"] = valuesarray2.size();
    archivechannel2["values"] = valuesarray2;
    archivechannel2["name"] = "channel_2";

    for(int y=0; y<Y_coordinates_Chanel_3.size(); y++)
        valuesarray3.append(Y_coordinates_Chanel_3.at(y)*1.33);

    archivechannel3["size"] = valuesarray3.size();
    archivechannel3["values"] = valuesarray3;
    archivechannel3["name"] = "channel_3";

    for(int y=0; y<Y_coordinates_Chanel_4.size(); y++)
        valuesarray1.append(Y_coordinates_Chanel_4.at(y)*1.44);

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
