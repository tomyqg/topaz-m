#ifndef METROLOGICALCALC_H
#define METROLOGICALCALC_H

#include <QObject>

class MetrologicalCalc :  public QObject
{
public:
    MetrologicalCalc();
    static double ConvertSignalToValue(double current, double currentlow,double currenthigh,double valuelow,double valuehigh);
};

#endif // METROLOGICALCALC_H
