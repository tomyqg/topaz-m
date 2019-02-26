#include "filemanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <assert.h>
#include <steel.h>
#include <defines.h>
#include "Channels/group_channels.h"

extern QList<ChannelOptions *> listChannels;
extern QList<cGroupChannels*> listGroup;
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
        if(Channel->enable)
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
            channel->SetDiapason(ch.value("Diapason").toInt());
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
    QJsonObject options, groupsjsonobj;
    QJsonArray settingsGroup;
//    systemoptions["Time"] = GetNewTimeString();
//    systemoptions["Date"] = GetNewDateString();
//    systemoptions["Display"] = GetCurrentDisplayParametr();
//    systemoptions["Calibration"] = GetCalibration();
//    systemoptions["Resolution"] = GetNewDisplayResolution();
    systemoptions["Arrows"] = opt->arrows;
    systemoptions["Display"] = opt->display;
    systemoptions["Autoscale"] = opt->autoscale;
    systemoptions["Brightness"] = opt->brightness;

    foreach (cGroupChannels * group, listGroup) {
        groupsjsonobj["Enabled"] = group->enabled;
        groupsjsonobj["Name"] = group->groupName;
//        groupsjsonobj["Channel1"] = -1;
//        groupsjsonobj["Channel2"] = -1;
//        groupsjsonobj["Channel3"] = -1;
//        groupsjsonobj["Channel4"] = -1;
        groupsjsonobj["Channel1"] = group->channel[0];
        groupsjsonobj["Channel2"] = group->channel[1];
        groupsjsonobj["Channel3"] = group->channel[2];
        groupsjsonobj["Channel4"] = group->channel[3];

        groupsjsonobj["Math1"] = group->mathChannel[0];
        groupsjsonobj["Math2"] = group->mathChannel[1];
        groupsjsonobj["Math3"] = group->mathChannel[2];
        groupsjsonobj["Math4"] = group->mathChannel[3];
//        for(int i = 0; i < listChannels.size(); i++)
//        {
//            if(group->channel[0] == listChannels.at(i))
//                groupsjsonobj["Channel1"] = i;
//            if(group->channel[1] == listChannels.at(i))
//                groupsjsonobj["Channel2"] = i;
//            if(group->channel[2] == listChannels.at(i))
//                groupsjsonobj["Channel3"] = i;
//            if(group->channel[3] == listChannels.at(i))
//                groupsjsonobj["Channel4"] = i;
//        }

//        for(int i = 0; i < listMath.size(); i++)
//        {
//            if(group->mathChannel[0] == listMath.at(i))
//                groupsjsonobj["Math1"] = i;
//            if(group->mathChannel[1] == listMath.at(i))
//                groupsjsonobj["Math2"] = i;
//            if(group->mathChannel[2] == listMath.at(i))
//                groupsjsonobj["Math3"] = i;
//            if(group->mathChannel[3] == listMath.at(i))
//                groupsjsonobj["Math4"] = i;
//        }
        groupsjsonobj["Type1"] = group->typeInput[0];
        groupsjsonobj["Type2"] = group->typeInput[1];
        groupsjsonobj["Type3"] = group->typeInput[2];
        groupsjsonobj["Type4"] = group->typeInput[3];
        settingsGroup.append(groupsjsonobj);
    }

    options["countGrp"] = listGroup.length();
    options["groups"] = settingsGroup;
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

    QJsonObject options = json["Options"].toObject();
    int countGroup = options["countGrp"].toInt();
    QJsonArray array = options["groups"].toArray();
    QJsonObject jsonobj;
    int index = 0;
    int sizeCh = listChannels.size();

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

//            int numChannel[MAX_NUM_CHAN_GROUP];
//            int numMath[MAX_NUM_CHAN_GROUP];
//            numChannel[0] = jsonobj.value("Channel1").toInt();
//            numChannel[1] = jsonobj.value("Channel2").toInt();
//            numChannel[2] = jsonobj.value("Channel3").toInt();
//            numChannel[3] = jsonobj.value("Channel4").toInt();
//            numMath[0] = jsonobj.value("Math1").toInt();
//            numMath[1] = jsonobj.value("Math2").toInt();
//            numMath[2] = jsonobj.value("Math3").toInt();
//            numMath[3] = jsonobj.value("Math4").toInt();
//            for(int i = 0; i < MAX_NUM_CHAN_GROUP; i++)
//            {
//                if((group->typeInput[i] == 1) && (numChannel[i] != -1))
//                {
//                    if(sizeCh > numChannel[i])
//                    {
//                        group->channel[i] = listChannels.at(numChannel[i]);
////                        if(!group->channel[i]->enable)
////                        {
////                            group->typeInput[i] == 0;
////                        }
//                    }
//                    else
//                    {
//                        //такое не может быть, но перестраховаться надо
//                        group->channel[i] = nullptr;
//                        group->typeInput[i] = 0;
//                    }
//                }
//                //                if((group->typeInput[i] == 2) && (numMath[i] != -1))
//                //                {
//                //                    if(sizeMath > numMath[i])
//                //                    {
//                //                        group->mathChannel[i] = listMath.at(numMath[i]);
//                //                    }
//                //                }
//            }
//            if((group->typeInput[0] == 0) &&  (group->typeInput[1] == 0) && \
//                    (group->typeInput[2] == 0) &&  (group->typeInput[3] == 0))
//            {
//                group->enabled = false;
//            }
        }
        index++;
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
