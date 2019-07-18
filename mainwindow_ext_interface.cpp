#include <QTimer>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defines.h"
#include "Channels/channelOptions.h"
#include "device_slot.h"
#include "Channels/group_channels.h"
#include "Channels/freq_channel.h"
#include "expert_access.h"
#include "Channels/math_channel.h"

// периоды обновления регистров
#define EXT_DEFAULT_UPDATE_MSEC 500 // 500 мсек
#define EXT_DIV_COUNTER_UPDATE 1000/EXT_DEFAULT_UPDATE_MSEC
#define EXT_CONSTANT_UPDATE_SEC 32768 //Константные значения - 9,1 часов
#define EXT_SLOWINFO_UPDATE_SEC 128   //Не оперативная информация - 2 мин
#define EXT_INFO_UPDATE_SEC 4     //Не оперативная информация - 4 сек
#define EXT_STATE_UPDATE_SEC 1     //Состояние прибора - 1 сек

extern QList<cDevice*> listDevice;
extern QList<ChannelOptions *> listChannels;
extern QList<cMathChannel *> listMath;
extern QList<Ustavka *> listUstavok;
extern QList<cRelay*> listRelais;
extern QList<cSteel*> listSteel;
extern QList<cFreqChannel *> listFreq;
//extern QList<cGroupChannels*> listGroup;
extern cIpController * ethernet;
extern cSystemOptions systemOptions;
extern QMutex mSysOpt;
extern QMutex mListUstvok;
extern QMutex mListMath;
extern QMutex mListChannel;
extern QMutex mListDev;
extern QMutex mListFreq;

QMutex mExtModbus;
int currentSetpointNum = 1;
int totalSetpoints = -1;
int currentMathNum = 1;
int totalMath = -1;


