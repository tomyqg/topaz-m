#ifndef CDEVICE_H
#define CDEVICE_H

#include <QObject>
#include <QTimer>
#include <stdint.h>
#include "registersmap.h"
#include "transaction.h"



class cDevice : public QObject
{
    Q_OBJECT
public:
    explicit cDevice(QObject *parent = 0);
    static int countDev;
    bool getOnline() { return online; }
    void setOnline() { online = true; }
    void pause(bool f) { pauseUpdateParam = f; }
    int getSlot() { return slot; }  // номер слота, к которому относится объект устройства
    uint16_t protocolVersion;       // версия протокола
    uint16_t hardwareVersion;       // версия аппаратного обеспечения
    uint16_t softwareVersion;       // версия ПО
    deviceTypeEnum deviceType;      // тип устройства
    uint32_t serialNumber;          // серийный номер платы
    uint32_t uptime;                // время работы с момента включения
//    deviceStateEnum deviceState;    // состояние устройства
    accessTypeEnum accessType;      // уровень доступа
    uint32_t mbCommCount;           // количество обработанных запросов
    uint32_t mbCommError;           // количество возникших ошибок в Modbus
    deviceStatusEnum deviceStatus;  // статус устройства
    uint16_t devErrors;             // список ошибок платы
    uint32_t factoryDate;           // дата производства
    uint32_t softwareCrc32;         // контролльная сумма ПО
    uint8_t uniqueId[12];           // уникальный идентификатор контроллера
    uint16_t root_Access;           // регистр доступа ROOT
    uint16_t deviceMode;            // режим работы платы
    int parseDeviceParam(Transaction tr);
    bool getStable() { return stableOnline; }
    void setMode(int m);
    int getMode() { return deviceMode; }

signals:
    void updateParam(Transaction tr);

public slots:

private slots:
    void resetOnline();
    void updateStatus();
    void updateConstParam();

private:
    bool online;                    // признак наличия платы в слоте
    uint8_t slot;                       // номер слота
    QTimer * timerResetOnline;      // таймер принятия решения об отсутствии платы
    QTimer * timerUpdateStatus;     // таймер запросов стауса устройства
    QTimer * timerUpdateConstParam; // таймер обновления параметров, постоянных для текущей платы
    bool pauseUpdateParam;          // пауза обновления параметров
    bool stableOnline;              // признак стабильного признакак онлайн/оффлайн
    int counterStatus;              // счётчик статусов для определения стабильности
    int countParams;                // счетчик для перебора запрашиваемых параметров
};

#endif // CDEVICE_H
