#include "filemanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

cFileManager::cFileManager(QObject *parent) : QObject(parent)
{

}

int cFileManager::writeChannelsSettings(QString path, QList<ChannelOptions*> listChannels, QList<Ustavka*> listUstavok)
{
    QJsonObject channeljsonobj,options;
    QJsonObject ustavkijsonobj;
    QJsonArray settings, settingsUst;

    foreach (ChannelOptions * Channel, listChannels) {
            channeljsonobj["Type"] = Channel->GetSignalType();
            channeljsonobj["Name"] = (Channel->GetChannelName());
            channeljsonobj["Units"] = Channel->GetUnitsName();
            channeljsonobj["HigherLimit"] = Channel->GetHigherLimit();
            channeljsonobj["LowerLimit"] = Channel->GetLowerLimit();
            channeljsonobj["HigherMeasLimit"] = Channel->GetHigherMeasureLimit();
            channeljsonobj["LowerMeasLimit"] = Channel->GetLowerMeasureLimit();
            channeljsonobj["Period"] = Channel->GetMeasurePeriod();
            channeljsonobj["State1HighMessage"] = (Channel->GetState1HighMessage());
            channeljsonobj["State1LowMessage"] = (Channel->GetState1LowMessage());
            channeljsonobj["State2HighMessage"] = (Channel->GetState2HighMessage());
            channeljsonobj["State2LowMessage"] = (Channel->GetState2LowMessage());
            channeljsonobj["MathString"] = Channel->GetMathString();
            channeljsonobj["MathWork"] = Channel->IsChannelMathematical();
            channeljsonobj["Diapason"] = Channel->GetDiapason();
            channeljsonobj["Dempher"] = Channel->GetDempherValue();
            channeljsonobj["RegistrationType"] = Channel->GetRegistrationType();
            settings.append(channeljsonobj);
        }


    options["count"] = listChannels.length();
    options["channels"] = settings;

    foreach (Ustavka * ust, listUstavok)
    {
        ustavkijsonobj["UstavkaChannel"] = ust->getChannel();
        ustavkijsonobj["StateHiValue"] = ust->getHiStateValue();
        ustavkijsonobj["StateLowValue"] = ust->getLowStateValue();
        ustavkijsonobj["lowHisteresis"] = ust->getHiHisteresis();
        ustavkijsonobj["lowLowsteresis"] = ust->getLowHisteresis();
        ustavkijsonobj["numRelayUp"] = ust->getnumRelayUp();
        ustavkijsonobj["numRelayDown"] = ust->getnumRelayDown();
        ustavkijsonobj["MessInHigh"] = (ust->getMessInHigh());
        ustavkijsonobj["MessNormHigh"] = (ust->getMessNormHigh());
        ustavkijsonobj["MessInLow"] = (ust->getMessInLow());
        ustavkijsonobj["MessNormLow"] = (ust->getMessNormLow());
        ustavkijsonobj["KvitirUp"] = ust->getKvitirUp();
        ustavkijsonobj["KvitirDown"] = ust->getKvitirDown();

        settingsUst.append(ustavkijsonobj);
    }

    options["countUst"] = listChannels.length();
    options["ustavki"] = settingsUst;

    QString setstr = QJsonDocument(options).toJson(QJsonDocument::Compact);
    QFile file(path);
    QTextStream out(&file);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    out << setstr;
    file.close();
    return 0;
}

int cFileManager::readChannelsSettings(QString path, QList<ChannelOptions *> listChannels, QList<Ustavka*> listUstavok)
{
    int ret = 0;

    QFile infile(path);
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    infile.close();

    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["channels"].toArray();
    QJsonObject ch;

    int count = json["count"].toInt();
    if(count < listChannels.size())
    {
        ret = -1;
    }

    int index = 0;
    foreach (ChannelOptions * channel, listChannels) {
        ch = array.at(index++).toObject();
        channel->SetHigherLimit(ch.value("HigherLimit").toDouble());
        channel->SetLowerLimit(ch.value("LowerLimit").toDouble());
        channel->SetHigherMeasureLimit(ch.value("HigherMeasLimit").toDouble());
        channel->SetLowerMeasureLimit(ch.value("LowerMeasLimit").toDouble());
        channel->SetSignalType(ch.value("Type").toInt());
        channel->SetCurSignalType(channel->GetSignalType());
        channel->SetUnitsName(ch.value("Units").toString().toUtf8());
        channel->SetMeasurePeriod(ch.value("Period").toDouble());
        channel->SetState1HighMessage(ch.value("State1HighMessage").toString().toUtf8());
        channel->SetState1LowMessage(ch.value("State1LowMessage").toString().toUtf8());
        channel->SetState2HighMessage(ch.value("State2HighMessage").toString().toUtf8());
        channel->SetState2LowMessage(ch.value("State2LowMessage").toString().toUtf8());
        channel->SetChannelName(ch.value("Name").toString().toUtf8());
        channel->SetMathematical(ch.value("MathWork").toBool());
        channel->SetMathEquation(ch.value("MathString").toString());
        channel->SetDempher(ch.value("Dempher").toInt());
        channel->SetDiapason(ch.value("Diapason").toInt());
        channel->SetRegistrationType(ch.value("RegistrationType").toInt());

    }

    count = json["countUst"].toInt();
    if(count < listUstavok.size())
    {
        ret -= 2;
    }

    array = json["ustavki"].toArray();
    QJsonObject jsonobj;
    index = 0;
    foreach (Ustavka * ust, listUstavok)
    {
        jsonobj = array.at(index++).toObject();
        ust->setUstavka(jsonobj.value("UstavkaChannel").toInt(), \
                        jsonobj.value("StateHiValue").toDouble(), \
                        jsonobj.value("StateLowValue").toDouble(), \
                        jsonobj.value("lowHisteresis").toDouble(), \
                        jsonobj.value("lowLowsteresis").toDouble(), \
                        jsonobj.value("numRelayUp").toInt(), \
                        jsonobj.value("numRelayDown").toInt() \
                        );
        ust->setMessInHigh(jsonobj.value("MessInHigh").toString().toUtf8());
        ust->setMessNormHigh(jsonobj.value("MessNormHigh").toString().toUtf8());
        ust->setMessInLow(jsonobj.value("MessInLow").toString().toUtf8());
        ust->setMessNormLow(jsonobj.value("MessNormLow").toString().toUtf8());
        ust->setKvitirUp(jsonobj.value("KvitirUp").toBool());
        ust->setKvitirDown(jsonobj.value("KvitirDown").toBool());
    }

    return ret;

}
