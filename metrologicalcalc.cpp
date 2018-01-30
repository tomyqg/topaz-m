#include "metrologicalcalc.h"

MetrologicalCalc::MetrologicalCalc()
{
}

double MetrologicalCalc::ConvertSignalToValue(double current, double signallow, double signalhigh, double valuelow, double valuehigh)
{
    double tmp = current-signallow;
    tmp *= (valuehigh-valuelow);
    double tmp1 = signalhigh-signallow;
    if(tmp1 == 0) tmp = 0;
    tmp /= tmp1;
    tmp += valuelow;
    //return ( ( (current-signallow)*(valuehigh-valuelow)/(signalhigh-signallow) ) + valuelow );
    return tmp;
}
