#include "metrologicalcalc.h"

MetrologicalCalc::MetrologicalCalc()
{
}

double MetrologicalCalc::ConvertSignalToValue(double current, double signallow, double signalhigh, double valuelow, double valuehigh)
{
    return ( ( (current-signallow)*(valuehigh-valuelow)/(signalhigh-signallow) ) + valuelow );
}
