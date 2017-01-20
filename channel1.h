#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>
//#include <options.h>
#include <QDialog>
//#include <mainwindow.h>



class Channel1Options
{
public:
    static int count222;

public :  int count;


public:

    int GetSignalType1();
    int GetSignalType();
    int GetLowerLimit();
    int GetHigherLimit();
    int GetLowerMeasureLimit();
    int GetHigherMeasureLimit();
    QString GetUnitsName();

    void SetSignalType(int newsignaltype);
    void SetLowerLimit(int newsignaltype);
    void SetHigherLimit(int newhigherlimit);
    void SetLowerMeasureLimit(int newlowermeaslimit);
    void SetHigherMeasureLimit(int newhighermeaslimit);
    void SetUnitsName(QString newunit);
    int Ret();

public:

    void setprop();
    void setprop2();

private:

    int type1=949;

    // переменные настроек прибора

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
