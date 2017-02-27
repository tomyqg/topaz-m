#include "channel1.h"
#include "QDebug"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

double ChannelOptions::GetHigherLimit()
{
    return this->higherlimit;
}

double ChannelOptions::GetHigherMeasureLimit()
{
    return this->highermeasurelimit;
}

double ChannelOptions::GetLowerLimit()
{
    return this->lowerlimit;
}

double ChannelOptions::GetLowerMeasureLimit()
{
    return this->lowermeasurelimit;
}

double ChannelOptions::GetMeasurePeriod()
{
    return this->measureperiod;
}

double ChannelOptions::GetSignalType()
{
    return this->signaltype;
}

void ChannelOptions::SetSignalType(double newsignaltype)
{
    this->signaltype = newsignaltype;
}

void ChannelOptions::SetUnitsName(QString unitsname)
{
    this->unitsname = unitsname;
}

void ChannelOptions::SetMeasurePeriod(double np)
{
    this->measureperiod = np;
}

QString ChannelOptions::GetUnitsName()
{
    return this->unitsname;
}

void ChannelOptions::SetHigherLimit(double newlimit)
{
    this->higherlimit=newlimit;
}

void ChannelOptions::SetLowerLimit(double newlimit)
{
    this->lowerlimit=newlimit;
}

void ChannelOptions::SetHigherMeasureLimit(double newlimit)
{
    this->highermeasurelimit=newlimit;
}

void ChannelOptions::SetLowerMeasureLimit(double newlimit)
{
    this->lowermeasurelimit=newlimit;
}

QString ChannelOptions::GetState1HighMessage()
{
    return this->state1highmessage;
}

QString ChannelOptions::GetState1LowMessage()
{
    return this->state1lowmessage;
}

QString ChannelOptions::GetState2HighMessage()
{
    return this->state2highmessage;
}

QString ChannelOptions::GetState2LowMessage()
{
    return this->state2lowmessage;
}

void ChannelOptions::SetState1HighMessage(QString newmessage)
{
    this->state1highmessage = newmessage;
}

void ChannelOptions::SetState1LowMessage(QString newmessage)
{
    this->state1lowmessage = newmessage;
}

void ChannelOptions::SetState2HighMessage(QString newmessage)
{
    this->state2highmessage = newmessage;
}

void ChannelOptions::SetState2LowMessage(QString newmessage)
{
    this->state2lowmessage = newmessage;
}

void ChannelOptions::SetState1Value(double value)
{
    this->state1value = value;
}

void ChannelOptions::SetState2Value(double value)
{
    this->state2value = value;
}

double ChannelOptions::GetState1Value()
{
    return this->state1value;
}

double ChannelOptions::GetState2Value()
{
    return this->state2value;
}
