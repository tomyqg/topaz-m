#include "filemanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <assert.h>
#include <steel.h>
#include <defines.h>
#include "Channels/group_channels.h"
#include "Channels/math_channel.h"

extern QList<ChannelOptions *> listChannels;
extern QList<cGroupChannels*> listGroup;
extern QList<cMathChannel*> listMath;
extern QList<Ustavka *> listUstavok;
extern QList<cSteel*> listSteel;
extern typeSteelTech steelTech[];

cFileManager::cFileManager(QObject *parent) : QObject(parent)
{

}

int cFileManager::writeChannelsSettings(QString path/*, QList<ChannelOptions*> listChannels*/)
{
    QJsonObject channeljsonobj,options;
    QJsonObject ustavkijsonobj;
    QJsonArray settings, settingsUst;

    int countCh = 0;
    foreach (ChannelOptions * Channel, listChannels) {
#ifdef RANDOM_CHAN
        if((Channel->getNum() <= NUM_CHAN_IN_4AI) || Channel->enable)
#else
        if(Channel->enable)
#endif
        {
            channeljsonobj["Slot"] = Channel->getSlot();
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
            channeljsonobj["ShiftCJ"] = Channel->getShiftColdJunction();
            channeljsonobj["EnShiftCJ"] = Channel->getStateColdJunction();
            channeljsonobj["Scheme"] = Channel->getShema();
            channeljsonobj["Dempher"] = Channel->GetDempherValue();
            channeljsonobj["RegistrationType"] = Channel->GetRegistrationType();
            settings.append(channeljsonobj);
            countCh++;
        }
    }

    options["count"] = countCh;
    options["channels"] = settings;

    foreach (Ustavka * ust, listUstavok)
    {
//        ustavkijsonobj["Num"] = ust->getNum();
        ustavkijsonobj["Identifikator"] = ust->getIdentifikator();
        ustavkijsonobj["UstavkaChannel"] = ust->getChannel();
        ustavkijsonobj["TypeFix"] = ust->getTypeFix();
        ustavkijsonobj["StateHiValue"] = ust->getHiStateValue();
//        ustavkijsonobj["StateLowValue"] = ust->getLowStateValue();
        ustavkijsonobj["lowHisteresis"] = ust->getHiHisteresis();
//        ustavkijsonobj["lowLowsteresis"] = ust->getLowHisteresis();
        ustavkijsonobj["numRelayUp"] = ust->getnumRelayUp();
//        ustavkijsonobj["numRelayDown"] = ust->getnumRelayDown();
        ustavkijsonobj["MessInHigh"] = (ust->getMessInHigh());
        ustavkijsonobj["MessNormHigh"] = (ust->getMessNormHigh());
//        ustavkijsonobj["MessInLow"] = (ust->getMessInLow());
//        ustavkijsonobj["MessNormLow"] = (ust->getMessNormLow());
        ustavkijsonobj["KvitirUp"] = ust->getKvitirUp();
//        ustavkijsonobj["KvitirDown"] = ust->getKvitirDown();

        settingsUst.append(ustavkijsonobj);
    }

    options["countUst"] = listUstavok.length();
    options["ustavki"] = settingsUst;



    QString setstr = QJsonDocument(options).toJson(QJsonDocument::Compact);
    QFile file(path);
    QTextStream out(&file);
    if(file.open(QIODevice::WriteOnly))
    {
        file.resize(0); // clear file
        out << setstr;
        file.close();
    }
    else
    {
        return 1; //есть проблемы в открыти файла
    }


    return 0;
}

int cFileManager::readChannelsSettings(QString path)
{

    QFile infile(path);
    QString sss;
    if(!infile.exists()) return 2;  //файл не доступен
    if(infile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&infile);
        sss = in.readLine();
        infile.close();
    }
    else
    {
        return 1;   //файл не открывается
    }

    if(sss.size() == 0) return 4;    //файл пустой


    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["channels"].toArray();
    QJsonObject ch;

    int count = json["count"].toInt();
//    assert(count = array.size());
    if(count != array.size())
    {
        cLogger mk(pathtomessages, cLogger::UI);
        mk.addMess("File " + QString(pathtooptions) + " incorrect", cLogger::ERR);
        return 3;
    }

