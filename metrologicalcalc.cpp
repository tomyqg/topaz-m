#include "metrologicalcalc.h"

MetrologicalCalc::MetrologicalCalc()
{
}

double MetrologicalCalc::ConvertCurrentToPressure(double current, double currentlow, double currenthigh, double pressurelow, double pressurehigh)
{
    return ( ( (current-currentlow)*(pressurehigh-pressurelow)/(currenthigh-currentlow) ) + pressurelow );
}
