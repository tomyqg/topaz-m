#include <QTimer>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defines.h"
#include "Channels/channelOptions.h"
#include "device_slot.h"
#include "Channels/group_channels.h"

extern QList<cDevice*> listDevice;
extern QList<ChannelOptions *> listChannels;
extern QList<cMathChannel *> listMath;
extern QList<Ustavka *> listUstavok;
extern QList<cRelay*> listRelais;
extern QList<cSteel*> listSteel;
//extern QList<cGroupChannels*> listGroup;
extern cIpController * ethernet;
extern cSystemOptions systemOptions;
extern QMutex mSysOpt;


void MainWindow::initExtInterface()
{
    /* Список функций обновления данных */
    // Аналоговые каналы
    for(int i = 0; i < TOTAL_NUM_CHAN; i++)
    {
        QString name = "analogChan" + QString::number(i+1);
        tablExtInterfaceChannels.append({name, &MainWindow::extGetChannel});
    }

    // Математические каналы
    for(int i = 0; i < TOTAL_NUM_MATH; i++)
    {
        QString name = "mathChan" + QString::number(i+1);
        tablExtInterfaceChannels.append({name, &MainWindow::extGetChannel});
    }

    // Таблица (список) функций применения новых параметров
    tablSetParamExtInterface.append({"displayMode", &MainWindow::extGetDisplayMode, &MainWindow::extSetDisplayMode});


    /* Запуск таймеров */
    // Отправка данных с периодом 0,5 сек
    connect(&timerExtInterface, SIGNAL(timeout()), this, SLOT(updateExtIntefaceData()));
    timerExtInterface.start(500);


}

// Отправка актуальных данных во внешние интерфейсы
void MainWindow::updateExtIntefaceData()
{

    foreach (typeTableExtInterface Ch, tablExtInterfaceChannels) {
        (this->*Ch.funcGet)(Ch.name);
    }

    foreach (typeTableSetParamExtInterface par, tablSetParamExtInterface) {
        (this->*par.funcGet)(par.name);
    }

//    tModbusBuffer data;
//    uint16_t dataUint16 = systemOptions.display;
//    memcpy(&data, &dataUint16, sizeof(dataUint16));
//    emit signalToExtModbus("displayMode", data);
}


void MainWindow::slotFromExtModbus(QString name, tModbusBuffer data)
{
    applyParam(name, (uint8_t*) &data);
}

// Функция применения новых параметров
void MainWindow::applyParam(QString name, uint8_t * data)
{
//    if(name == "displayMode")
//    {
//        mSysOpt.lock();
//        systemOptions.display = data.data[0];
//        mSysOpt.unlock();
//    }

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
            fl = (float)listChannels.at(chan-1)->GetCurrentChannelValue();
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

    emit signalToExtModbus(name, data);
}

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
