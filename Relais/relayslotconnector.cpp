#include "relayslotconnector.h"

cRelaySlotConnector::cRelaySlotConnector(int userR, int devR, int slot, QObject *parent)
    : QObject(parent)
{
    userRelay = userR;
    deviceRelay = devR;
    numSlot = slot;
}
