#include "metrologicalcalc.h"

MetrologicalCalc::MetrologicalCalc()
{
}

double MetrologicalCalc::ConvertSignalToValue(double current, double signallow, double signalhigh, double valuelow, double valuehigh)
{
    double tmp = current-signallow;
    tmp *= (valuehigh-valuelow);
    tmp /= (signalhigh-signallow);
    tmp += valuelow;
    //return ( ( (current-signallow)*(valuehigh-valuelow)/(signalhigh-signallow) ) + valuelow );
    return tmp;
}
