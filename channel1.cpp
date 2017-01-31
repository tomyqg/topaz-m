#include "channel1.h"
#include "QDebug"

QString Channel1Options::unitsname = "options1 name";
int Channel1Options::count222 = 7;
int Channel1Options::higherlimit = 20;
int Channel1Options::lowerlimit= 4;
int Channel1Options::highermeasurelimit = 100;
int Channel1Options::lowermeasurelimit= 0;
int Channel1Options::signaltype= 0;

int Channel1Options::GetSignalType1()
{
    return 1;
}

int Channel1Options::Ret()
{
    return Channel1Options::count222;
}

QString Channel1Options::GetUnitsName()
{
    return Channel1Options::unitsname;
}

int Channel1Options::GetHigherLimit()
{
    return Channel1Options::higherlimit;
}

int Channel1Options::GetHigherMeasureLimit()
{
    return Channel1Options::highermeasurelimit;
}

int Channel1Options::GetLowerLimit()
{
    return Channel1Options::lowerlimit;
}

int Channel1Options::GetLowerMeasureLimit()
{
    return Channel1Options::lowermeasurelimit;
}

int Channel1Options::GetSignalType()
{
    return Channel1Options::signaltype;
}

void Channel1Options::SetSignalType(int newsignaltype)
{
    Channel1Options::signaltype = newsignaltype;
}

void Channel1Options::SetUnitsName(QString newunitname)
{
    unitsname = newunitname;
//    qDebug() << "SetUnitsName";
}

void Channel1Options::Protecfun()
{
    qDebug() << "Protected funct";
}
