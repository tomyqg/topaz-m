#include "filemanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <steel.h>
#include <defines.h>

extern QList<cSteel*> listSteel;
extern typeSteelTech steelTech[];

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

int cFileManager::readSteelsSettings(QString path)
{
    int ret = 0;

    QFile infile(path);
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    infile.close();

    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray steelsArray = json["Steels"].toArray();

    int count = json["CountSteel"].toInt();
    if(count < listSteel.size())
    {
        ret = -1;
    }

    int index = 0;
    foreach (cSteel * steel, listSteel) {
        QJsonObject steelObj = steelsArray.at(index++).toObject();
        steel->technology = &steelTech[steelObj.value("NumTech").toInt()];
        QJsonArray relais = steelObj["Relais"].toArray();
        for(int i = 0; (i < SUM_RELAYS) && (i < relais.size()); i++)
        {
            steel->relais[i] = relais.at(i).toInt();
        }
    }

    count = json["CountTech"].toInt();
    if(count < NUM_TECHNOLOGIES)
    {
        ret -= 2;
    }

    QJsonArray arrayTech = json["Technologies"].toArray();
    for(int i = 0; (i < count) && (i < NUM_TECHNOLOGIES); i++)
    {
        QJsonObject techObj = arrayTech.at(i).toObject();
//        int size = min(techObj.value("Name").toString().size(), SIZE_TECH_NAME_STR);
        strcpy(steelTech[i].name, techObj.value("Name").toString().toUtf8().data());
//        steelTech[i].name = techObj.value("Name").toString().toLatin1().data();
        steelTech[i].nSt = techObj.value("TermoCouple").toInt();
        steelTech[i].COH = techObj.value("SensorActivity").toInt();
        steelTech[i].b1 = techObj.value("Crystallization").toInt();
        steelTech[i].dSt = techObj.value("TimeSquareTemp").toDouble();
        steelTech[i].dt = techObj.value("RangeTemp").toDouble();
        steelTech[i].tPt = techObj.value("TimeMeasureTemp").toDouble();
        steelTech[i].LPtl = techObj.value("LowTemp").toInt();
        steelTech[i].LPth = techObj.value("HiTemp").toInt();
        steelTech[i].dSE = techObj.value("TimeSquareEDS").toDouble();
        steelTech[i].dE = techObj.value("RangeEDS").toDouble();
        steelTech[i].tPE = techObj.value("TimeMeasureEDS").toDouble();
        steelTech[i].b2 = techObj.value("MassCoeff").toInt();
        steelTech[i].O = techObj.value("FinalOx").toInt();
        steelTech[i].Y = techObj.value("Assimilation").toInt();
        steelTech[i].G = techObj.value("MassMelting").toInt();
    }

    return ret;
}

int cFileManager::writeSystemOptionsToFile(QString path, cSystemOptions * opt)
{
    QJsonObject systemoptions;
//    systemoptions["Time"] = GetNewTimeString();
//    systemoptions["Date"] = GetNewDateString();
//    systemoptions["Display"] = GetCurrentDisplayParametr();
//    systemoptions["Calibration"] = GetCalibration();
//    systemoptions["Resolution"] = GetNewDisplayResolution();
    systemoptions["Arrows"] = opt->arrows;
    systemoptions["Display"] = opt->display;
    systemoptions["Autoscale"] = opt->autoscale;
    QString setstr = QJsonDocument(systemoptions).toJson(QJsonDocument::Compact);
    QFile file(path);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
    return 0;
}

int cFileManager::readSystemOptionsFromFile(QString path, cSystemOptions * opt)
{
    QFile infile(path);
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
//    StackedOptions::calibrationprm = json["Calibration"].toString();
//    StackedOptions::DisplayParametr = json["Display"].toInt();
//    StackedOptions::displayResolution = json["Resolution"].toString();
    opt->arrows = json["Arrows"].toBool();
    opt->display = json["Display"].toInt();
    opt->autoscale = json["Autoscale"].toBool();
    infile.close();
}

int cFileManager::writeSteelsSettings(QString path)
{
    QJsonObject steeljsonobj, options, techjsobj;
    QJsonArray settingssteel, settingstech;

    foreach (cSteel * steel, listSteel) {
        steeljsonobj["NumSteel"] = steel->num;
        QJsonArray relais;
        for(int i = 0; i < SUM_RELAYS; i++)
        {
            relais.append(steel->relais[i]);
        }
        steeljsonobj["Relais"] = relais;
        steeljsonobj["NumTech"] = steel->technology->num;
        settingssteel.append(steeljsonobj);
    }
    options["CountSteel"] = listSteel.length();
    options["Steels"] = settingssteel;

    for(int i = 0; i < NUM_TECHNOLOGIES; i++)
    {
        techjsobj["NumTech"] = steelTech[i].num;
        techjsobj["Name"] = QString(steelTech[i].name);
        techjsobj["TermoCouple"] = steelTech[i].nSt;
        techjsobj["SensorActivity"] = steelTech[i].COH;
        techjsobj["Crystallization"] = steelTech[i].b1;
        techjsobj["TimeSquareTemp"] = steelTech[i].dSt;
        techjsobj["RangeTemp"] = steelTech[i].dt;
        techjsobj["TimeMeasureTemp"] = steelTech[i].tPt;
        techjsobj["LowTemp"] = steelTech[i].LPtl;
        techjsobj["HiTemp"] = steelTech[i].LPth;
        techjsobj["TimeSquareEDS"] = steelTech[i].dSE;
        techjsobj["RangeEDS"] = steelTech[i].dE;
        techjsobj["TimeMeasureEDS"] = steelTech[i].tPE;
        techjsobj["MassCoeff"] = steelTech[i].b2;
        techjsobj["FinalOx"] = steelTech[i].O;
        techjsobj["Assimilation"] = steelTech[i].Y;
        techjsobj["MassMelting"] = steelTech[i].G;
        settingstech.append(techjsobj);
    }

    options["CountTech"] = NUM_TECHNOLOGIES;
    options["Technologies"] = settingstech;

    QString setstr = QJsonDocument(options).toJson(QJsonDocument::Compact);
    QFile file(path);
    QTextStream out(&file);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    out << setstr;
    file.close();

    return 0;
}
