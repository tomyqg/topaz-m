#include <assert.h>
#include <QDebug>
#include <QTimer>
#include "slotsconfig.h"
#include "../registermap.h"


cSlotsConfig::cSlotsConfig(QObject *parent) : QObject(parent)
{
    stateUpdConf = 0;
    countSlots = 0;
    curSlot = 1;
    timerUpdConfig = new QTimer(this);
    // реализовать тут первичную загрузку конфигурации из файла вместо updConfig()
//    updConfig();
    connect(timerUpdConfig, SIGNAL(timeout()), this, SLOT(updConfig()));
    timerUpdConfig->start(UpdateConfigTimer);
}

cSlotsConfig::~cSlotsConfig()
{
//    foreach (Slot * s, listSlots) {
//        delete s;
//    }
    for(int i = 0; i < listSlots.size(); i++)
    {
        listSlots.at(i)->deleteLater();
//        listSlots.erase();
    }
}

int cSlotsConfig::addSlot(int num, int type)
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
    return countSlots;
}

int cSlotsConfig::getCountSlots()
{
    countSlots = listSlots.size();
    return countSlots;
}

void cSlotsConfig::updConfig()
{
    int updateInterval = UpdateConfigTimer;
    Transaction tr(Transaction::R);
    tr.dir = Transaction::R;
    tr.offset = RegisterMap::getOffsetByName("deviceType");
    if(stateUpdConf != 0)
    {
        // конфигурация слота не была завершена за выделеное время
        // - проблемы с платой.
        // Принимаем, что она оффлайн, если она вообще есть
        if(isSlotSet(curSlot)) updOnlineStatus(curSlot, false);
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "Slot" << curSlot << "is offline";
#endif
        if(curSlot >= MAX_NUM_SLOTS)
        {
            curSlot = 1;
            stateUpdConf = 0;
            timerUpdConfig->setInterval(PeriodReconfig);
            return;
        }
        else curSlot++;
    }
    tr.slave = curSlot;
    emit sendRequest(tr);
    stateUpdConf = 1;
    timerUpdConfig->setInterval(updateInterval);
}