void MainWindow::initExtInterface()
{
    /* List Functions of Input Registers */
    tablExtInterfaceRegisters.append({"modbusProtocolVersion", &MainWindow::extGetProtVer});
    tablExtInterfaceRegisters.append({"hardwareVersion", &MainWindow::extGetHardVer});
    tablExtInterfaceRegisters.append({"softwareVersion", &MainWindow::extGetSoftVer});
    tablExtInterfaceRegisters.append({"deviceType", &MainWindow::extGetDevType});
    tablExtInterfaceRegisters.append({"deviceName", &MainWindow::extGetDevName});
    tablExtInterfaceRegisters.append({"manufacturer", &MainWindow::extGetManufact});
    tablExtInterfaceRegisters.append({"webSite", &MainWindow::extGetWebSite});
    tablExtInterfaceRegisters.append({"serialNumber", &MainWindow::extGetSerialNum});
    tablExtInterfaceRegisters.append({"manufactureDate", &MainWindow::extGetManufDate});
    tablExtInterfaceRegisters.append({"deviceID", &MainWindow::extGetDevID});
    tablExtInterfaceRegisters.append({"matherBoardModel", &MainWindow::extGetModelMatherboard});
    tablExtInterfaceRegisters.append({"deviceState", &MainWindow::extGetDevState});
    tablExtInterfaceRegisters.append({"workingTime", &MainWindow::extGetWorkingTime});
    tablExtInterfaceRegisters.append({"modbusCountErrors", &MainWindow::extGetModbusErrs});
    tablExtInterfaceRegisters.append({"modbusCountGoods", &MainWindow::extGetModbusGoods});
    tablExtInterfaceRegisters.append({"accessType", &MainWindow::extGetAccessType});
    tablExtInterfaceRegisters.append({"countModules", &MainWindow::extGetCountModules});

    //Подключаемые модули (платы)
    for(int i=0; i<TOTAL_NUM_DEVICES; i++)
    {
        QString module = "module" + QString::number(i+1);
        tablExtInterfaceRegisters.append({module + "Errors", &MainWindow::extGetModulIsErrors});
        tablExtInterfaceRegisters.append({module + "Online", &MainWindow::extGetModulIsOnline});
        tablExtInterfaceRegisters.append({module + "AccessType", &MainWindow::extGetModulAccessType});
        tablExtInterfaceRegisters.append({module + "Type", &MainWindow::extGetModuleType});
        tablExtInterfaceRegisters.append({module + "ProtocolVersion", &MainWindow::extGetModuleProtocolVer});
        tablExtInterfaceRegisters.append({module + "HardwareVersion", &MainWindow::extGetModuleHardVer});
        tablExtInterfaceRegisters.append({module + "SoftwareVersion", &MainWindow::extGetModuleSoftVer});
        tablExtInterfaceRegisters.append({module + "SerialNumber", &MainWindow::extGetModuleSerialNumber});
        tablExtInterfaceRegisters.append({module + "FactoryDate", &MainWindow::extGetModuleFactoryDate});

        tablSetParamExtInterface.append({"module" + module + "Mode", &MainWindow::extGetModuleMode, &MainWindow::extSetModuleMode});
        tablSetParamExtInterface.append({"module" + module + "ModbusBaud", &MainWindow::extGetModuleModbusBaud, &MainWindow::extSetModuleModbusBaud});
        tablSetParamExtInterface.append({"module" + module + "ModbusParity", &MainWindow::extGetModuleModbusParity, &MainWindow::extSetModuleModbusParity});
        tablSetParamExtInterface.append({"module" + module + "MbStopBits", &MainWindow::extGetModuleModbusStopBits, &MainWindow::extSetModuleModbusStopBits});
        tablSetParamExtInterface.append({"module" + module + "DataOrder", &MainWindow::extGetModuleModbusDataOrder, &MainWindow::extSetModuleModbusDataOrder});
    }

    // Аналоговые каналы
    for(int i = 0; i < TOTAL_NUM_CHAN; i++)
    {
        QString num = QString::number(i+1);
        tablExtInterfaceRegisters.append({"analogChan" + num, &MainWindow::extGetChannel});
        tablExtInterfaceRegisters.append({"analogStatusChan" + num, &MainWindow::extGetChannel});   //Vag: не реализовано
        tablExtInterfaceRegisters.append({"analogUnitChan" + num, &MainWindow::extGetChannel});

        tablSetParamExtInterface.append({"chan" + num + "SignalType", &MainWindow::extGetChannelSignalType, &MainWindow::extSetChannelSignalType});
        tablSetParamExtInterface.append({"chan" + num + "AdditionalParameter1", &MainWindow::extGetChannelAdditionalPar1, &MainWindow::extSetChannelAdditionalPar1});
        tablSetParamExtInterface.append({"chan" + num + "AdditionalParameter2", &MainWindow::extGetChannelAdditionalPar2, &MainWindow::extSetChannelAdditionalPar2});
        tablSetParamExtInterface.append({"chan" + num + "CorrectionCjValue", &MainWindow::extGetChannelCorrectionCj, &MainWindow::extSetChannelCorrectionCj});
        tablSetParamExtInterface.append({"chan" + num + "LowMeasureLim", &MainWindow::extGetChannelLowLim, &MainWindow::extSetChannelLowLim});
        tablSetParamExtInterface.append({"chan" + num + "HighMeasureLim", &MainWindow::extGetChannelHiLim, &MainWindow::extSetChannelHiLim});
        tablSetParamExtInterface.append({"chan" + num + "Name", &MainWindow::extGetChannelName, &MainWindow::extSetChannelName});
        tablSetParamExtInterface.append({"chan" + num + "Period", &MainWindow::extGetChannelPeriod, &MainWindow::extSetChannelPeriod});
        tablSetParamExtInterface.append({"chan" + num + "Dempher", &MainWindow::extGetChannelDempher, &MainWindow::extSetChannelDempher});
        tablSetParamExtInterface.append({"chan" + num + "TypeRegistration", &MainWindow::extGetChannelTypeRegistration, &MainWindow::extSetChannelTypeRegistration});
        tablSetParamExtInterface.append({"chan" + num + "TypeValue", &MainWindow::extGetChannelTypeValue, &MainWindow::extSetChannelTypeValue});

    }

    // Математические каналы
    for(int i = 0; i < TOTAL_NUM_MATH; i++)
    {
        QString name = "mathChan" + QString::number(i+1);
//        tablExtInterfaceChannels.append({name, &MainWindow::extGetChannel});
        tablExtInterfaceRegisters.append({name, &MainWindow::extGetChannel});
    }

    // Частотные каналы
    for(int i = 0; i < TOTAL_NUM_FREQ; i++)
    {
        QString num = QString::number(i+1);
        tablExtInterfaceRegisters.append({"freqChan" + num, &MainWindow::extGetChannel});

        tablSetParamExtInterface.append({"freq" + num + "SignalType", &MainWindow::extGetFreqSignalType, &MainWindow::extSetFreqSignalType});
        tablSetParamExtInterface.append({"freq" + num + "AdditionalParameter", &MainWindow::extGetFreqAdditionalParameter, &MainWindow::extSetFreqAdditionalParameter});
        tablSetParamExtInterface.append({"freq" + num + "ImpulseDuration", &MainWindow::extGetFreqImpulseDuration, &MainWindow::extSetFreqImpulseDuration});
        tablSetParamExtInterface.append({"freq" + num + "ImpWeight", &MainWindow::extGetFreqImpWeight, &MainWindow::extSetFreqImpWeight});
        tablSetParamExtInterface.append({"freq" + num + "LowMeasureLim", &MainWindow::extGetFreqLowMeasureLim, &MainWindow::extSetFreqLowMeasureLim});
        tablSetParamExtInterface.append({"freq" + num + "HighMeasureLim", &MainWindow::extGetFreqHighMeasureLim, &MainWindow::extSetFreqHighMeasureLim});
        tablSetParamExtInterface.append({"freq" + num + "Period", &MainWindow::extGetFreqPeriod, &MainWindow::extSetFreqPeriod});
        tablSetParamExtInterface.append({"freq" + num + "Dempher", &MainWindow::extGetFreqDempher, &MainWindow::extSetFreqDempher});
        tablSetParamExtInterface.append({"freq" + num + "TypeValue", &MainWindow::extGetFreqTypeValue, &MainWindow::extSetFreqTypeValue});
        tablSetParamExtInterface.append({"freq" + num + "Name", &MainWindow::extGetFreqName, &MainWindow::extSetFreqName});
        tablSetParamExtInterface.append({"freq" + num + "Unit", &MainWindow::extGetFreqUnit, &MainWindow::extSetFreqUnit});

    }



    // Таблица (список) функций применения новых параметров
    tablSetParamExtInterface.append({"ipAddress", &MainWindow::extGetIpAddress, &MainWindow::extSetIpAddress});
    tablSetParamExtInterface.append({"maskIP", &MainWindow::extGetIpMask, &MainWindow::extSetIpMask});
    tablSetParamExtInterface.append({"broadcast", &MainWindow::extGetBroadcast, &MainWindow::extSetBroadcast});
    tablSetParamExtInterface.append({"portWebServer", &MainWindow::extGetPortWebServer, &MainWindow::extSetPortWebServer});

    tablSetParamExtInterface.append({"modbusTcpPort", &MainWindow::extGetPortModbus, &MainWindow::extSetPortModbus});
    tablSetParamExtInterface.append({"modbusType", &MainWindow::extGetPortModbusType, &MainWindow::extSetPortModbusType});
    tablSetParamExtInterface.append({"modbusSlaveAddress", &MainWindow::extGetModbusSlave, &MainWindow::extSetModbusSlave});
    tablSetParamExtInterface.append({"modbusRtuBaudrate", &MainWindow::extGetModbusRtuBaudrate, &MainWindow::extSetModbusRtuBaudrate});

    tablSetParamExtInterface.append({"displayMode", &MainWindow::extGetDisplayMode, &MainWindow::extSetDisplayMode});
    tablSetParamExtInterface.append({"displayAutoscale", &MainWindow::extGetDisplayAutoscale, &MainWindow::extSetDisplayAutoscale});
    tablSetParamExtInterface.append({"displayArrows", &MainWindow::extGetDisplayArrows, &MainWindow::extSetDisplayArrows});
    tablSetParamExtInterface.append({"brightness", &MainWindow::extGetDisplayBrightness, &MainWindow::extSetDisplayBrightness});
    tablSetParamExtInterface.append({"deviceMode", &MainWindow::extGetDevMode, &MainWindow::extSetDevMode});
    tablSetParamExtInterface.append({"currentDate", &MainWindow::extGetCurrentDate, &MainWindow::extSetCurrentDate});
    tablSetParamExtInterface.append({"currentTime", &MainWindow::extGetCurrentTime, &MainWindow::extSetCurrentTime});
    tablSetParamExtInterface.append({"accessPass", &MainWindow::extGetAccessPass, &MainWindow::extSetAccessPass});
    tablSetParamExtInterface.append({"currentSetpointNum", &MainWindow::extGetSetPoint, &MainWindow::extSetSetPoint});
    tablSetParamExtInterface.append({"totalSetpoint", &MainWindow::extGetTotalSetpoint, &MainWindow::extSetTotalSetpoint});
    tablSetParamExtInterface.append({"setpointIdentifikator", &MainWindow::extGetSetpointIdentifikator, &MainWindow::extSetSetpointIdentifikator});
    tablSetParamExtInterface.append({"setpointNumChan", &MainWindow::extGetSetpointChannel, &MainWindow::extSetSetpointChannel});
    tablSetParamExtInterface.append({"setpointTypeFix", &MainWindow::extGetSetpointType, &MainWindow::extSetSetpointType});
    tablSetParamExtInterface.append({"setpointValue", &MainWindow::extGetSetpointValue, &MainWindow::extSetSetpointValue});
    tablSetParamExtInterface.append({"setpointHisteresis", &MainWindow::extGetSetpointHister, &MainWindow::extSetSetpointHister});
    tablSetParamExtInterface.append({"setpointNumRelay", &MainWindow::extGetSetpointRelay, &MainWindow::extSetSetpointRelay});
    tablSetParamExtInterface.append({"setpointOverMess", &MainWindow::extGetSetpointOverMess, &MainWindow::extSetSetpointOverMess});
    tablSetParamExtInterface.append({"setpointNormMess", &MainWindow::extGetSetpointNormMess, &MainWindow::extSetSetpointNormMess});
    tablSetParamExtInterface.append({"setpointUserConfirm", &MainWindow::extGetSetpointConfirm, &MainWindow::extSetSetpointConfirm});
    tablSetParamExtInterface.append({"setpointTimeFilter", &MainWindow::extGetSetpointTimeFilter, &MainWindow::extSetSetpointTimeFilter});
    tablSetParamExtInterface.append({"currentMathNum", &MainWindow::extGetCurrentMath, &MainWindow::extSetCurrentMath});
    tablSetParamExtInterface.append({"totalMath", &MainWindow::extGetTotalMath, &MainWindow::extSetTotalMath});
    tablSetParamExtInterface.append({"mathName", &MainWindow::extGetMathName, &MainWindow::extSetMathName});
    tablSetParamExtInterface.append({"mathFormula", &MainWindow::extGetMathFormula, &MainWindow::extSetMathFormula});
    tablSetParamExtInterface.append({"mathX1", &MainWindow::extGetMathArgument, &MainWindow::extSetMathArgument});
    tablSetParamExtInterface.append({"mathX2", &MainWindow::extGetMathArgument, &MainWindow::extSetMathArgument});
    tablSetParamExtInterface.append({"mathX3", &MainWindow::extGetMathArgument, &MainWindow::extSetMathArgument});
    tablSetParamExtInterface.append({"mathX4", &MainWindow::extGetMathArgument, &MainWindow::extSetMathArgument});
    tablSetParamExtInterface.append({"mathUnit", &MainWindow::extGetMathUnit, &MainWindow::extSetMathUnit});
    tablSetParamExtInterface.append({"mathHiLimit", &MainWindow::extGetMathHiLimit, &MainWindow::extSetMathHiLimit});
    tablSetParamExtInterface.append({"mathLowLimit", &MainWindow::extGetMathLowLimit, &MainWindow::extSetMathLowLimit});
    tablSetParamExtInterface.append({"mathPeriod", &MainWindow::extGetMathPeriod, &MainWindow::extSetMathPeriod});









    /* Запуск таймеров */
    // Отправка данных с периодом 0,5 сек
    connect(&timerExtInterface, SIGNAL(timeout()), this, SLOT(updateObjectsOfMainThread()));
    timerExtInterface.start(EXT_DEFAULT_UPDATE_MSEC);
}

