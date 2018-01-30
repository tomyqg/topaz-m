#include <assert.h>
#include <QDebug>
#include <QTimer>
#include "slotsconfig.h"
#include "registermap.h"


cSlotsConfig::cSlotsConfig(QObject *parent) : QObject(parent)
{
    stateUpdConf = 0;
    countSlots = 0;
    timerUpdConfig = new QTimer(this);
    // реализовать тут первичную загрузку конфигурации из файла вместо updConfig()
//    updConfig();
    connect(timerUpdConfig, SIGNAL(timeout()), this, SLOT(updConfig()));
    timerUpdConfig->start(UpdateConfigTimer);
}

cSlotsConfig::~cSlotsConfig()
{

}

void cSlotsConfig::addSlot(int num, int type)
{
    bool isSet = false;
    foreach (Slot * sl, listSlots) {
        if(sl->getNum() == num)
        {
            sl->setType(type);
            isSet = true;
            break;
        }
    }
    if(!isSet)
    {
        Slot * s = new Slot(this);
        s->initSlot(num, type);
        listSlots.append(s);
    }
    countSlots = listSlots.size();
}

int cSlotsConfig::getCountSlots()
{
    countSlots = listSlots.size();
    return countSlots;
}

void cSlotsConfig::updConfig()
{
    Transaction tr(Transaction::R);
    switch(stateUpdConf)
    {
    case 0: // кинуть клич по все слоты
    {
        tr.dir = Transaction::R;
        tr.offset = RegisterMap::getOffsetByName("deviceType");
        for(int i = 0; i < TOTAL_NUM_SLOTS; i++)
        {
            tr.slave = i+1;
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig SIGNAL: slave" << tr.slave << ", offset" << tr.offset;
#endif
            emit sendRequest(tr);
        }
        stateUpdConf = 1;
        timerUpdConfig->setInterval(1000);
        break;
    }
    case 1: // ответа не было, значит все слоты в отключке
        stateUpdConf = 0;
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "Slots is offline";
#endif
        listSlots.clear();  //если слоты не ответили, значит их нет
        timerUpdConfig->setInterval(UpdateConfigTimer);
        break;
    case 2: // хоть один из слотов ответил за время посылки запросов
    {
        tr.dir = Transaction::R;
        tr.offset = RegisterMap::getOffsetByName("deviceStatus");
        for(int i = 0; i < TOTAL_NUM_SLOTS; i++)
        {
            if(isSlotSet(i+1))
            {
                tr.slave = i+1;
#ifdef DEBAG_SLOT_CONFIG
                qDebug() << "SlotConfig SIGNAL: slave" << tr.slave << ", offset" << tr.offset;
#endif
                emit sendRequest(tr);
            }
        }
        stateUpdConf = 3;
        timerUpdConfig->setInterval(500);
        break;
    }
    case 3:

        break;

    case 4:

        break;
    default:
        break;
    }
}

void cSlotsConfig::receiveConf(Transaction tr)
{
    Transaction trans = tr;
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "SlotConfig SLOT: slave" << trans.slave \
                 << ", offset" << trans.offset\
                 << ", vol" << trans.volInt << trans.volFlo;
#endif
    QString paramName = RegisterMap::getNameByOffset(trans.offset);
    if(paramName == "deviceType")
    {
        stateUpdConf = 2;
        addSlot(trans.slave, trans.volInt);
     }
    else if(paramName == "deviceStatus")
    {
        stateUpdConf = 4;
        setStatusDev(trans.slave, trans.volInt);
    }
    else
    {


    }
}

int cSlotsConfig::getTypeDevice(int slot)
{
    foreach (Slot * s, listSlots) {
        if(s->getNum() == slot)
        {
            return s->getType();
        }
    }
    return 0;
}

bool cSlotsConfig::isSlotSet(int slot)
{
    foreach (Slot * s, listSlots) {
        if(s->getNum() == slot)
        {
            return true;
        }
    }
    return false;
}

int cSlotsConfig::getStatusDev(int sl)
{
    foreach (Slot * s, listSlots) {
        if(s->getNum() == sl)
        {
            return s->getStatus();
        }
    }
    return 0;
}

void cSlotsConfig::setStatusDev(int sl, int st)
{
    foreach (Slot * s, listSlots) {
        if(s->getNum() == sl)
        {
            s->setStatus(st);
        }
    }
}
