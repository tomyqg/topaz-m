#include "channelslotconnector.h"
//#include "assert.h"

cChannelSlotConnector::cChannelSlotConnector(int userCh, int devCh, int slot, QObject *parent)
    : QObject(parent)
{
    userChannel = userCh;
    deviceChannel = devCh;
    numSlot = slot;
}

//void cChannelSlotConnector::addChannelSlot(int userCh, int devCh, int slot)
//{
//    userChannel = userCh;
//    deviceChannel = devCh;
//    numSlot = slot;
//}
