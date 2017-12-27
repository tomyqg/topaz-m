#include "ustavka.h"



void Ustavka::setUstavka(double val, double hist, int relayUp, int relayDown)
{
    stateValue = val;
    histeresis = hist;
    numRelayUp = relayUp;
    numRelayDown = relayDown;
    setUp = false;
    setDown = false;
//    isSetted = false;
//    timeFilter.stop();
}

int Ustavka::getUstavka(double cur)
{
    if((cur > (stateValue + histeresis)))
    {
        setUp = true;
        setDown = false;
        return numRelayUp;
    }
    else if((cur < (stateValue - histeresis)))
    {
        setUp = false;
        setDown = true;
        return numRelayDown;
    }
    else
    {
        setUp = false;
        setDown = false;
        return 0;
    }
}

double Ustavka::getStateValue()
{
    return stateValue;
}

double Ustavka::getHisteresis()
{
    return histeresis;
}

int Ustavka::getnumRelayUp()
{
    return numRelayUp;
}

int Ustavka::getnumRelayDown()
{
    return numRelayDown;
}
