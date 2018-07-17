#include "steel_controller.h"

cSteelController::cSteelController(QObject *parent) : QObject(parent)
{

}


cSteelController::~cSteelController()
{
//    foreach (cRelaySlotConnector * rs, listRS) {
//        delete rs;
//    }
    for(int i = 0; i < listSt.size(); i++)
    {
        listSt.at(i)->deleteLater();
    }
}

int cSteelController::addSteelSlot(int userS, int devS, int slot)
{
    bool findS = false;

    // поиск ранее установленной связки реле-слот
    foreach (cSteelConnect * s, listSt)
    {
        if(s->userSteel == userS)
        {
            s->deviceSteel = devS;
            s->numSlot;
            findS = true;
        }
    }

    // связка устанавливается вновь
    if(!findS)
    {
        cSteelConnect * s = new cSteelConnect(userS, devS, slot);
        listSt.append(s);
    }
}

int cSteelController::getDevSteel(int userS)
{
    int res = -1;
    foreach (cSteelConnect * s, listSt) {
        if(s->userSteel == userS)
        {
            res = s->deviceSteel;
        }
    }
    return res;
}

int cSteelController::getSlotBySteel(int userS)
{
    int res = -1;
    foreach (cSteelConnect * s, listSt) {
        if(s->userSteel == userS)
        {
            res = s->numSlot;
        }
    }
    return res;
}

bool cSteelController::isConnect()
{
    return (listSt.size() > 0);
}
