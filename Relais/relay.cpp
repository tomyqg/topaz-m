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
        if(type) f = !f;    //инвертирование, если надо
        emit signalSwitch(mySlot, myPhysicalNum, f);
        confirmedState = false;
    }
}

void cRelay::setCurState(bool state)
{
    if(type) state = !state;//инвертирование, если надо
    curState = state;
    if(curState == newState) confirmedState = true;
    else confirmedState = false;
}
