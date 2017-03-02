#include "metrologicalcalc.h"

MetrologicalCalc::MetrologicalCalc()
{

}


double MetrologicalCalc::CurrentToPressure(double current, double currentlow, double currenthigh, double pressurelow, double pressurehigh)
{
    return ( ( (current-currentlow)*(pressurehigh-pressurelow)/(currenthigh-currentlow) ) + pressurelow );
}
