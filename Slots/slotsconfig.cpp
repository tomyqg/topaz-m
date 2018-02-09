#include <assert.h>
#include <QDebug>
#include <QTimer>
#include "slotsconfig.h"
#include "registermap.h"


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
        if(curSlot >= MAX_NUM_SLOTS) curSlot = 1;
        else curSlot++;
    }
    tr.slave = curSlot;
    emit sendRequest(tr);
    stateUpdConf = 1;
    timerUpdConfig->setInterval(UpdateConfigTimer);


//    switch(stateUpdConf)
//    {
//    case 0: // кинуть клич по все слоты
//    {
//        tr.dir = Transaction::R;
//        tr.offset = RegisterMap::getOffsetByName("deviceType");
//        for(int i = 0; i < TOTAL_NUM_SLOTS; i++)
//        {
//            tr.slave = i+1;
//#ifdef DEBAG_SLOT_CONFIG
//            qDebug() << "SlotConfig SIGNAL: slave" << tr.slave \
//                     << "deviceType" << tr.offset;
//#endif
//            emit sendRequest(tr);
//        }
//        stateUpdConf = 1;
//        timerUpdConfig->setInterval(1000);
//        break;
//    }
//    case 1: // ответа не было, значит все слоты в отключке
//        stateUpdConf = 0;
//#ifdef DEBAG_SLOT_CONFIG
//        qDebug() << "Slots is offline";
//#endif
//        listSlots.clear();  //если слоты не ответили, значит их нет
//        timerUpdConfig->setInterval(UpdateConfigTimer);
//        break;
//    case 2: // хоть один из слэйвов ответил за время посылки запросов
//    {
//        tr.dir = Transaction::R;
//        tr.offset = RegisterMap::getOffsetByName("deviceStatus");
//        for(int i = 0; i < TOTAL_NUM_SLOTS; i++)
//        {
//            if(isSlotSet(i+1))
//            {
//                tr.slave = i+1;
//#ifdef DEBAG_SLOT_CONFIG
//                qDebug() << "SlotConfig SIGNAL: slave" << tr.slave << ", offset" << tr.offset;
//#endif
//                emit sendRequest(tr);
//            }
//        }
//        stateUpdConf = 3;
//        timerUpdConfig->setInterval(50*listSlots.size());
//        break;
//    }
//    case 3: // ни один слот не ответил о своём состоянии - очень странно.
//        stateUpdConf = 0;
//        // начнём всё с начала
//        timerUpdConfig->setInterval(UpdateConfigTimer);
//        break;
//    case 4: // есть слоты, готовые к работе - продолжаем диалог только с ними пока
//        // запросить конфигурации всех каналов - с ними разберётся дальше основной поток
//        tr.dir = Transaction::R;
//        foreach (Slot * s, listSlots)
//        {
//            if(s->isOnline() && (s->getType() == 1))
//                //в слоте есть плата и её тип 4AI, тогда запрос всех настроек каналов
//            {
//                tr.slave = s->getNum();
//#ifdef DEBAG_SLOT_CONFIG
//                qDebug() << "SlotConfig SIGNAL: slave" << tr.slave << " get channel configs" ;
//#endif
//                // статусы каналов
//                tr.offset = RegisterMap::getOffsetByName("chan0Status");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan1Status");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan2Status");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan3Status");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                // поддерживаемые типы сигналов
//                tr.offset = RegisterMap::getOffsetByName("chan0SupportedSignals");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan1SupportedSignals");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan2SupportedSignals");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan3SupportedSignals");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                // установленный тип сигнала
//                tr.offset = RegisterMap::getOffsetByName("chan0SignalType");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan1SignalType");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan2SignalType");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan3SignalType");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                // конфигурация измерительного канала
//                tr.offset = RegisterMap::getOffsetByName("chan0AdditionalParameter1");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan1AdditionalParameter1");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan2AdditionalParameter1");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan3AdditionalParameter1");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan0AdditionalParameter2");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan1AdditionalParameter2");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan2AdditionalParameter2");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//                tr.offset = RegisterMap::getOffsetByName("chan3AdditionalParameter2");
//                if(tr.offset != 0xFFFF) emit sendRequest(tr);
//            }
//        }
//        stateUpdConf = 5;   // Будем ждать всей конфигурации
//        timerUpdConfig->setInterval(UpdateConfigTimer);
//        break;
//    case 5:

