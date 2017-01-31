#include "channel1.h"
#include "QDebug"

int ChannelOptions::count222 = 7;

int ChannelOptions::GetSignalType1()
{
    return 1;
}

int ChannelOptions::GetHigherLimit()
{
    return ChannelOptions::higherlimit;
}

int ChannelOptions::GetHigherMeasureLimit()
{
    return ChannelOptions::highermeasurelimit;
}

int ChannelOptions::GetLowerLimit()
{
    return ChannelOptions::lowerlimit;
}

int ChannelOptions::GetLowerMeasureLimit()
{
    return ChannelOptions::lowermeasurelimit;
}

int ChannelOptions::GetSignalType()
{
    return ChannelOptions::signaltype;
}

void ChannelOptions::SetSignalType(int newsignaltype)
{
    ChannelOptions::signaltype = newsignaltype;
}

void ChannelOptions::SetUnitsName(QString newunitname)
{
    unitsname = newunitname;
}

QString ChannelOptions::GetUnitsName()
{
    return ChannelOptions::unitsname;
}