//    int size = listChannels.size();
//    if(count > size)
//    {
//        for(int i = size; i < count; i++)
//        {
//            ChannelOptions * ch = new ChannelOptions();
//            ch->SetCurrentChannelValue(0);
//            ch->setNum(i+1);
////            connect(ch, SIGNAL(updateSignal(int)), parent, SLOT(&MainWindow::updateChannelSlot(int)));
//            listChannels.append(ch);
//        }
//    }

    int index = 0;
    foreach (ChannelOptions * channel, listChannels) {
        if(index < count)
        {
            ch = array.at(index).toObject();
//            channel->enable = true;
//            channel->setSlot(ch.value("Slot").toInt());
//            channel->setSlotChannel(index%NUM_CHAN_IN_4AI);
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
            channel->setShiftColdJunction(ch.value("ShiftCJ").toDouble());
            channel->enableColdJunction(ch.value("EnShiftCJ").toInt());
            channel->SetDiapasonShema(ch.value("Diapason").toInt(), ch.value("Scheme").toInt());
//            channel->setShema(ch.value("Scheme").toInt());
            channel->SetRegistrationType(ch.value("RegistrationType").toInt());
        }
        else
        {
            channel->enable = false;
        }
        index++;
    }

    count = json["countUst"].toInt();
    // если уставки описаны в файле настроек, то очистить список по умолчанию
    // и пересоздать
    if(count != 0)
    {
        listUstavok.clear();
        for(int i = 0; i < count; i ++)
        {
            Ustavka *ust = new Ustavka();
            ust->setNum(i);
            ust->setIdentifikator("Limit " + QString::number(i+1));
            listUstavok.append(ust);
        }
    }

    array = json["ustavki"].toArray();
    QJsonObject jsonobj;
    index = 0;
    foreach (Ustavka * ust, listUstavok)
    {
        jsonobj = array.at(index++).toObject();
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
    }

    return 0;

}

int cFileManager::readSteelsSettings(QString path)
{

    int ret = 0;
    QFile infile(path);
    QString sss;
    if(infile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&infile);
        sss = in.readLine();
        infile.close();
    }
    else
    {
        return 1;
    }

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

    return 0;
}

int cFileManager::writeSystemOptionsToFile(QString path, cSystemOptions * opt)
{
    QJsonObject systemoptions;
    QJsonObject options, groupsjsonobj, mathjsonobj;
    QJsonArray settingsGroup, settingsMath;

    systemoptions["Arrows"] = opt->arrows;
    systemoptions["Display"] = opt->display;
    systemoptions["Autoscale"] = opt->autoscale;
    systemoptions["Brightness"] = opt->brightness;
    systemoptions["TypeMultigraph"] = opt->typeMultigraph;

    foreach (cGroupChannels * group, listGroup) {
        groupsjsonobj["Enabled"] = group->enabled;
        groupsjsonobj["Name"] = group->groupName;

        groupsjsonobj["Channel1"] = group->channel[0];
        groupsjsonobj["Channel2"] = group->channel[1];
        groupsjsonobj["Channel3"] = group->channel[2];
        groupsjsonobj["Channel4"] = group->channel[3];

        groupsjsonobj["Math1"] = group->mathChannel[0];
        groupsjsonobj["Math2"] = group->mathChannel[1];
        groupsjsonobj["Math3"] = group->mathChannel[2];
        groupsjsonobj["Math4"] = group->mathChannel[3];

        groupsjsonobj["Type1"] = group->typeInput[0];
        groupsjsonobj["Type2"] = group->typeInput[1];
        groupsjsonobj["Type3"] = group->typeInput[2];
        groupsjsonobj["Type4"] = group->typeInput[3];
        settingsGroup.append(groupsjsonobj);
    }

    options["countGrp"] = listGroup.length();
    options["groups"] = settingsGroup;

    foreach (cMathChannel * math, listMath)
    {
        mathjsonobj["Name"] = math->getName();
        mathjsonobj["MathString"] = math->GetMathString();
        mathjsonobj["HigherMeasureLimit"] = math->GetHigherMeasureLimit();
        mathjsonobj["LowerMeasureLimit"] = math->GetLowerMeasureLimit();
        mathjsonobj["X1"] = math->numChannel[0];
        mathjsonobj["X2"] = math->numChannel[1];
        mathjsonobj["X3"] = math->numChannel[2];
        mathjsonobj["X4"] = math->numChannel[3];

        settingsMath.append(mathjsonobj);
    }

    options["countMath"] = listMath.length();
    options["mathChannels"] = settingsMath;

    systemoptions["Options"] = options;
    QString setstr = QJsonDocument(systemoptions).toJson(QJsonDocument::Compact);
    QFile file(path);
    if(file.open(QIODevice::WriteOnly))
    {
        file.resize(0); // clear file
        QTextStream out(&file);
        out << setstr;
        file.close();
    }
    else
    {
        return 1;   //ошибка открытия файла
    }
    return 0;
}

