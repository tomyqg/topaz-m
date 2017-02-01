#include "channel1.h"
#include "QDebug"

int ChannelOptions::GetHigherLimit()
{
    return this->higherlimit;
}

int ChannelOptions::GetHigherMeasureLimit()
{
    return this->highermeasurelimit;
}

int ChannelOptions::GetLowerLimit()
{
    return this->lowerlimit;
}

int ChannelOptions::GetLowerMeasureLimit()
{
    return this->lowermeasurelimit;
}

int ChannelOptions::GetSignalType()
{
    return this->signaltype;
}

void ChannelOptions::SetSignalType(int newsignaltype)
{
    this->signaltype = newsignaltype;
}

void ChannelOptions::SetUnitsName(QString unitsname)
{
    this->unitsname = unitsname;
}

QString ChannelOptions::GetUnitsName()
{
    return this->unitsname;
}
