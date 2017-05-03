#ifndef METROLOGICALCALC_H
#define METROLOGICALCALC_H

#include <QObject>

class MetrologicalCalc :  public QObject
{
public:
    MetrologicalCalc();
    double ConvertCurrentToPressure(double current, double currentlow,double currenthigh,double pressurelow,double pressurehigh);
};

#endif // METROLOGICALCALC_H