int cFileManager::readSystemOptionsFromFile(QString path, cSystemOptions * opt)
{
    QFile infile(path);
    QString sss;
    QJsonArray array;
    QJsonObject jsonobj;

    if(infile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&infile);
        sss = in.readLine();
        infile.close();
    }
    else
    {
        return 1; //есть проблемы с открытием файла
    }
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
//    StackedOptions::calibrationprm = json["Calibration"].toString();
//    StackedOptions::DisplayParametr = json["Display"].toInt();
//    StackedOptions::displayResolution = json["Resolution"].toString();
    opt->arrows = json["Arrows"].toBool();
    opt->display = json["Display"].toInt();
    opt->autoscale = json["Autoscale"].toBool();
    opt->brightness = 80;   // перестраховка на случай отсутствия найсройки в файле
    opt->brightness = json["Brightness"].toInt();
    opt->typeMultigraph = (cSystemOptions::TypeMultigraphEnum)(json["TypeMultigraph"].toInt());

    QJsonObject options = json["Options"].toObject();
    int countGroup = options["countGrp"].toInt();
    array = options["groups"].toArray();

    int index = 0;

    while(countGroup > listGroup.size())
    {
        cGroupChannels * group = new cGroupChannels;
        listGroup.append(group);
    }

    foreach (cGroupChannels * group, listGroup) {
        if(countGroup <= index)
        {
            group->enabled = false;
        }
        else
        {
            jsonobj = array.at(index).toObject();
            group->enabled = jsonobj.value("Enabled").toBool();
            group->groupName = jsonobj.value("Name").toString().toUtf8();
            group->typeInput[0] = jsonobj.value("Type1").toInt();
            group->typeInput[1] = jsonobj.value("Type2").toInt();
            group->typeInput[2] = jsonobj.value("Type3").toInt();
            group->typeInput[3] = jsonobj.value("Type4").toInt();
            group->channel[0] = jsonobj.value("Channel1").toInt();
            group->channel[1] = jsonobj.value("Channel2").toInt();
            group->channel[2] = jsonobj.value("Channel3").toInt();
            group->channel[3] = jsonobj.value("Channel4").toInt();
            group->mathChannel[0] = jsonobj.value("Math1").toInt();
            group->mathChannel[1] = jsonobj.value("Math2").toInt();
            group->mathChannel[2] = jsonobj.value("Math3").toInt();
            group->mathChannel[3] = jsonobj.value("Math4").toInt();
        }
        index++;
    }

    int countMath = options["countMath"].toInt();
    if(countMath != 0)
    {
        listMath.clear();
        for(int i = 0; i < countMath; i ++)
        {
            cMathChannel *math = new cMathChannel();
            math->setNum(i);
            math->setName("Math " + QString::number(i+1));
            listMath.append(math);
        }
    }

    array = options["mathChannels"].toArray();
    index = 0;
    foreach (cMathChannel * math, listMath)
    {
        jsonobj = array.at(index++).toObject();
        math->setName(jsonobj.value("Name").toString().toUtf8());
        math->SetMathEquation(jsonobj.value("MathString").toString().toUtf8());
        math->SetHigherMeasureLimit(jsonobj.value("HigherMeasureLimit").toDouble());
        math->SetLowerMeasureLimit(jsonobj.value("LowerMeasureLimit").toDouble());
        math->numChannel[0] = jsonobj.value("X1").toInt();
        math->numChannel[1] = jsonobj.value("X2").toInt();
        math->numChannel[2] = jsonobj.value("X3").toInt();
        math->numChannel[3] = jsonobj.value("X4").toInt();
    }

    return 0;
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
    if(file.open(QIODevice::WriteOnly))
    {
        file.resize(0); // clear file
        out << setstr;
        file.close();
    }
    else
    {
        return 1;
    }

    return 0;
}
