#include "steel_connect.h"

cSteelConnect::cSteelConnect(int userS, int devS, int slot, QObject *parent) : QObject(parent)
{
    userSteel = userS;
    deviceSteel = devS;
    numSlot = slot;
}
