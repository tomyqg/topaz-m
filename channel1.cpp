#include "channel1.h"
#include "QDebug"

//QString ChannelOptions::unitsname = "options1 name";
int ChannelOptions::count222 = 7;
//int ChannelOptions::higherlimit = 20;
//int ChannelOptions::lowerlimit= 4;
//int ChannelOptions::highermeasurelimit = 100;
//int ChannelOptions::lowermeasurelimit= 0;
//int ChannelOptions::signaltype= 0;

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
