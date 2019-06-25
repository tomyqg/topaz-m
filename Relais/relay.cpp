#include "relay.h"

cRelay::cRelay(uint8_t numDev, uint8_t slot, QObject *parent):
      myPhysicalNum(numDev),
      mySlot(slot),
      QObject(parent)
{
    enable = false;
    newState = false;
    confirmedState = false;
    curState = false;
    type = false;
    connect(&timerUpdate, SIGNAL(timeout()), this, SLOT(update()));
    timerUpdate.start(500);
}

void cRelay::setState(bool f)
{
    if((f != newState) && enable)
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

    if(curState != newState)
    {
        confirmedState = false;
        if(type) state = !state;
        emit signalSwitch(mySlot, myPhysicalNum, !state);
    }
    else
    {
        confirmedState = true;
    }
}

void cRelay::update()
{
    if(enable && !confirmedState)
    {
        emit signalGetState(mySlot, myPhysicalNum);
    }
}
