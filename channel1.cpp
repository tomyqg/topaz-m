#include "channel1.h"
#include "QDebug"

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
