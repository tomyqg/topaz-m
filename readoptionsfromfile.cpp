#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "dialog.h"

extern QString pathtofile;

void Options::ReadSystemOptionsFromFile()
{
    QFile infile(pathtofile + "systemoptions.txt");
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    Options::calibrationprm = json["Calibration"].toString();
    Options::DisplayParametr = json["Display"].toInt();
    infile.close();
}

void Options::ReadChannelsOptionsFromFile()
{
    QFile infile(pathtofile + "options.txt");
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["channels"].toArray();
    infile.close();

    QJsonObject ch1 = array.at(0).toObject();
    options_channel1.SetHigherLimit(ch1.value("HigherLimit").toDouble());
    options_channel1.SetLowerLimit(ch1.value("LowerLimit").toDouble());
    options_channel1.SetHigherMeasureLimit(ch1.value("HigherMeasLimit").toDouble());
    options_channel1.SetLowerMeasureLimit(ch1.value("LowerMeasLimit").toDouble());
    options_channel1.SetSignalType(ch1.value("Type").toDouble());
    options_channel1.SetUnitsName(ch1.value("Units").toString());
    options_channel1.SetMeasurePeriod(ch1.value("Period").toDouble());
    options_channel1.SetState1HighMessage(ch1.value("State1HighMessage").toString());
    options_channel1.SetState1LowMessage(ch1.value("State1LowMessage").toString());
    options_channel1.SetState2HighMessage(ch1.value("State2HighMessage").toString());
    options_channel1.SetState2LowMessage(ch1.value("State2LowMessage").toString());
    options_channel1.SetState1Value(ch1.value("State1Value").toDouble());
    options_channel1.SetState2Value(ch1.value("State2Value").toDouble());
    options_channel1.SetChannelName(ch1.value("Name").toString());
    options_channel1.SetMathEquation(ch1.value("MathString").toString());
    options_channel1.SetMathematical(ch1.value("MathWork").toBool());

    QJsonObject ch2 = array.at(1).toObject();
    options_channel2.SetHigherLimit(ch2.value("HigherLimit").toDouble());
    options_channel2.SetLowerLimit(ch2.value("LowerLimit").toDouble());
    options_channel2.SetHigherMeasureLimit(ch2.value("HigherMeasLimit").toDouble());
    options_channel2.SetLowerMeasureLimit(ch2.value("LowerMeasLimit").toDouble());
    options_channel2.SetSignalType(ch2.value("Type").toDouble());
    options_channel2.SetUnitsName(ch2.value("Units").toString());
    options_channel2.SetMeasurePeriod(ch2.value("Period").toDouble());
    options_channel2.SetState1HighMessage(ch2.value("State1HighMessage").toString());
    options_channel2.SetState1LowMessage(ch2.value("State1LowMessage").toString());
    options_channel2.SetState2HighMessage(ch2.value("State2HighMessage").toString());
    options_channel2.SetState2LowMessage(ch2.value("State2LowMessage").toString());
    options_channel2.SetState1Value(ch2.value("State1Value").toDouble());
    options_channel2.SetState2Value(ch2.value("State2Value").toDouble());
    options_channel2.SetChannelName(ch2.value("Name").toString());
    options_channel2.SetMathEquation(ch2.value("MathString").toString());
    options_channel2.SetMathematical(ch2.value("MathWork").toBool());

    QJsonObject ch3 = array.at(2).toObject();
    options_channel3.SetHigherLimit(ch3.value("HigherLimit").toDouble());
    options_channel3.SetLowerLimit(ch3.value("LowerLimit").toDouble());
    options_channel3.SetHigherMeasureLimit(ch3.value("HigherMeasLimit").toDouble());
    options_channel3.SetLowerMeasureLimit(ch3.value("LowerMeasLimit").toDouble());
    options_channel3.SetSignalType(ch3.value("Type").toDouble());
    options_channel3.SetUnitsName(ch3.value("Units").toString());
    options_channel3.SetMeasurePeriod(ch3.value("Period").toDouble());
    options_channel3.SetState1HighMessage(ch3.value("State1HighMessage").toString());
    options_channel3.SetState1LowMessage(ch3.value("State1LowMessage").toString());
    options_channel3.SetState2HighMessage(ch3.value("State2HighMessage").toString());
    options_channel3.SetState2LowMessage(ch3.value("State2LowMessage").toString());
    options_channel3.SetState1Value(ch3.value("State1Value").toDouble());
    options_channel3.SetState2Value(ch3.value("State2Value").toDouble());
    options_channel3.SetChannelName(ch3.value("Name").toString());
    options_channel3.SetMathEquation(ch3.value("MathString").toString());
    options_channel3.SetMathematical(ch3.value("MathWork").toBool());

    QJsonObject ch4 = array.at(3).toObject();
    options_channel4.SetHigherLimit(ch4.value("HigherLimit").toDouble());
    options_channel4.SetLowerLimit(ch4.value("LowerLimit").toDouble());
    options_channel4.SetHigherMeasureLimit(ch4.value("HigherMeasLimit").toDouble());
    options_channel4.SetLowerMeasureLimit(ch4.value("LowerMeasLimit").toDouble());
    options_channel4.SetSignalType(ch4.value("Type").toDouble());
    options_channel4.SetUnitsName(ch4.value("Units").toString());
    options_channel4.SetMeasurePeriod(ch4.value("Period").toDouble());
    options_channel4.SetState1HighMessage(ch4.value("State1HighMessage").toString());
    options_channel4.SetState1LowMessage(ch4.value("State1LowMessage").toString());
    options_channel4.SetState2HighMessage(ch4.value("State2HighMessage").toString());
    options_channel4.SetState2LowMessage(ch4.value("State2LowMessage").toString());
    options_channel4.SetState1Value(ch4.value("State1Value").toDouble());
    options_channel4.SetState2Value(ch4.value("State2Value").toDouble());
    options_channel4.SetChannelName(ch4.value("Name").toString());
    options_channel4.SetMathEquation(ch4.value("MathString").toString());
    options_channel4.SetMathematical(ch4.value("MathWork").toBool());
}

