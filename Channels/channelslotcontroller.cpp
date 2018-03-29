#include <QVector>
#include "channelslotcontroller.h"


cChannelSlotController::cChannelSlotController(QObject *parent) : QObject(parent)
{

}

cChannelSlotController::~cChannelSlotController()
{
    foreach (cChannelSlotConnector * cs, listCS) {
        delete cs;
    }
}

/*
 * Функция добавляет новую связку канал-слот или,
 *  если связка ранее устанвлена, обновляет параметры
 */
int cChannelSlotController::addChannelSlot(int userCh, int devCh, int slot)
{
    bool findCh = false;

    // поиск ранее установленной связки канал-слот
    foreach (cChannelSlotConnector * c, listCS)
    {
        if(c->userChannel == userCh)
        {
            c->deviceChannel = devCh;
            c->numSlot = slot;
            findCh = true;
        }
    }

    // связка устанавливается вновь
    if(!findCh)
    {
        cChannelSlotConnector * cs = new cChannelSlotConnector(userCh, devCh, slot);
        listCS.append(cs);
    }
}

int cChannelSlotController::getDevChannel(int userCh)
{
    int res = -1;
    foreach (cChannelSlotConnector * cs, listCS) {
        if(cs->userChannel == userCh)
        {
            res = cs->deviceChannel;
        }
    }
    return res;
}

int cChannelSlotController::getSlotByChannel(int userCh)
{
    int res = -1;
    foreach (cChannelSlotConnector * cs, listCS) {
        if(cs->userChannel == userCh)
        {
            res = cs->numSlot;
        }
    }
    return res;
}