//        break;
//    default:
//        stateUpdConf = 0;
//        timerUpdConfig->setInterval(UpdateConfigTimer);
//        break;
//    }
}

void cSlotsConfig::receiveConf(Transaction tr)
{
    Transaction trans = tr;
    QString paramName = RegisterMap::getNameByOffset(trans.offset);
    if((paramName == "deviceType") && (curSlot = trans.slave))
    {
        int count = addSlot(trans.slave, trans.volInt);
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "countSlots = " << count;
#endif
        trans.offset = RegisterMap::getOffsetByName("deviceStatus");
#ifdef DEBAG_SLOT_CONFIG
        qDebug() << "SlotConfig SIGNAL: slave" << trans.slave << ", offset" << trans.offset;
#endif
        emit sendRequest(trans);
        timerUpdConfig->setInterval(UpdateConfigTimer);
    } else if((paramName == "deviceStatus") && (curSlot = trans.slave)) {
        setStatusDev(trans.slave, trans.volInt);
        if(getTypeDevice(curSlot) == 1) // 4AI - плата аналоговых входов
        {
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig SIGNAL: slave" << trans.slave << " get channel configs" ;
#endif
            // статусы каналов
            trans.offset = RegisterMap::getOffsetByName("chan0Status");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1Status");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2Status");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3Status");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // поддерживаемые типы сигналов
            trans.offset = RegisterMap::getOffsetByName("chan0SupportedSignals");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1SupportedSignals");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2SupportedSignals");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3SupportedSignals");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // установленный тип сигнала
            trans.offset = RegisterMap::getOffsetByName("chan0SignalType");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1SignalType");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2SignalType");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3SignalType");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // конфигурация измерительного канала
            trans.offset = RegisterMap::getOffsetByName("chan0AdditionalParameter1");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan1AdditionalParameter1");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan2AdditionalParameter1");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("chan3AdditionalParameter1");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
//            trans.offset = RegisterMap::getOffsetByName("chan0AdditionalParameter2");
//            if(trans.offset != 0xFFFF) emit sendRequest(trans);
//            trans.offset = RegisterMap::getOffsetByName("chan1AdditionalParameter2");
//            if(trans.offset != 0xFFFF) emit sendRequest(trans);
//            trans.offset = RegisterMap::getOffsetByName("chan2AdditionalParameter2");
//            if(trans.offset != 0xFFFF) emit sendRequest(trans);
//            trans.offset = RegisterMap::getOffsetByName("chan3AdditionalParameter2");
//            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // запрос температуры и напряжения на плате на последок
            trans.offset = RegisterMap::getOffsetByName("onBoardTemp");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            trans.offset = RegisterMap::getOffsetByName("onBoardVoltage");
            if(trans.offset != 0xFFFF) emit sendRequest(trans);
            // берём больший запас времени для получения настроек каналов
            timerUpdConfig->setInterval(UpdateConfigTimer*2);
        }
    } else if((paramName == "onBoardVoltage") && (curSlot = trans.slave))
    {
        // конфигурация слота завершена, переходим на следующий
        stateUpdConf = 0;
        if(curSlot >= TOTAL_NUM_SLOTS) curSlot = 1;
        else curSlot++;
#ifdef DEBAG_SLOT_CONFIG
            qDebug() << "SlotConfig Next slot" << curSlot;
#endif
    }
//    if(paramName == "deviceType")
//    {
//        stateUpdConf = 2;
//        int count = addSlot(trans.slave, trans.volInt);
//#ifdef DEBAG_SLOT_CONFIG
//        qDebug() << "countSlots = " << count;
//#endif

//     }
//    else if(paramName == "deviceStatus")
//    {
//#ifdef DEBAG_SLOT_CONFIG
//        qDebug() << "SlotConfig SLOT: slave" << trans.slave \
//                 << paramName << "offset" << trans.offset \
//                 << "vol" << trans.volInt << trans.volFlo;
//#endif
//        stateUpdConf = 4;
//        setStatusDev(trans.slave, trans.volInt);
//    }
//    else if(paramName == "chan0Status")
//    {
//#ifdef DEBAG_SLOT_CONFIG
//        qDebug() << "SlotConfig SLOT: slave" << trans.slave \
//                 << paramName << "offset" << trans.offset \
//                 << "vol" << trans.volInt << trans.volFlo;
//#endif
//        // тут будет много строчек и "else if",
//        // если не восспользоваться таблицей [имя, параметр, функция]
//    }
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
