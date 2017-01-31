#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>


class ChannelOptions
{
public:
    static int count222;

public :  int count;

public:

    QString GetUnitsName();

    int GetSignalType1();
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

    // приватные переменные настроек канала 1

private:

    int signaltype;
    int lowerlimit;
    int higherlimit;
    int lowermeasurelimit;
    int highermeasurelimit;
    int measureperiodsecond;
    QString unitsname;

//    static int signaltype;
//    static int lowerlimit;
//    static int higherlimit;
//    static int lowermeasurelimit;
//    static int highermeasurelimit;
//    static int measureperiodsecond;
//    static QString unitsname;
};

class Channel2Options : public ChannelOptions
{

//  переменные настроек канала 2

private:

    static int signaltype;
    static int lowerlimit;
    static int higherlimit;
    static int lowermeasurelimit;
    static int highermeasurelimit;
    static int measureperiodsecond;
    static QString unitsname;

};

#endif // CHANNEL1_H