//Обновление объектов основного потока в ОСНОВНОМ потоке
void MainWindow::updateObjectsOfMainThread()
{
    //Контроль количества уставок
    int size = listUstavok.size();
    if((totalSetpoints != -1) && (totalSetpoints != size) && (totalSetpoints <= TOTAL_NUM_USTAVKI))
    {
        mListUstvok.lock();
        for(int i = totalSetpoints; i < size; i++)
        {
            listUstavok.removeLast();
        }
        for(int i = size; i < totalSetpoints; i++)
        {
            Ustavka * ust = new Ustavka();
            ust->setNum(i);
            ust->setIdentifikator("Limit " + QString::number(i+1));
            connect(ust, SIGNAL(workReleSignal(int, bool)), this, SLOT(sendRelayStateToWorker(int, bool)));
            listUstavok.append(ust);
        }
        mListUstvok.unlock();
    }

    //Контроль количества матканалов
    size = listMath.size();
    if((totalMath != -1) && (totalMath != size) && (totalMath <= TOTAL_NUM_MATH))
    {
        mListMath.lock();
        for(int i = totalMath; i < size; i++)
        {
            listMath.removeLast();
        }
        for(int i = size; i < totalMath; i++)
        {
            cMathChannel * math = new cMathChannel();
            math->setNum(i);
            math->setName("Math " + QString::number(i+1));
            listMath.append(math);
        }
        mListMath.unlock();
    }

}

// Отправка актуальных данных во внешние интерфейсы
void MainWindow::updateExtIntefaceData()
{


}

void MainWindow::slotUpdateExtIntefaceData(QString name)
{

   //Vag: тут можно поставить таймер фильтрации частоты запросов

    foreach (typeTableExtInterface reg, tablExtInterfaceRegisters) {
        if(reg.name == name)
        {
            (this->*reg.funcGet)(name);
        }
    }

    foreach (typeTableSetParamExtInterface par, tablSetParamExtInterface) {
        if(par.name == name){
            (this->*par.funcGet)(name);
            break;
        }
    }
}


void MainWindow::slotFromExtModbus(QString name, tModbusBuffer data)
{
    applyParam(name, (uint8_t*) &data);
}

// Функция применения новых параметров
void MainWindow::applyParam(QString name, uint8_t * data)
{
    foreach (typeTableSetParamExtInterface par, tablSetParamExtInterface) {
        if(par.name == name){
            (this->*par.funcSet)(name, data);
            break;
        }
    }
}

/* Функции обработки данных (чтение/запись) */

// Данные каналов
void MainWindow::extGetChannel(QString name)
{
    tModbusBuffer data;
    if(name.contains("analogChan"))
    {
        int chan = name.right(name.size() - QString("analogChan").size()).toInt();
        float fl = 0;
        if((chan > 0) && (chan <= listChannels.size()))
        {
            mListChannel.lock();
            fl = (float)listChannels.at(chan-1)->GetCurrentChannelValue();
            mListChannel.unlock();
        }
        memcpy(&data, &fl, sizeof(float));
    }
    else if(name.contains("mathChan"))
    {
        int chan = name.right(name.size() - QString("mathChan").size()).toInt();
        float fl = 0;
        if((chan > 0) && (chan <= listMath.size()))
        {
            fl = (float)listMath.at(chan-1)->GetCurrentMathValue();
        }
        memcpy(&data, &fl, sizeof(float));
    }
    else if(name.contains("freqChan"))
    {
        int chan = name.right(name.size() - QString("freqChan").size()).toInt();
        float fl = 0;
        mListFreq.lock();
        if((chan > 0) && (chan <= listFreq.size()))
        {
            fl = (float)listFreq.at(chan-1)->GetCurrentChannelValue();
        }
        mListFreq.unlock();
        memcpy(&data, &fl, sizeof(float));
    }

    emit signalToExtModbus(name, data);
}



