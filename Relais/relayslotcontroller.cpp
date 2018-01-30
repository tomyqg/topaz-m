#include "relayslotcontroller.h"

cRelaySlotController::cRelaySlotController(QObject *parent) : QObject(parent)
{

}

int cRelaySlotController::addRelaySlot(int userR, int devR, int slot)
{
    bool findR = false;

    // поиск ранее установленной связки реле-слот
    foreach (cRelaySlotConnector * r, listRS)
    {
        if(r->userRelay == userR)
        {
            r->deviceRelay = devR;
            r->numSlot;
            findR = true;
        }
    }

    // связка устанавливается вновь
    if(!findR)
    {
        cRelaySlotConnector * rs = new cRelaySlotConnector(userR, devR, slot);
        listRS.append(rs);
    }
}

int cRelaySlotController::getDevRelay(int userR)
{
    int res = -1;
    foreach (cRelaySlotConnector * rs, listRS) {
        if(rs->userRelay == userR)
        {
            res = rs->deviceRelay;
        }
    }
    return res;
}

int cRelaySlotController::getSlotByRelay(int userR)
{
    int res = -1;
    foreach (cRelaySlotConnector * rs, listRS) {
        if(rs->userRelay == userR)
        {
            res = rs->numSlot;
        }
    }
    return res;
}
