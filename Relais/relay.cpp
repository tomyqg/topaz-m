#include "relay.h"

cRelay::cRelay(uint8_t numDev, uint8_t slot, QObject *parent):
      myPhysicalNum(numDev),
      mySlot(slot),
      QObject(parent)
{
    newState = false;
    oldState = false;
    confirmedState = true;
    curState = false;
}

void cRelay::setState(bool f)
{
    if(f != newState)
    {
        newState = f;
        emit signalSwitch(mySlot, myPhysicalNum, newState);
        confirmedState = false;
    }
}

void cRelay::setCurState(bool state)
{
    curState = state;
    if(curState == newState) confirmedState = true;
    else confirmedState = false;
}