QJsonArray MessageWrite::LogMessageRead()
{
    QFile file(pathtofile + "Log.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["messagesqueue"].toArray();
    MessageWrite::messagesqueue = array;
    file.close();
    //    qDebug() << MessageWrite::messagesqueue;
    return MessageWrite::messagesqueue;
}

void ChannelOptions::ReadSingleChannelOptionFromFile(int channel)
{
    QFile infile(pathtofile + "options.txt");
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["channels"].toArray();
    QJsonObject ch = array.at(channel-1).toObject();

    this->SetHigherLimit(ch.value("HigherLimit").toDouble());
    this->SetLowerLimit(ch.value("LowerLimit").toDouble());
    this->SetHigherMeasureLimit(ch.value("HigherMeasLimit").toDouble());
    this->SetLowerMeasureLimit(ch.value("LowerMeasLimit").toDouble());
    this->SetSignalType(ch.value("Type").toDouble());
    this->SetUnitsName(ch.value("Units").toString());
    this->SetMeasurePeriod(ch.value("Period").toDouble());
    this->SetState1HighMessage(ch.value("State1HighMessage").toString());
    this->SetState1LowMessage(ch.value("State1LowMessage").toString());
    this->SetState2HighMessage(ch.value("State2HighMessage").toString());
    this->SetState2LowMessage(ch.value("State2LowMessage").toString());
    this->SetState1Value(ch.value("State1Value").toDouble());
    this->SetState2Value(ch.value("State2Value").toDouble());
    this->SetChannelName(ch.value("Name").toString());
    this->SetMathematical(ch.value("MathWork").toBool());
    this->SetMathEquation(ch.value("MathString").toString());

    infile.close();
}