void MainWindow::extGetProtVer(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 4);
    QString verStr = PROTOCOL_VER;
    QStringList listStrVer = verStr.split('.');
    int i = 0;
    foreach (QString str, listStrVer) {
        data.data[i++] = str.toInt();
    }
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetChannelStatus(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 4);
    int chan = name.right(name.size() - QString("analogStatusChan").size()).toInt();
    if((chan>0) && (chan<=listChannels.size()))
    {
        mListChannel.lock();
        int status = listChannels.at(chan-1)->getChannelStatus();
        mListChannel.unlock();
        data.data[0] = status & 0xFF;
    }
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetHardVer(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 4);
    QString verStr = HARDWARE_VERSION;
    QStringList listStrVer = verStr.split('.');
    int i = 0;
    foreach (QString str, listStrVer) {
        data.data[i++] = str.toInt();
    }
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetSoftVer(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 4);
    QString verStr = VER;
    QStringList listStrVer = verStr.split('.');
    int i = 0;
    foreach (QString str, listStrVer) {
        data.data[i++] = str.toInt();
    }
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetDevType(QString name)
{
    tModbusBuffer data;
    data.data[0] = systemOptions.typeMultigraph;
    data.data[1] = 0;
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetDevName(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 32);
    QString StrName = "Multigraph";
    if(systemOptions.typeMultigraph == cSystemOptions::Multigraph_Steel)
    {
        StrName = "Multigraph-Steel";
    }
    memcpy(&data, StrName.toLocal8Bit(), StrName.toLocal8Bit().size());
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetManufact(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 32);
    QString StrName = "Teplopribor";
    memcpy(&data, StrName.toLocal8Bit(), StrName.toLocal8Bit().size());
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetWebSite(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 32);
    QString StrName = "tpchel.ru";
    memcpy(&data, StrName.toLocal8Bit(), StrName.toLocal8Bit().size());
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetSerialNum(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 32);
    QString StrName = "000123";
    memcpy(&data, StrName.toLocal8Bit(), StrName.toLocal8Bit().size());
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetManufDate(QString name)
{
    //Vag: переделать после реализации хранения даты производства

    tModbusBuffer data;
    memset(&data, 0, 4);
    QString manufDate = "9.07.2019";
    QStringList listStr = manufDate.split('.');
    QString str;
    str = listStr.at(2);
    data.data[0] = str.toInt() & 0xFF;
    data.data[1] = (str.toInt() >> 8) & 0xFF;
    str = listStr.at(1);
    data.data[2] = str.toInt();
    str = listStr.at(0);
    data.data[3] = str.toInt();
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetDevID(QString name)
{
    tModbusBuffer data;
    data.data[0] = 0;
    data.data[1] = 0;
    data.data[2] = 0;
    data.data[3] = 0;
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetDevState(QString name)
{
    tModbusBuffer data;
    data.data[0] = 2;
    data.data[1] = 0;
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetWorkingTime(QString name)
{
    tModbusBuffer data;
    data.data[0] = 0;
    data.data[1] = 0;
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModbusErrs(QString name)
{
    tModbusBuffer data;
    data.data[0] = 0;
    data.data[1] = 0;
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModbusGoods(QString name)
{
    tModbusBuffer data;
    data.data[0] = 0;
    data.data[1] = 0;
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetAccessType(QString name)
{

    // Vag: реализовать отдельный признак доступа к прибору

    tModbusBuffer data;
    data.data[0] = cExpertAccess::getMode();
    data.data[1] = 0;
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetCountModules(QString name)
{
    tModbusBuffer data;
    uint8_t count = 0;
    mListDev.lock();
    foreach (cDevice * dev, listDevice) {
        if(dev->getOnline()) count++;
    }
    mListDev.unlock();
    data.data[0] = count;
    data.data[1] = 0;
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModelMatherboard(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 32);
    QString StrName = "MYD-AM335X";
    memcpy(&data, StrName.toLocal8Bit(), StrName.toLocal8Bit().size());
    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModulIsErrors(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint32_t errors = listDevice.at(num-1)->devErrors;
        memcpy(&data, &errors, sizeof(errors));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModulIsOnline(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint16_t flag = listDevice.at(num-1)->getOnline()?1:0;
        memcpy(&data, &flag, sizeof(flag));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModulAccessType(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint16_t flag = 0;
        if(listDevice.at(num-1)->accessType == EAT_SERVICE)
        {
            flag = 1;
        }
        else if(listDevice.at(num-1)->accessType == EAT_ROOT)
        {
            flag = 2;
        }
        memcpy(&data, &flag, sizeof(flag));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModuleProtocolVer(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint32_t ver = (uint32_t)listDevice.at(num-1)->protocolVersion;
        memcpy(&data, &ver, sizeof(ver));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModuleHardVer(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint32_t ver = (uint32_t)listDevice.at(num-1)->hardwareVersion;
        memcpy(&data, &ver, sizeof(ver));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModuleSoftVer(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint32_t ver = (uint32_t)listDevice.at(num-1)->softwareVersion;
        memcpy(&data, &ver, sizeof(ver));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModuleSerialNumber(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint32_t serial = (uint32_t)listDevice.at(num-1)->serialNumber;
        memcpy(&data, &serial, sizeof(serial));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModuleFactoryDate(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint32_t serial = (uint32_t)listDevice.at(num-1)->serialNumber;
        memcpy(&data, &serial, sizeof(serial));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

void MainWindow::extGetModuleType(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint16_t type = (uint16_t)listDevice.at(num-1)->deviceType;
        memcpy(&data, &type, sizeof(type));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}



/*
 * Функции установки/чтения параметров (Holding Registers)
 */

// установка режима дисплея
void MainWindow::extSetDisplayMode(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t dataUint16;
    memcpy(&dataUint16, data, sizeof(dataUint16));
    systemOptions.display = dataUint16;
    mSysOpt.unlock();
}
// Получение режима дисплея
void MainWindow::extGetDisplayMode(QString name)
{
    tModbusBuffer data;
    uint16_t dataUint16 = systemOptions.display;
    memcpy(&data, &dataUint16, sizeof(dataUint16));
    emit signalToExtModbus(name, data);
}

// установка IP адреса прибора
void MainWindow::extSetIpAddress(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint32_t address;
    memcpy(&address, data, sizeof(address));
//    systemOptions.address = address;
    mSysOpt.unlock();
}
// Получение IP адреса прибора
void MainWindow::extGetIpAddress(QString name)
{
    tModbusBuffer data;
    uint32_t address = (192 << 24) + (168 << 16) + (0 << 8) + 2;
    memcpy(&data, &address, sizeof(address));
    emit signalToExtModbus(name, data);
}

// установка IP маски подсети
void MainWindow::extSetIpMask(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint32_t mask;
    memcpy(&mask, data, sizeof(mask));
//    systemOptions.mask = mask;
    mSysOpt.unlock();
}
// Получение IP маски подсети
void MainWindow::extGetIpMask(QString name)
{
    tModbusBuffer data;
    uint32_t mask = (255 << 24) + (255 << 16) + (0 << 8) + 0;
    memcpy(&data, &mask, sizeof(mask));
    emit signalToExtModbus(name, data);
}

// установка Шлюза подсети
void MainWindow::extSetBroadcast(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint32_t broadcast;
    memcpy(&broadcast, data, sizeof(broadcast));
//    systemOptions.broadcast = broadcast;
    mSysOpt.unlock();
}
// Получение Шлюза подсети
void MainWindow::extGetBroadcast(QString name)
{
    tModbusBuffer data;
    uint32_t broadcast = (192 << 24) + (168 << 16) + (0 << 8) + 1;
    memcpy(&data, &broadcast, sizeof(broadcast));
    emit signalToExtModbus(name, data);
}

// установка порта веб-сервера
void MainWindow::extSetPortWebServer(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t port;
    memcpy(&port, data, sizeof(port));
//    systemOptions.portWebServer = port;
    mSysOpt.unlock();
}
// Получение порта веб-сервера
void MainWindow::extGetPortWebServer(QString name)
{
    tModbusBuffer data;
    uint16_t port = 80;
    memcpy(&data, &port, sizeof(port));
    emit signalToExtModbus(name, data);
}

// установка порта Modbus TCP
void MainWindow::extSetPortModbus(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t port;
    memcpy(&port, data, sizeof(port));
//    systemOptions.portModbus = port;
    mSysOpt.unlock();
}
// Получение порта Modbus TCP
void MainWindow::extGetPortModbus(QString name)
{
    tModbusBuffer data;
    uint16_t port = 502;
    memcpy(&data, &port, sizeof(port));
    emit signalToExtModbus(name, data);
}

// установка типа Modbus соединения
void MainWindow::extSetPortModbusType(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t type;
    memcpy(&type, data, sizeof(type));
//    systemOptions.modbusType = type;
    mSysOpt.unlock();
}
// Получение типа Modbus соединения
void MainWindow::extGetPortModbusType(QString name)
{
    tModbusBuffer data;
    uint16_t type = 0;
    memcpy(&data, &type, sizeof(type));
    emit signalToExtModbus(name, data);
}

// установка Modbus слэйв адреса
void MainWindow::extSetModbusSlave(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t slave;
    memcpy(&slave, data, sizeof(slave));
    systemOptions.extModbus.adress = slave;
    mSysOpt.unlock();
}
// Получение Modbus слэйв адреса
void MainWindow::extGetModbusSlave(QString name)
{
    tModbusBuffer data;
    uint16_t slave = systemOptions.extModbus.adress;
    memcpy(&data, &slave, sizeof(slave));
    emit signalToExtModbus(name, data);
}

// установка скорости Modbus RTU
void MainWindow::extSetModbusRtuBaudrate(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t baud;
    memcpy(&baud, data, sizeof(baud));
    systemOptions.extModbus.baud = baud;
    mSysOpt.unlock();
}
// Получение скорости Modbus RTU
void MainWindow::extGetModbusRtuBaudrate(QString name)
{
    tModbusBuffer data;
    uint16_t baud = systemOptions.extModbus.baud;
    memcpy(&data, &baud, sizeof(baud));
    emit signalToExtModbus(name, data);
}

// установка автомасштабирования графиков
void MainWindow::extSetDisplayAutoscale(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t autoscale;
    memcpy(&autoscale, data, sizeof(autoscale));
    systemOptions.autoscale = (autoscale != 0) ? true : false;
    mSysOpt.unlock();
}
// Получение автомасштабирования графиков
void MainWindow::extGetDisplayAutoscale(QString name)
{
    tModbusBuffer data;
    uint16_t autoscale = systemOptions.autoscale ? 1 : 0;
    memcpy(&data, &autoscale, sizeof(autoscale));
    emit signalToExtModbus(name, data);
}

// установка отображения стрелок на графике
void MainWindow::extSetDisplayArrows(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t arrows;
    memcpy(&arrows, data, sizeof(arrows));
    systemOptions.arrows = (arrows != 0) ? true : false;
    mSysOpt.unlock();
}
// Получение отображения стрелок на графике
void MainWindow::extGetDisplayArrows(QString name)
{
    tModbusBuffer data;
    uint16_t arrows = systemOptions.arrows ? 1 : 0;
    memcpy(&data, &arrows, sizeof(arrows));
    emit signalToExtModbus(name, data);
}

// установка яркости
void MainWindow::extSetDisplayBrightness(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t brightness;
    memcpy(&brightness, data, sizeof(brightness));
    if(brightness < 10)
    {
        brightness = 10;
    }
    else if(brightness > 100)
    {
        brightness = 100;
    }
    systemOptions.brightness = brightness;
    mSysOpt.unlock();
}
// Получение яркости
void MainWindow::extGetDisplayBrightness(QString name)
{
    tModbusBuffer data;
    uint16_t brightness = systemOptions.brightness;
    memcpy(&data, &brightness, sizeof(brightness));
    emit signalToExtModbus(name, data);
}

// установка режима прибора
void MainWindow::extSetDevMode(QString name, uint8_t * data)
{
    mSysOpt.lock();
    uint16_t mode;
    memcpy(&mode, data, sizeof(mode));
    systemOptions.mode = mode;
    mSysOpt.unlock();
}
// Получение режима прибора
void MainWindow::extGetDevMode(QString name)
{
    tModbusBuffer data;
    uint16_t mode = systemOptions.mode;
    memcpy(&data, &mode, sizeof(mode));
    emit signalToExtModbus(name, data);
}

// установка текущей даты прибора
void MainWindow::extSetCurrentDate(QString name, uint8_t * data)
{
//    uint32_t date;
//    memcpy(&date, data, sizeof(date));

#ifndef WIN32
    QString newdate = QString::number((data[3] * 0x00FF) + data[2])\
            + "-" + QString::number(data[1]) + "-" + QString::number(data[0]);
    QString newtime = QDateTime::currentDateTime().time().toString();

    QProcess process;
    process.startDetached("date --set " + newdate);
    process.startDetached("date --set " + newtime); // max freq on
    process.startDetached("hwclock -w");
#endif
}
// Получение текущей даты прибора
void MainWindow::extGetCurrentDate(QString name)
{
    tModbusBuffer data;
    uint32_t date;
    date = QDateTime::currentDateTime().date().year() << 16;
    date += QDateTime::currentDateTime().date().month() << 8;
    date += QDateTime::currentDateTime().date().day();
//    data.data[3] = QDateTime::currentDateTime().date().year() / 0xFF;
//    data.data[2] = QDateTime::currentDateTime().date().year() & 0xFF;
//    data.data[1] = QDateTime::currentDateTime().date().month();
//    data.data[0] = QDateTime::currentDateTime().date().day();
    memcpy(&data, &date, sizeof(date));
    emit signalToExtModbus(name, data);
}

// установка текущего времени
void MainWindow::extSetCurrentTime(QString name, uint8_t * data)
{

#ifndef WIN32
    QString newdate = QDateTime::currentDateTime().date().toString("YYYY-MM-DD");
    QString newtime = QString::number(data[3]) + ":" + QString::number(data[2]) + ":" + QString::number(data[1]);
    QProcess process;
    process.startDetached("date --set " + newdate);
    process.startDetached("date --set " + newtime); // max freq on
    process.startDetached("hwclock -w");
#endif
}
// Получение текущего времени
void MainWindow::extGetCurrentTime(QString name)
{
    tModbusBuffer data;
    uint32_t date;
    date = QDateTime::currentDateTime().time().hour() << 24;
    date += QDateTime::currentDateTime().time().minute() << 16;
    date += QDateTime::currentDateTime().time().second() << 8;
    memcpy(&data, &date, sizeof(date));
    emit signalToExtModbus(name, data);
}

// установка текущего уровня доступа (пароль)
void MainWindow::extSetAccessPass(QString name, uint8_t * data)
{



}
// Получение снежинок (пароль скрыт)
void MainWindow::extGetAccessPass(QString name)
{
    tModbusBuffer data;
//    data.data[0] = 0;
    memset(&data, '*', 32);
    emit signalToExtModbus(name, data);
}

// установка номера редактируемой уставки
void MainWindow::extSetSetPoint(QString name, uint8_t * data)
{
    int num = data[0] + data[1] * 0xF - 1;
    if((num >= 0) && (num < listUstavok.size()))
    {
        mExtModbus.lock();
        currentSetpointNum = num;
        mExtModbus.unlock();
    }
}
// Получение номера редактируемой уставки
void MainWindow::extGetSetPoint(QString name)
{
    tModbusBuffer data;
    int num = currentSetpointNum + 1;
    data.data[0] = num & 0xFF;
    data.data[1] = (num << 8) &0xFF;
    emit signalToExtModbus(name, data);
}

// установка количества уставок (создание/удаление)
void MainWindow::extSetTotalSetpoint(QString name, uint8_t * data)
{
    uint16_t total = 1;
    memcpy(&total, data, sizeof(total));
    mExtModbus.lock();
    totalSetpoints = total;
    mExtModbus.unlock();
}
// Получение количества уставок
void MainWindow::extGetTotalSetpoint(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 4);
    int total = listUstavok.size();
    data.data[0] = total & 0xFF;
    data.data[1] = (total >> 8) & 0xFF;
    emit signalToExtModbus(name, data);
}

#define LENGHT_STR_IDENTOFIKATOR 32
// установка названия уставки
void MainWindow::extSetSetpointIdentifikator(QString name, uint8_t * data)
{
    char str[LENGHT_STR_IDENTOFIKATOR];
    memcpy(&str, data, LENGHT_STR_IDENTOFIKATOR);
    QString identificator = QString(QByteArray(str, LENGHT_STR_IDENTOFIKATOR));

    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        listUstavok.at(currentSetpointNum)->setIdentifikator(identificator);
    }
    mListUstvok.unlock();
}
// Получение названия уставки
void MainWindow::extGetSetpointIdentifikator(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, LENGHT_STR_IDENTOFIKATOR);
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        QString StrName = listUstavok.at(currentSetpointNum)->getIdentifikator();
        memcpy(&data, StrName.toUtf8(), LENGHT_STR_IDENTOFIKATOR);
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}

// установка номера входного сигнала
void MainWindow::extSetSetpointChannel(QString name, uint8_t * data)
{
    int channel = data[0];
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        Ustavka * ust = listUstavok.at(currentSetpointNum);
        ust->setUstavka(ust->getIdentifikator(), \
                        channel, \
                        ust->getTypeFix(), \
                        ust->getHiStateValue(),\
                        ust->getHiHisteresis(),\
                        ust->getnumRelayUp());
    }
    mListUstvok.unlock();
}
// Получение номера входного сигнала
void MainWindow::extGetSetpointChannel(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 32);
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        uint8_t channel = listUstavok.at(currentSetpointNum)->getChannel() & 0xFF;
        memcpy(&data, &channel, sizeof(channel));
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}

// установка типа фиксации
void MainWindow::extSetSetpointType(QString name, uint8_t * data)
{
    int type = data[0];
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        Ustavka * ust = listUstavok.at(currentSetpointNum);
        ust->setUstavka(ust->getIdentifikator(), \
                        ust->getChannel(), \
                        type, \
                        ust->getHiStateValue(),\
                        ust->getHiHisteresis(),\
                        ust->getnumRelayUp());
    }
    mListUstvok.unlock();
}
// Получение типа фиксации
void MainWindow::extGetSetpointType(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 32);
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        uint8_t type = listUstavok.at(currentSetpointNum)->getTypeFix() & 0xFF;
        memcpy(&data, &type, sizeof(type));
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}

// установка уровня срабатывания уставки
void MainWindow::extSetSetpointValue(QString name, uint8_t * data)
{
    float value;
    memcpy(&value, data, sizeof(value));
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        Ustavka * ust = listUstavok.at(currentSetpointNum);
        ust->setUstavka(ust->getIdentifikator(), \
                        ust->getChannel(), \
                        ust->getTypeFix(), \
                        (double)value,\
                        ust->getHiHisteresis(),\
                        ust->getnumRelayUp());
    }
    mListUstvok.unlock();
}
// Получение уровня срабатывания уставки
void MainWindow::extGetSetpointValue(QString name)
{
    tModbusBuffer data;
    float value;
    memset(&data, 0, sizeof(value));
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        value = (float)listUstavok.at(currentSetpointNum)->getHiStateValue();
        memcpy(&data, &value, sizeof(value));
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}

// установка гистерезиса
void MainWindow::extSetSetpointHister(QString name, uint8_t * data)
{
    float value;
    memcpy(&value, data, sizeof(value));
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        Ustavka * ust = listUstavok.at(currentSetpointNum);
        ust->setUstavka(ust->getIdentifikator(), \
                        ust->getChannel(), \
                        ust->getTypeFix(), \
                        ust->getHiStateValue(),\
                        (double)value,\
                        ust->getnumRelayUp());
    }
    mListUstvok.unlock();
}
// Получение гистерезиса
void MainWindow::extGetSetpointHister(QString name)
{
    tModbusBuffer data;
    float value;
    memset(&data, 0, sizeof(value));
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        value = (float)listUstavok.at(currentSetpointNum)->getHiHisteresis();
        memcpy(&data, &value, sizeof(value));
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}

// установка выходного дискретного сигнала
void MainWindow::extSetSetpointRelay(QString name, uint8_t * data)
{
    int relay = data[0] + (data[1] << 8);
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        Ustavka * ust = listUstavok.at(currentSetpointNum);
        ust->setUstavka(ust->getIdentifikator(), \
                        ust->getChannel(), \
                        ust->getTypeFix(), \
                        ust->getHiStateValue(),\
                        ust->getHiHisteresis(),\
                        relay);
    }
    mListUstvok.unlock();
}
// Получение выходного дискретного сигнала
void MainWindow::extGetSetpointRelay(QString name)
{
    tModbusBuffer data;
    int relay;
    memset(&data, 0, sizeof(relay));
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        relay = listUstavok.at(currentSetpointNum)->getnumRelayUp();
        memcpy(&data, &relay, sizeof(relay));
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}

#define LENGHT_STR_OVERMESS 64
// установка сообщения при срабатывании уставки
void MainWindow::extSetSetpointOverMess(QString name, uint8_t * data)
{
    char str[LENGHT_STR_OVERMESS];
    memcpy(&str, data, LENGHT_STR_OVERMESS);
    QString mess = QString(QByteArray(str, LENGHT_STR_OVERMESS));

    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        listUstavok.at(currentSetpointNum)->setMessInHigh(mess);
    }
    mListUstvok.unlock();
}
// Получение сообщения при срабатывании уставки
void MainWindow::extGetSetpointOverMess(QString name)
{
    tModbusBuffer data;
    QString str;
    memset(&data, 0, LENGHT_STR_OVERMESS);
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        QString str = listUstavok.at(currentSetpointNum)->getMessInHigh();
        memcpy(&data, str.toUtf8(), LENGHT_STR_OVERMESS);
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}

#define LENGHT_STR_NORMMESS 64
// установка сообщения восстановления уставки
void MainWindow::extSetSetpointNormMess(QString name, uint8_t * data)
{
    char str[LENGHT_STR_NORMMESS];
    memcpy(&str, data, LENGHT_STR_NORMMESS);
    QString mess = QString(QByteArray(str, LENGHT_STR_NORMMESS));

    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        listUstavok.at(currentSetpointNum)->setMessNormHigh(mess);
    }
    mListUstvok.unlock();
}
// Получение сообщения восстановления уставки
void MainWindow::extGetSetpointNormMess(QString name)
{
    tModbusBuffer data;
    QString str;
    memset(&data, 0, LENGHT_STR_NORMMESS);
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        QString str = listUstavok.at(currentSetpointNum)->getMessNormHigh();
        memcpy(&data, str.toUtf8(), LENGHT_STR_NORMMESS);
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}

// установка необходимости подтверждения
void MainWindow::extSetSetpointConfirm(QString name, uint8_t * data)
{
    bool kvit = (data[0] != 0) ? true : false;
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        listUstavok.at(currentSetpointNum)->setKvitirUp(kvit);
    }
    mListUstvok.unlock();
}
// Получение признака необходимости подтверждения
void MainWindow::extGetSetpointConfirm(QString name)
{
    tModbusBuffer data;
    mListUstvok.lock();
    if(currentSetpointNum < listUstavok.size())
    {
        int kvit = listUstavok.at(currentSetpointNum)->getKvitirUp() ? 1 : 0;
        data.data[0] = kvit && 0xFF;
        data.data[1] = 0;
    }
    mListUstvok.unlock();
    emit signalToExtModbus(name, data);
}


// установка задержки перед переключением
void MainWindow::extSetSetpointTimeFilter(QString name, uint8_t * data)
{
    // Vag: пока не реализовано изменение этого параметра
}
// Получение задержки перед переключением
void MainWindow::extGetSetpointTimeFilter(QString name)
{
    tModbusBuffer data;
    data.data[0] = DELAY_RELAY && 0xFF;
    data.data[1] = (DELAY_RELAY >> 8) && 0xFF;
    emit signalToExtModbus(name, data);
}

// установка номера редактируемого матканала
void MainWindow::extSetCurrentMath(QString name, uint8_t * data)
{
    int num = data[0] + data[1] * 0xF - 1;
    if((num >= 0) && (num < listUstavok.size()))
    {
        mExtModbus.lock();
        currentMathNum = num;
        mExtModbus.unlock();
    }
}
// Получение номера редактируемого матканала
void MainWindow::extGetCurrentMath(QString name)
{
    tModbusBuffer data;
    int num = currentMathNum + 1;
    data.data[0] = num & 0xFF;
    data.data[1] = (num << 8) &0xFF;
    emit signalToExtModbus(name, data);
}

// установка количества матканалов (создание/удаление)
void MainWindow::extSetTotalMath(QString name, uint8_t * data)
{
    uint16_t total = 1;
    memcpy(&total, data, sizeof(total));
    mExtModbus.lock();
    totalMath = total;
    mExtModbus.unlock();
}
// Получение количества матканалов
void MainWindow::extGetTotalMath(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 4);
    int total = listMath.size();
    data.data[0] = total & 0xFF;
    data.data[1] = (total >> 8) & 0xFF;
    emit signalToExtModbus(name, data);
}

#define LENGHT_MATH_NAME 8
// установка названия матканала
void MainWindow::extSetMathName(QString name, uint8_t * data)
{
    char str[LENGHT_MATH_NAME];
    memcpy(&str, data, LENGHT_MATH_NAME);
    QString nameMath = QString(QByteArray(str, LENGHT_MATH_NAME));

    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        listMath.at(currentMathNum)->setName(nameMath);
    }
    mListMath.unlock();
}
// Получение названия матканала
void MainWindow::extGetMathName(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, LENGHT_MATH_NAME);
    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        QString StrName = listMath.at(currentMathNum)->getName();
        memcpy(&data, StrName.toUtf8(), LENGHT_MATH_NAME);
    }
    mListMath.unlock();
    emit signalToExtModbus(name, data);
}

