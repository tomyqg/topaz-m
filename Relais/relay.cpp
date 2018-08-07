#include "relay.h"

cRelay::cRelay(uint8_t numDev, uint8_t slot, QObject *parent):
      myPhysicalNum(numDev),
      mySlot(slot),
      QObject(parent)
{
    newState = false;
    oldState = false;
    confirmedState = false;
}
