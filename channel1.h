#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>


class Channel1Options
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
    int Ret();
    int thetemp  = 5;

    void SetSignalType(int newsignaltype);
    void SetLowerLimit(int newsignaltype);
    void SetHigherLimit(int newhigherlimit);
    void SetLowerMeasureLimit(int newlowermeaslimit);
    void SetHigherMeasureLimit(int newhighermeaslimit);
    void SetUnitsName(QString newunit);

    // приватные переменные настроек канала 1

private:

    static int signaltype;
    static int lowerlimit;
    static int higherlimit;
    static int lowermeasurelimit;
    static int highermeasurelimit;
    static int measureperiodsecond;
    static QString unitsname;

protected:

    void Protecfun();

};

class Channel2Options : public Channel1Options
{

    // приватные переменные настроек канала 1

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
