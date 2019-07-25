#include "device_slot.h"

#define TIME_RESET_ONLINE_SEC   10
#define TIME_UPDATE_STATUS_SEC  3
#define TIME_UPDATE_CONST_SEC   100
#define COUNT_STABLE_STATUS     3

int cDevice::countDev = 1;

cDevice::cDevice(QObject *parent) : QObject(parent)
{
    slot = countDev++;
    online = false;
    counterStatus = 0;
    stableOnline = false;
    deviceType = Device_None;
    deviceStatus = Device_Status_NOINIT;
    factoryDate = 0;
    pauseUpdateParam = false;
    timerResetOnline = new QTimer(this);
    timerUpdateStatus = new QTimer(this);
    timerUpdateConstParam = new QTimer(this);
    connect(timerResetOnline, SIGNAL(timeout()), this, SLOT(resetOnline()));
    resetOnline();
    connect(timerUpdateStatus, SIGNAL(timeout()), this, SLOT(updateStatus()));
    updateStatus();
    connect(timerUpdateConstParam, SIGNAL(timeout()), this, SLOT(updateConstParam()));
    updateConstParam();
    timerResetOnline->start(TIME_RESET_ONLINE_SEC*1000);
    timerUpdateStatus->start(TIME_UPDATE_STATUS_SEC*1000);
    timerUpdateConstParam->start(TIME_UPDATE_CONST_SEC*1000);
    countParams = 0;
    deviceMode = 0;
    accessType = EAT_ROOT;
}

int cDevice::parseDeviceParam(Transaction tr)
{
    if(tr.slave != slot) return -1;    //ошиблись адресом

    // определение стабильности платы
    if(online) counterStatus ++;
    else counterStatus = 0;
    if(counterStatus > COUNT_STABLE_STATUS) stableOnline = true;
    else stableOnline = false;

    online  = true;     // устройство на связи
    timerResetOnline->start(TIME_RESET_ONLINE_SEC*1000);  // перезапуск таймера сброса Онлайна
    QString nameParam = cRegistersMap::getNameByOffset(tr.offset);
    if(nameParam == "protocolVersion")
    {
        protocolVersion = (uint32_t)tr.volInt;
    }
    else if(nameParam == "hardwareVersion")
    {
        hardwareVersion = (uint32_t)tr.volInt;
    }
    else if(nameParam == "softwareVersion")
    {
        softwareVersion = (uint32_t)tr.volInt;
    }
    else if(nameParam == "deviceType")
    {
        deviceType = (deviceTypeEnum)tr.volInt;
    }
    else if(nameParam == "serialNumber")
    {
        serialNumber = tr.volInt;
    }
    else if(nameParam == "uptime")
    {
        uptime = tr.volInt;
    }
//    else if(nameParam == "deviceState")
//    {
//        deviceState = (deviceStateEnum)tr.volInt;
//    }
    else if(nameParam == "accessType")
    {

        if(accessType != (uint16_t)tr.volInt)
        {
            Transaction trans(Transaction::W, slot);
            trans.offset = cRegistersMap::getOffsetByName("accessType");
            trans.volInt = accessType;
            emit updateParam(trans);
        }
//        accessType = tr.volInt;
    }
    else if(nameParam == "mbCommCount")
    {
        mbCommCount = tr.volInt;
    }
    else if(nameParam == "mbCommError")
    {
        mbCommError = tr.volInt;
    }
//    else if(nameParam == "deviceStatus")
//    {
//        deviceStatus = (deviceStatusEnum)tr.volInt;
//    }
    else if(nameParam == "devErrors")
    {
        devErrors = (uint16_t)tr.volInt;
    }
    else if(nameParam == "factoryDate")
    {
        factoryDate = tr.volInt;
    }
    else if(nameParam == "softwareCrc32")
    {
        softwareCrc32 = tr.volInt;
    }
    else if(nameParam == "uniqueId")
    {
        memcpy(uniqueId, tr.paramA12, 12);
    }
    else if(nameParam == "root_Access")
    {
        root_Access = (uint16_t)tr.volInt;
    }
    else if(nameParam == "deviceMode")
    {
        if(deviceMode != (uint16_t)tr.volInt)
        {
            Transaction trans(Transaction::W, slot);
            trans.offset = cRegistersMap::getOffsetByName("deviceMode");
            trans.volInt = deviceMode;
            emit updateParam(trans);
        }
    }

    return 0;
}

void cDevice::setMode(int m)
{
    if((deviceMode != m) && online)
    {
        deviceMode = m;
        Transaction tr(Transaction::W, slot);
        tr.offset = cRegistersMap::getOffsetByName("deviceMode");
        tr.volInt = deviceMode;
        emit updateParam(tr);
    }
}

void cDevice::resetOnline()
{
    if(pauseUpdateParam) return;

    // определение стабильности платы
    if(!online) counterStatus ++;
    else counterStatus = 0;
    if(counterStatus > COUNT_STABLE_STATUS) stableOnline = true;
    else stableOnline = false;


    //если таймер сработал, значит плата давно не отвечала - оффлайн
    online = false;
    Transaction tr(Transaction::R, slot);
    tr.offset = cRegistersMap::getOffsetByName("deviceType");
    emit updateParam(tr);
}

void cDevice::updateStatus()
{
    if(pauseUpdateParam) return;
    if(!online) return;
    Transaction tr(Transaction::R, slot);
    QList<QString> params;
    if((deviceType == Device_None) || ((int)deviceType >= Count_Device_Type))
    {
        params << "deviceType";
    }
    else
    {
        params << "uptime" /*<< "deviceState"*/ << "accessType" << "mbCommCount"\
           << "mbCommError" << "deviceStatus" << "devErrors" << "deviceMode"; /*<< "root_Access";*/
    }
//    for(int i = 0; i < params.size(); i++)
//    {
//        tr.offset = cRegistersMap::getOffsetByName(params.at(i));
//        emit updateParam(tr);
//    }
    tr.offset = cRegistersMap::getOffsetByName(params.at(countParams++));
    emit updateParam(tr);
    if(countParams >= params.size()) countParams = 0;
}

void cDevice::updateConstParam()
{
    if(pauseUpdateParam) return;
    if(!online) return;
    if((deviceType == Device_None) || ((int)deviceType >= Count_Device_Type)) return;
    Transaction tr(Transaction::R, slot);
    QList<QString> params;
    params << "protocolVersion" << "hardwareVersion" << "softwareVersion"\
           << "deviceType" << "serialNumber" << "factoryDate" \
           << "softwareCrc32" << "uniqueId";
    for(int i = 0; i < params.size(); i++)
    {
        tr.offset = cRegistersMap::getOffsetByName(params.at(i));
        emit updateParam(tr);
    }
}