void cSlotsConfig::receiveConf(Transaction tr)
{
    Transaction trans = tr;
    QString paramName = RegisterMap::getNameByOffset(trans.offset);
    if((paramName == "deviceType") && (curSlot == trans.slave))
    {
        int count = addSlot(trans.slave, trans.volInt);
        trans.offset = RegisterMap::getOffsetByName("deviceStatus");
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "SlotConfig SLOT: slave" << trans.slave \
                 << "deviceType" << trans.offset << trans.volInt;
#endif
        emit sendRequest(trans);
        timerUpdConfig->setInterval(UpdateConfigTimer);
    } else if((paramName == "deviceStatus") && (curSlot == trans.slave)) {
        setStatusDev(trans.slave, trans.volInt);
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "SlotConfig SLOT:" \
                 << trans.offset << trans.volInt << trans.volFlo;
#endif
        if(getTypeDevice(curSlot) == 1) // 4AI - плата аналоговых входов
        {
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig SIGNAL: slave" << trans.slave << " get channel 1 configs" ;
#endif
            // статусы каналов
            trans.offset = RegisterMap::getOffsetByName("chan0Status");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // поддерживаемые типы сигналов
            trans.offset = RegisterMap::getOffsetByName("chan0SupportedSignals");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // установленный тип сигнала
            trans.offset = RegisterMap::getOffsetByName("chan0SignalType");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // конфигурация измерительного канала
            trans.offset = RegisterMap::getOffsetByName("chan0AdditionalParameter1");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan0AdditionalParameter2");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // Измеренная температура холодного спая
            trans.offset = RegisterMap::getOffsetByName("chan0CjValue");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);

            // берём больший запас времени для получения настроек каналов
            timerUpdConfig->setInterval(TimeReadChannelConf);
        }
    }
    else if((paramName == "chan0CjValue") && (curSlot == trans.slave))
    {
        if(getTypeDevice(curSlot) == 1) // 4AI - плата аналоговых входов
        {
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "SlotConfig SLOT:" \
                 << trans.offset << trans.volInt << trans.volFlo;
#endif
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig SIGNAL: slave" << trans.slave << " get channel 2 configs" ;
#endif
            trans.offset = RegisterMap::getOffsetByName("chan1Status");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1SupportedSignals");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1SignalType");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1AdditionalParameter1");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1AdditionalParameter2");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1CjValue");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            timerUpdConfig->setInterval(TimeReadChannelConf);
        }
    }
    else if((paramName == "chan1CjValue") && (curSlot == trans.slave))
    {
        if(getTypeDevice(curSlot) == 1) // 4AI - плата аналоговых входов
        {
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "SlotConfig SLOT:" \
                 << trans.offset << trans.volInt << trans.volFlo;
#endif
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig SIGNAL: slave" << trans.slave << " get channel 3 configs" ;
#endif
            trans.offset = RegisterMap::getOffsetByName("chan2Status");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2SupportedSignals");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2SignalType");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2AdditionalParameter1");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2AdditionalParameter2");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2CjValue");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            timerUpdConfig->setInterval(TimeReadChannelConf);
        }
    }
    else if((paramName == "chan2CjValue") && (curSlot == trans.slave))
    {
        if(getTypeDevice(curSlot) == 1) // 4AI - плата аналоговых входов
        {
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig SLOT:" \
                     << trans.offset << trans.volInt << trans.volFlo;
#endif
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig SIGNAL: slave" << trans.slave << " get channel 4 configs" ;
#endif
            trans.offset = RegisterMap::getOffsetByName("chan3Status");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3SupportedSignals");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3SignalType");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3AdditionalParameter1");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3AdditionalParameter2");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3CjValue");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            timerUpdConfig->setInterval(TimeReadChannelConf);
        }
    }
    else if((paramName == "chan3CjValue") && (curSlot == trans.slave))
    {
        if(getTypeDevice(curSlot) == 1) // 4AI - плата аналоговых входов
        {
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig SLOT:" \
                     << trans.offset << trans.volInt << trans.volFlo;
#endif
            // запрос температуры и напряжения на плате на последок
            trans.offset = RegisterMap::getOffsetByName("onBoardTemp");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("onBoardVoltage");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            timerUpdConfig->setInterval(TimeReadChannelConf);
        }
    }
    else if((paramName == "onBoardVoltage") && (curSlot == trans.slave))
    {
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "SlotConfig SLOT:" \
                 << trans.offset << trans.volInt << trans.volFlo;
#endif
        // конфигурация слота завершена, переходим на следующий
        stateUpdConf = 0;
        endConfSlot(curSlot);
        if(curSlot >= TOTAL_NUM_SLOTS)
        {
            curSlot = 1;
            // после сканирования всех слотов конфигуратор отдыхает [10] секунд
            // затем снова опрос, на случай если какой-то слот отвалился/заменили/завис
            timerUpdConfig->setInterval(PeriodReconfig);
        }
        else curSlot++;
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig Next slot" << curSlot;
#endif
    } else if((curSlot == trans.slave) && (stateUpdConf == 1))
    {
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "SlotConfig SLOT:" \
                 << trans.offset << trans.volInt << trans.volFlo;
#endif
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
            s->updOnline(true);
        }
    }
}

void cSlotsConfig::updOnlineStatus(int sl, bool st)
{
    foreach (Slot * s, listSlots) {
        if(s->getNum() == sl)
        {
            s->updOnline(st);
        }
    }
}

void cSlotsConfig::endConfSlot(int sl)
{
    foreach (Slot * s, listSlots) {
        if(s->getNum() == sl)
        {
            s->resetNeedConfig();
        }
    }
}
