#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "dialog.h"

extern QString pathtofile;

void Options::readsystemoptionsfromfile()
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

void Options::readoptionsfromfile()
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
    options1.SetHigherLimit(ch1.value("HigherLimit").toDouble());
    options1.SetLowerLimit(ch1.value("LowerLimit").toDouble());
    options1.SetHigherMeasureLimit(ch1.value("HigherMeasLimit").toDouble());
    options1.SetLowerMeasureLimit(ch1.value("LowerMeasLimit").toDouble());
    options1.SetSignalType(ch1.value("Type").toDouble());
    options1.SetUnitsName(ch1.value("Units").toString());
    options1.SetMeasurePeriod(ch1.value("Period").toDouble());
    options1.SetState1HighMessage(ch1.value("State1HighMessage").toString());
    options1.SetState1LowMessage(ch1.value("State1LowMessage").toString());
    options1.SetState2HighMessage(ch1.value("State2HighMessage").toString());
    options1.SetState2LowMessage(ch1.value("State2LowMessage").toString());
    options1.SetState1Value(ch1.value("State1Value").toDouble());
    options1.SetState2Value(ch1.value("State2Value").toDouble());
    options1.SetChannelName(ch1.value("Name").toString());
    options1.SetMathEquation(ch1.value("MathString").toString());
    options1.SetMathematical(ch1.value("MathWork").toBool());

    QJsonObject ch2 = array.at(1).toObject();
    options2.SetHigherLimit(ch2.value("HigherLimit").toDouble());
    options2.SetLowerLimit(ch2.value("LowerLimit").toDouble());
    options2.SetHigherMeasureLimit(ch2.value("HigherMeasLimit").toDouble());
    options2.SetLowerMeasureLimit(ch2.value("LowerMeasLimit").toDouble());
    options2.SetSignalType(ch2.value("Type").toDouble());
    options2.SetUnitsName(ch2.value("Units").toString());
    options2.SetMeasurePeriod(ch2.value("Period").toDouble());
    options2.SetState1HighMessage(ch2.value("State1HighMessage").toString());
    options2.SetState1LowMessage(ch2.value("State1LowMessage").toString());
    options2.SetState2HighMessage(ch2.value("State2HighMessage").toString());
    options2.SetState2LowMessage(ch2.value("State2LowMessage").toString());
    options2.SetState1Value(ch2.value("State1Value").toDouble());
    options2.SetState2Value(ch2.value("State2Value").toDouble());
    options2.SetChannelName(ch2.value("Name").toString());
    options2.SetMathEquation(ch2.value("MathString").toString());
    options2.SetMathematical(ch2.value("MathWork").toBool());

    QJsonObject ch3 = array.at(2).toObject();
    options3.SetHigherLimit(ch3.value("HigherLimit").toDouble());
    options3.SetLowerLimit(ch3.value("LowerLimit").toDouble());
    options3.SetHigherMeasureLimit(ch3.value("HigherMeasLimit").toDouble());
    options3.SetLowerMeasureLimit(ch3.value("LowerMeasLimit").toDouble());
    options3.SetSignalType(ch3.value("Type").toDouble());
    options3.SetUnitsName(ch3.value("Units").toString());
    options3.SetMeasurePeriod(ch3.value("Period").toDouble());
    options3.SetState1HighMessage(ch3.value("State1HighMessage").toString());
    options3.SetState1LowMessage(ch3.value("State1LowMessage").toString());
    options3.SetState2HighMessage(ch3.value("State2HighMessage").toString());
    options3.SetState2LowMessage(ch3.value("State2LowMessage").toString());
    options3.SetState1Value(ch3.value("State1Value").toDouble());
    options3.SetState2Value(ch3.value("State2Value").toDouble());
    options3.SetChannelName(ch3.value("Name").toString());
    options3.SetMathEquation(ch3.value("MathString").toString());
    options3.SetMathematical(ch3.value("MathWork").toBool());

    QJsonObject ch4 = array.at(3).toObject();
    options4.SetHigherLimit(ch4.value("HigherLimit").toDouble());
    options4.SetLowerLimit(ch4.value("LowerLimit").toDouble());
    options4.SetHigherMeasureLimit(ch4.value("HigherMeasLimit").toDouble());
    options4.SetLowerMeasureLimit(ch4.value("LowerMeasLimit").toDouble());
    options4.SetSignalType(ch4.value("Type").toDouble());
    options4.SetUnitsName(ch4.value("Units").toString());
    options4.SetMeasurePeriod(ch4.value("Period").toDouble());
    options4.SetState1HighMessage(ch4.value("State1HighMessage").toString());
    options4.SetState1LowMessage(ch4.value("State1LowMessage").toString());
    options4.SetState2HighMessage(ch4.value("State2HighMessage").toString());
    options4.SetState2LowMessage(ch4.value("State2LowMessage").toString());
    options4.SetState1Value(ch4.value("State1Value").toDouble());
    options4.SetState2Value(ch4.value("State2Value").toDouble());
    options4.SetChannelName(ch4.value("Name").toString());
    options4.SetMathEquation(ch4.value("MathString").toString());
    options4.SetMathematical(ch4.value("MathWork").toBool());
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

void ChannelOptions::readoptionsfromfile(int channel)
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
