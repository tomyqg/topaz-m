#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>

class ChannelOptions
{

public :  int count;

public:

    QString GetUnitsName();

    int GetSignalType();
    int GetLowerLimit();
    int GetHigherLimit();
    int GetLowerMeasureLimit();
    int GetHigherMeasureLimit();

    void SetSignalType(int newsignaltype);
    void SetLowerLimit(int newsignaltype);
    void SetHigherLimit(int newhigherlimit);
    void SetLowerMeasureLimit(int newlowermeaslimit);
    void SetHigherMeasureLimit(int newhighermeaslimit);
    void SetUnitsName(QString newunit);

    // приватные переменные настроек канала

private:

    int signaltype;
    int lowerlimit;
    int higherlimit;
    int lowermeasurelimit;
    int highermeasurelimit;
    int measureperiodsecond;
    QString unitsname;

};

#endif // CHANNEL1_H
