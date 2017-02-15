#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>

class ChannelOptions
{

public:

    QString GetUnitsName();

    double GetSignalType();
    double GetLowerLimit();
    double GetHigherLimit();
    double GetLowerMeasureLimit();
    double GetHigherMeasureLimit();

    void SetSignalType(double newsignaltype);
    void SetLowerLimit(double newsignaltype);
    void SetHigherLimit(double newhigherlimit);
    void SetLowerMeasureLimit(double newlowermeaslimit);
    void SetHigherMeasureLimit(double newhighermeaslimit);
    void SetUnitsName(QString newunit);

    // приватные переменные настроек канала

private:

    double signaltype;
    double lowerlimit;
    double higherlimit;
    double lowermeasurelimit;
    double highermeasurelimit;
    double measureperiodsecond;
    QString unitsname;
};

#endif // CHANNEL1_H
