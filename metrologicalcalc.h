#ifndef METROLOGICALCALC_H
#define METROLOGICALCALC_H

#include <QObject>

class MetrologicalCalc :  public QObject
{
public:
    MetrologicalCalc();
    static double ConvertSignalToValue(double current, double signallow,double signalhigh,double valuelow,double valuehigh);
private:
};

#endif // METROLOGICALCALC_H