#define LENGHT_MATH_FORMULA 32
// установка формулы матканала
void MainWindow::extSetMathFormula(QString name, uint8_t * data)
{
    char str[LENGHT_MATH_FORMULA];
    memcpy(&str, data, LENGHT_MATH_FORMULA);
    QString formula = QString(QByteArray(str, LENGHT_MATH_FORMULA));

    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        listMath.at(currentMathNum)->SetMathEquation(formula);
    }
    mListMath.unlock();
}
// Получение формулы матканала
void MainWindow::extGetMathFormula(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, LENGHT_MATH_FORMULA);
    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        QString strFormula = listMath.at(currentMathNum)->GetMathString();
        memcpy(&data, strFormula.toUtf8(), LENGHT_MATH_FORMULA);
    }
    mListMath.unlock();
    emit signalToExtModbus(name, data);
}

// установка аргументов формулы матканала
void MainWindow::extSetMathArgument(QString name, uint8_t * data)
{
    if(name.contains("mathX"))
    {
        int chan = name.right(name.size() - QString("mathX").size()).toInt();
        if((chan > 0) && (chan <= NUM_MATH_ARG) && (currentMathNum <= listMath.size()))
        {
            mListMath.lock();
            listMath.at(currentMathNum)->numChannel[chan-1] = data[0];
            mListMath.unlock();
        }
    }
}
// Получение аргументов формулы матканала
void MainWindow::extGetMathArgument(QString name)
{
    tModbusBuffer data;
    if(name.contains("mathX"))
    {
        int chan = name.right(name.size() - QString("mathX").size()).toInt();
        if((chan > 0) && (chan <= NUM_MATH_ARG) && (currentMathNum <= listMath.size()))
        {
            int argument = listMath.at(currentMathNum)->numChannel[chan-1];
            data.data[0] = argument & 0xFF;
            data.data[1] = 0;
        }
    }
    emit signalToExtModbus(name, data);
}

