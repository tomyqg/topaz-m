#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "options.h"
#include "keyboard.h"
#include "defines.h"

QString pathtofile2 = "";

extern QList<Ustavka *> listUstavok;

//void Options::ReadSystemOptionsFromFile()
//{
//    QFile infile(pathtosystemoptions);
//    infile.open(QIODevice::ReadOnly);
//    QTextStream in(&infile);
//    QString sss = in.readLine();
//    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
//    QJsonObject json = doc.object();
//    Options::calibrationprm = json["Calibration"].toString();
//    Options::DisplayParametr = json["Display"].toInt();
//    Options::displayResolution = json["Resolution"].toString();

//    infile.close();
//}

//void Options::ReadChannelsOptionsFromFile()
//{
//    QFile infile(pathtooptions);
//    infile.open(QIODevice::ReadOnly);
//    QTextStream in(&infile);
//    QString sss = in.readLine();
//    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
//    QJsonObject json = doc.object();
//    QJsonArray array = json["channels"].toArray();
//    infile.close();


//    QJsonArray settings;

//    QList<ChannelOptions *> ChannelsObjectsList;

//    ChannelsObjectsList.append(&options_channel1);
//    ChannelsObjectsList.append(&options_channel2);
//    ChannelsObjectsList.append(&options_channel3);
//    ChannelsObjectsList.append(&options_channel4);
//    QJsonObject jsonobj;

//    int index = 0;

//    foreach (ChannelOptions * Channel, ChannelsObjectsList) {
//        jsonobj = array.at(index).toObject();

//        Channel->SetHigherLimit(jsonobj.value("HigherLimit").toDouble());
//        Channel->SetLowerLimit(jsonobj.value("LowerLimit").toDouble());
//        Channel->SetHigherMeasureLimit(jsonobj.value("HigherMeasLimit").toDouble());
//        Channel->SetLowerMeasureLimit(jsonobj.value("LowerMeasLimit").toDouble());
//        Channel->SetSignalType(jsonobj.value("Type").toDouble());
//        Channel->SetCurSignalType(Channel->GetSignalType());
//        Channel->SetUnitsName(jsonobj.value("Units").toString());
//        Channel->SetMeasurePeriod(jsonobj.value("Period").toDouble());
//        Channel->SetState1HighMessage(jsonobj.value("State1HighMessage").toString());
//        Channel->SetState1LowMessage(jsonobj.value("State1LowMessage").toString());
//        Channel->SetState2HighMessage(jsonobj.value("State2HighMessage").toString());
//        Channel->SetState2LowMessage(jsonobj.value("State2LowMessage").toString());
//        Channel->SetChannelName(jsonobj.value("Name").toString());
//        Channel->SetMathEquation(jsonobj.value("MathString").toString());
//        Channel->SetMathematical(jsonobj.value("MathWork").toBool());
//        Channel->SetRegistrationType(jsonobj.value("RegistrationType").toInt());
//        Channel->SetDempher(jsonobj.value("Dempher").toInt());
//        Channel->SetDiapason(jsonobj.value("Diapason").toInt());
//        index ++ ;
//    }
//}

//QJsonArray MessageWrite::LogMessageRead()
//{
//    QFile file(pathtomessages);
//    while(file.isOpen());   //подождать пока файл не закроется другим потоком
//    file.open(QIODevice::ReadOnly);
//    QTextStream in(&file);
//    QString sss = in.readLine();
//    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
//    QJsonObject json = doc.object();
//    QJsonArray array = json["messages"].toArray();

//    mMessQueue.lock();
//    MessageWrite::messagesqueue = array;
//    mMessQueue.unlock();

//    file.close();
//    return MessageWrite::messagesqueue;
//}

void ChannelOptions::ReadSingleChannelOptionFromFile(int channel)
{
    QFile infile(pathtooptions);
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
    this->SetSignalType(ch.value("Type").toInt());
//    this->SetCurSignalType(this->GetSignalType());
    this->SetUnitsName(ch.value("Units").toString().toUtf8());
    this->SetMeasurePeriod(ch.value("Period").toDouble());
    this->SetState1HighMessage(ch.value("State1HighMessage").toString().toUtf8());
    this->SetState1LowMessage(ch.value("State1LowMessage").toString().toUtf8());
    this->SetState2HighMessage(ch.value("State2HighMessage").toString().toUtf8());
    this->SetState2LowMessage(ch.value("State2LowMessage").toString().toUtf8());
    this->SetChannelName(ch.value("Name").toString().toUtf8());
    this->SetMathematical(ch.value("MathWork").toBool());
    this->SetMathEquation(ch.value("MathString").toString());
    this->SetDempher(ch.value("Dempher").toInt());
    this->SetDiapason(ch.value("Diapason").toInt());
    this->SetRegistrationType(ch.value("RegistrationType").toInt());

    infile.close();
}

void MainWindow::ReadUstavkiFromFile()
{
    QFile infile(pathtooptions);
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["ustavki"].toArray();
    infile.close();
    QJsonObject jsonobj;

    int index = 0;
    foreach (Ustavka * ust, listUstavok)
    {
        jsonobj = array.at(index).toObject();
        ust->setNum(index);
        ust->setUstavka(
                    /*jsonobj.value("Num").toInt(),*/ \
                    jsonobj.value("Identifikator").toString().toUtf8(), \
                    jsonobj.value("UstavkaChannel").toInt(), \
                    jsonobj.value("TypeFix").toBool(), \
                    jsonobj.value("StateHiValue").toDouble(), \
                    /*jsonobj.value("StateLowValue").toDouble(),*/ \
                    jsonobj.value("lowHisteresis").toDouble(), \
                    /*jsonobj.value("lowLowsteresis").toDouble(),*/ \
                    jsonobj.value("numRelayUp").toInt() \
                    /*jsonobj.value("numRelayDown").toInt()*/ \
                    );
        ust->setMessInHigh(jsonobj.value("MessInHigh").toString().toUtf8());
        ust->setMessNormHigh(jsonobj.value("MessNormHigh").toString().toUtf8());
//        ust->setMessInLow(jsonobj.value("MessInLow").toString().toUtf8());
//        ust->setMessNormLow(jsonobj.value("MessNormLow").toString().toUtf8());
        ust->setKvitirUp(jsonobj.value("KvitirUp").toBool());
//        ust->setKvitirDown(jsonobj.value("KvitirDown").toBool());
        index++;
    }
}
