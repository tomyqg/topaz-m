#ifndef METROLOGICALCALC_H
#define METROLOGICALCALC_H


class MetrologicalCalc
{
public:
    MetrologicalCalc();

    double CurrentToPressure(double current, double currentlow,double currenthigh,double pressurelow,double pressurehigh);

};

#endif // METROLOGICALCALC_H