#define LENGHT_MATH_UNIT 4
// установка единицы измерения матканала
void MainWindow::extSetMathUnit(QString name, uint8_t * data)
{
    char str[LENGHT_MATH_UNIT];
    memcpy(&str, data, LENGHT_MATH_UNIT);
    QString unit = QString(QByteArray(str, LENGHT_MATH_UNIT));

    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        listMath.at(currentMathNum)->setUnit(unit);
    }
    mListMath.unlock();
}
// Получение единицы измерения матканала
void MainWindow::extGetMathUnit(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, LENGHT_MATH_UNIT);
    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        QString unit = listMath.at(currentMathNum)->getUnit();
        memcpy(&data, unit.toUtf8(), LENGHT_MATH_UNIT);
    }
    mListMath.unlock();
    emit signalToExtModbus(name, data);
}

// установка верхнего предела отображения матканала
void MainWindow::extSetMathHiLimit(QString name, uint8_t * data)
{
    float value;
    memcpy(&value, &data, sizeof(value));
    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        listMath.at(currentMathNum)->SetHigherMeasureLimit(value);
    }
    mListMath.unlock();
}
// Получение верхнего предела отображения матканала
void MainWindow::extGetMathHiLimit(QString name)
{
    tModbusBuffer data;
    float value;
    memset(&data, 0, sizeof(value));
    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        value = listMath.at(currentMathNum)->GetHigherMeasureLimit();
        memcpy(&data, &value, sizeof(value));
    }
    mListMath.unlock();
    emit signalToExtModbus(name, data);
}

