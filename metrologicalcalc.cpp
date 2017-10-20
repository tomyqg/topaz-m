#include "metrologicalcalc.h"

MetrologicalCalc::MetrologicalCalc()
{
}

double MetrologicalCalc::ConvertCurrentToValue(double current, double currentlow, double currenthigh, double valuelow, double valuehigh)
{
    return ( ( (current-currentlow)*(pressurehigh-pressurelow)/(currenthigh-currentlow) ) + pressurelow );
}