// установка нижнего предела отображения матканала
void MainWindow::extSetMathLowLimit(QString name, uint8_t * data)
{
    float value;
    memcpy(&value, &data, sizeof(value));
    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        listMath.at(currentMathNum)->SetLowerMeasureLimit(value);
    }
    mListMath.unlock();
}
// Получение нижнего предела отображения матканала
void MainWindow::extGetMathLowLimit(QString name)
{
    tModbusBuffer data;
    float value;
    memset(&data, 0, sizeof(value));
    mListMath.lock();
    if(currentMathNum < listMath.size())
    {
        value = listMath.at(currentMathNum)->GetLowerMeasureLimit();
        memcpy(&data, &value, sizeof(value));
    }
    mListMath.unlock();
    emit signalToExtModbus(name, data);
}

// установка периода обновления матканала
void MainWindow::extSetMathPeriod(QString name, uint8_t * data)
{
    //Vag: пока не реализовано управление этим параметром
}
// Получение периода обновления матканала
void MainWindow::extGetMathPeriod(QString name)
{
    tModbusBuffer data;
    float period = MATH_UPDATE_PERIOD_MS / 1000;
    emit signalToExtModbus(name, data);
}

// Установка типа сигнала аналогого канала
void MainWindow::extSetChannelSignalType(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("SignalType").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        listChannels.at(num-1)->SetSignalType(data[0]);
    }
    mListChannel.unlock();

}
// Получение типа сигнала аналогого канала
void MainWindow::extGetChannelSignalType(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("SignalType").size()).toInt();

    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        uint16_t type = listChannels.at(num-1)->GetSignalType();
        memcpy(&data, &type, sizeof(type));
    }
    mListChannel.unlock();

    emit signalToExtModbus(name, data);
}

// Установка Additional Parametr1 аналогого канала
void MainWindow::extSetChannelAdditionalPar1(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("AdditionalParameter1").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        listChannels.at(num-1)->setAdditionalParametr1(data);
    }
    mListChannel.unlock();
}
// Получение Additional Parametr1 аналогого канала
void MainWindow::extGetChannelAdditionalPar1(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("AdditionalParameter1").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        listChannels.at(num-1)->getAdditionalParametr1(data.data);
    }
    mListChannel.unlock();
    emit signalToExtModbus(name, data);
}

// Установка Additional Parametr2 аналогого канала
void MainWindow::extSetChannelAdditionalPar2(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("AdditionalParameter2").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        listChannels.at(num-1)->setAdditionalParametr2(data);
    }
    mListChannel.unlock();
}
// Получение Additional Parametr2 аналогого канала
void MainWindow::extGetChannelAdditionalPar2(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("AdditionalParameter2").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        listChannels.at(num-1)->getAdditionalParametr2(data.data);
    }
    mListChannel.unlock();
    emit signalToExtModbus(name, data);
}

// Установка поправки к компенсации холодного спая
void MainWindow::extSetChannelCorrectionCj(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("CorrectionCjValue").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        float value;
        memcpy(&value, data, sizeof(value));
        listChannels.at(num-1)->setShiftColdJunction(value);
    }
    mListChannel.unlock();

}
// Получение поправки к компенсации холодного спая
void MainWindow::extGetChannelCorrectionCj(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("CorrectionCjValue").size()).toInt();

    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        float value = listChannels.at(num-1)->getShiftColdJunction();
        memcpy(&data, &value, sizeof(value));
    }
    mListChannel.unlock();

    emit signalToExtModbus(name, data);
}

// Установка нижнего предела измерения
void MainWindow::extSetChannelLowLim(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("LowMeasureLim").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        float value;
        memcpy(&value, data, sizeof(value));
        listChannels.at(num-1)->SetLowerMeasureLimit(value);
    }
    mListChannel.unlock();

}
// Получение нижнего предела измерения
void MainWindow::extGetChannelLowLim(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("LowMeasureLim").size()).toInt();

    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        float value = listChannels.at(num-1)->GetLowerMeasureLimit();
        memcpy(&data, &value, sizeof(value));
    }
    mListChannel.unlock();

    emit signalToExtModbus(name, data);
}

// Установка верхнего предела измерения
void MainWindow::extSetChannelHiLim(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("HighMeasureLim").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        float value;
        memcpy(&value, data, sizeof(value));
        listChannels.at(num-1)->SetHigherMeasureLimit(value);
    }
    mListChannel.unlock();
}
// Получение верхнего предела измерения
void MainWindow::extGetChannelHiLim(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("HighMeasureLim").size()).toInt();

    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        float value = listChannels.at(num-1)->GetHigherMeasureLimit();
        memcpy(&data, &value, sizeof(value));
    }
    mListChannel.unlock();
    emit signalToExtModbus(name, data);
}

#define LENGHT_CHAN_NAME 16
// Получение имени канала
void MainWindow::extGetChannelName(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("Name").size()).toInt();
    memset(&data, 0, LENGHT_CHAN_NAME);
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        QString StrName = listChannels.at(num-1)->GetChannelName();
        memcpy(&data, StrName.toLocal8Bit(), StrName.toLocal8Bit().size());
    }
    mListChannel.unlock();
    emit signalToExtModbus(name, data);
}
// Установка имени канала
void MainWindow::extSetChannelName(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("Name").size()).toInt();
    QString strName = QString(QByteArray((char*)data, LENGHT_CHAN_NAME));
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        listChannels.at(num-1)->SetChannelName(strName);
    }
    mListChannel.unlock();
}

// Установка периода измерения
void MainWindow::extSetChannelPeriod(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("Period").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        float period;
        memcpy(&period, data, sizeof(period));
        listChannels.at(num-1)->SetMeasurePeriod(period);
    }
    mListChannel.unlock();

}
// Получение периода предела измерения
void MainWindow::extGetChannelPeriod(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("Period").size()).toInt();

    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        float period = listChannels.at(num-1)->GetMeasurePeriod();
        memcpy(&data, &period, sizeof(period));
    }
    mListChannel.unlock();

    emit signalToExtModbus(name, data);
}

// Установка числа демпфирования
void MainWindow::extSetChannelDempher(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("Dempher").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        uint16_t dempher;
        memcpy(&dempher, data, sizeof(dempher));
        listChannels.at(num-1)->SetDempher(dempher);
    }
    mListChannel.unlock();

}
// Получение числа демпфирования
void MainWindow::extGetChannelDempher(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("Dempher").size()).toInt();

    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        uint16_t dempher = listChannels.at(num-1)->GetDempherValue() & 0x7FFF;
        memcpy(&data, &dempher, sizeof(dempher));
    }
    mListChannel.unlock();

    emit signalToExtModbus(name, data);
}

// Установка типа регистрации
void MainWindow::extSetChannelTypeRegistration(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("TypeRegistration").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        uint16_t type;
        memcpy(&type, data, sizeof(type));
        listChannels.at(num-1)->SetRegistrationType(type);
    }
    mListChannel.unlock();

}
// Получение типа регистрации
void MainWindow::extGetChannelTypeRegistration(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("TypeRegistration").size()).toInt();

    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        uint16_t type = listChannels.at(num-1)->GetRegistrationType() & 0x7FFF;
        memcpy(&data, &type, sizeof(type));
    }
    mListChannel.unlock();

    emit signalToExtModbus(name, data);
}

// Установка типа отображения
void MainWindow::extSetChannelTypeValue(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("TypeValue").size()).toInt();
    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        uint16_t type;
        memcpy(&type, data, sizeof(type));
        listChannels.at(num-1)->setVolueVoltageType(type);
    }
    mListChannel.unlock();

}
// Получение типа отображения
void MainWindow::extGetChannelTypeValue(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("TypeValue").size()).toInt();

    mListChannel.lock();
    if((num > 0) && (num <= listChannels.size()))
    {
        uint16_t type = listChannels.at(num-1)->getVoltageType() & 0x7FFF;
        memcpy(&data, &type, sizeof(type));
    }
    mListChannel.unlock();

    emit signalToExtModbus(name, data);
}


// Получение режима работы платы
void MainWindow::extGetModuleMode(QString name)
{
    tModbusBuffer data;
    memset(&data, 0, 8);
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();

    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint16_t type = (uint16_t)listDevice.at(num-1)->getMode() & 0x7FFF;
        memcpy(&data, &type, sizeof(type));
    }
    mListDev.unlock();

    emit signalToExtModbus(name, data);
}

// Установка режима работы платы
void MainWindow::extSetModuleMode(QString name, uint8_t * data)
{
    QString module = name.right(name.size() - QString("module").size());//.toInt();
    int num = module.left(1).toInt();
    mListDev.lock();
    if((num > 0) && (num <= listDevice.size()))
    {
        uint16_t mode;
        memcpy(&mode, &data, sizeof(mode));
        listDevice.at(num-1)->setMode(mode);
    }
    mListDev.unlock();
}

// Получение текущей скорости внутреннего MODBUS
void MainWindow::extGetModuleModbusBaud(QString name)
{
    //Vag: пока не реализовано
}
// Установка скорости внутреннего MODBUS
void MainWindow::extSetModuleModbusBaud(QString name, uint8_t * data)
{
    //Vag: пока не реализовано
}

// Получение бита чётности
void MainWindow::extGetModuleModbusParity(QString name)
{
    //Vag: пока не реализовано
}
// Установка бита чётности
void MainWindow::extSetModuleModbusParity(QString name, uint8_t * data)
{
    //Vag: пока не реализовано
}

// Получение количества стоп-битов
void MainWindow::extGetModuleModbusStopBits(QString name)
{
    //Vag: пока не реализовано
}
// Установка количества стоп-битов
void MainWindow::extSetModuleModbusStopBits(QString name, uint8_t * data)
{
    //Vag: пока не реализовано
}

// Получение формата данных MODBUS
void MainWindow::extGetModuleModbusDataOrder(QString name)
{
    //Vag: пока не реализовано
}
// Установка формата данных MODBUS
void MainWindow::extSetModuleModbusDataOrder(QString name, uint8_t * data)
{
    //Vag: пока не реализовано
}

// Установка типа сигнала частотного канала
void MainWindow::extSetFreqSignalType(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("FreqSignalType").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        listFreq.at(num-1)->SetSignalType(data[0]);
    }
    mListFreq.unlock();
}
// Получение типа сигнала частотного канала
void MainWindow::extGetFreqSignalType(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("chan").size());
    int num = chan.left(chan.size() - QString("FreqSignalType").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        uint16_t type = listFreq.at(num-1)->GetSignalType();
        memcpy(&data, &type, sizeof(type));
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}


// Установка Additional Parametr1 частотного канала
void MainWindow::extSetFreqAdditionalParameter(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("AdditionalParameter").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
//        listFreq.at(num-1)->setAdditionalParametr1(data);
    }
    mListFreq.unlock();
}
// Получение Additional Parametr1 частотного канала
void MainWindow::extGetFreqAdditionalParameter(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("AdditionalParameter").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
//        listFreq.at(num-1)->getAdditionalParametr1(data.data);
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}

// Установка длительности единиченого импульса
void MainWindow::extSetFreqImpulseDuration(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("ImpulseDuration").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value;
        memcpy(&value, data, sizeof(value));
        listFreq.at(num-1)->setImpulseDuration(value);
    }
    mListFreq.unlock();
}
// Получение длительности единиченого импульса
void MainWindow::extGetFreqImpulseDuration(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("ImpulseDuration").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value = listFreq.at(num-1)->GetImpulseDuration();
        memcpy(data.data, &value, sizeof(value));
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}

// Установка веса импульса
void MainWindow::extSetFreqImpWeight(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("ImpWeight").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value;
        memcpy(&value, data, sizeof(value));
        listFreq.at(num-1)->setImpulseWeight(value);
    }
    mListFreq.unlock();
}
// Получение веса импульса
void MainWindow::extGetFreqImpWeight(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("ImpWeight").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value = listFreq.at(num-1)->getImpulseWeight();
        memcpy(data.data, &value, sizeof(value));
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}

// Установка нижнего предела измеряемого частотного сигнала
void MainWindow::extSetFreqLowMeasureLim(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("LowMeasureLim").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value;
        memcpy(&value, data, sizeof(value));
        listFreq.at(num-1)->SetLowerMeasureLimit(value);
    }
    mListFreq.unlock();
}
// Получение нижнего предела измеряемого частотного сигнала
void MainWindow::extGetFreqLowMeasureLim(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("LowMeasureLim").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value = listFreq.at(num-1)->GetLowerMeasureLimit();
        memcpy(data.data, &value, sizeof(value));
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}

// Установка верхнего предела измеряемого частотного сигнала
void MainWindow::extSetFreqHighMeasureLim(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("HighMeasureLim").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value;
        memcpy(&value, data, sizeof(value));
        listFreq.at(num-1)->SetHigherMeasureLimit(value);
    }
    mListFreq.unlock();
}
// Получение верхнего предела измеряемого частотного сигнала
void MainWindow::extGetFreqHighMeasureLim(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("HighMeasureLim").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value = listFreq.at(num-1)->GetHigherMeasureLimit();
        memcpy(data.data, &value, sizeof(value));
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}

// Установка периода обновления измерения
void MainWindow::extSetFreqPeriod(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("Period").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value;
        memcpy(&value, data, sizeof(value));
        listFreq.at(num-1)->SetMeasurePeriod(value);
    }
    mListFreq.unlock();
}
// Получение периода обновления измерения
void MainWindow::extGetFreqPeriod(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("Period").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        float value = listFreq.at(num-1)->getMeasurePeriod();
        memcpy(data.data, &value, sizeof(value));
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}

// Установка числа демпфирования
void MainWindow::extSetFreqDempher(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("Dempher").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        uint16_t value;
        memcpy(&value, data, sizeof(value));
        listFreq.at(num-1)->setDempher(value);
    }
    mListFreq.unlock();
}
// Получение числа демпфирования
void MainWindow::extGetFreqDempher(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("Dempher").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        uint16_t value = listFreq.at(num-1)->getDempher();
        memcpy(data.data, &value, sizeof(value));
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}

// Установка типа отображения измерения
void MainWindow::extSetFreqTypeValue(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("TypeValue").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        uint16_t value;
        memcpy(&value, data, sizeof(value));
//        listFreq.at(num-1)->setTypeValue(value); //Vag: пока не реализовано
    }
    mListFreq.unlock();
}
// Получение типа отображения измерения
void MainWindow::extGetFreqTypeValue(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("TypeValue").size()).toInt();
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        //Vag: пока не реализовано
//        uint16_t value = listFreq.at(num-1)->getTypeValue();
//        memcpy(data, &value, sizeof(value));
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}

#define LENGHT_FREQ_NAME 8
// Получение имени частотного канала
void MainWindow::extGetFreqName(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("Name").size()).toInt();
    memset(&data, 0, LENGHT_FREQ_NAME);
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        QString StrName = listFreq.at(num-1)->GetChannelName();
        memcpy(&data, StrName.toLocal8Bit(), StrName.toLocal8Bit().size());
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}
// Установка имени частотного канала
void MainWindow::extSetFreqName(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("Name").size()).toInt();
    QString strName = QString(QByteArray((char*)data, LENGHT_FREQ_NAME));
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        listFreq.at(num-1)->setName(strName);
    }
    mListFreq.unlock();
}

#define LENGHT_FREQ_UNIT 4
// Получение имени частотного канала
void MainWindow::extGetFreqUnit(QString name)
{
    tModbusBuffer data;
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("Unit").size()).toInt();
    memset(&data, 0, LENGHT_FREQ_UNIT);
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        QString StrName = listFreq.at(num-1)->getUnit();
        memcpy(&data, StrName.toLocal8Bit(), StrName.toLocal8Bit().size());
    }
    mListFreq.unlock();
    emit signalToExtModbus(name, data);
}
// Установка имени частотного канала
void MainWindow::extSetFreqUnit(QString name, uint8_t * data)
{
    QString chan = name.right(name.size() - QString("freq").size());
    int num = chan.left(chan.size() - QString("Unit").size()).toInt();
    QString strName = QString(QByteArray((char*)data, LENGHT_FREQ_UNIT));
    mListFreq.lock();
    if((num > 0) && (num <= listFreq.size()))
    {
        listFreq.at(num-1)->setUnit(strName);
    }
    mListFreq.unlock();
}
