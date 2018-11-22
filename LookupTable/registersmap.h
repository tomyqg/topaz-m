#ifndef CREGISTERSMAP_H
#define CREGISTERSMAP_H

#include <QObject>
#include "lookup_tables.h"

#define MAGIC_BUFFER_END    0x7FFF

enum termoCoupleType{
    TC_Type_S = 0,
    TC_Type_K = 1,
    TC_Type_L = 2,
    TC_Type_B = 3,
    TC_Type_A1 = 4,
    TC_Type_J = 5,
    TC_Type_N = 6,
};
enum deviceTypeEnum{
    Device_4AI = 1,
    Device_8RP = 2,
    Device_STEEL = 3,
};
enum deviceStateEnum{
    Device_State_OK = 0,
    Device_State_ErrModbus = 1,
    Device_State_ErrChannel = 4,
    Device_State_ErrCallibr = 5
};
enum accessTypeEnum{
    Access_User = 2,
    Access_Root = 3
};
enum deviceStatusEnum{
    Device_Status_NOINIT = 0,   //запуск, самодиагностика
    Device_Status_CONFIG = 1,   //конфигурирование каналов
    Device_Status_EXECUTE = 2,  //рабочий режим
    Device_Status_IDLE = 3,     //простаивание
    Device_Status_ERROR = 4     //ошибка устройства на этапе запуска
};


enum statusChannelEnum{
    StatusCh_Off = 0,                       //выключен
    StatusCh_On = 1,                        //включен
    StatusCh_WaitConf = 2,                  //ожидает конфигурации
    StatusCh_Configuration = 3,             //выполняет процесс конфигурации
    StatusCh_EndConfig = 4,                 //конфигурация завершена
    StatusCh_SteelWaitData = 5,             //ожидание обновления значения параметра Data(STEEL)
    StatusCh_SteelUpdateData = 6,           //обработка обновления значения параметра Data(STEEL)
    StatusCh_DataReady = 7,                 //значение параметра Data обновлено
    StatusCh_SteelErrorTC = 8,              //ошибка термопары(STEEL)
    StatusCh_SteelErrorEds = 9,             //ошибка датчика окисленности(STEEL)
    StatusCh_SteelNotFoundSquareTemp = 10,  //не найдена площадка по температуре(STEEL)
    StatusCh_SteelNotFoundSquareEds = 11,   //не найдена площадка по окисленности(STEEL)
    StatusCh_SteelNotFoundSquares = 12,     //площадки не найдены(STEEL)
    StatusCh_SteelSquaresOK = 13,           //площадки найдены(STEEL)
};



class cRegistersMap : public QObject
{
    Q_OBJECT
public:
    explicit cRegistersMap(QObject *parent = 0);
    static uint16_t getOffsetByName(QString name);
    static QString getNameByOffset(uint16_t offset);
    static tLookupRegisters getDpByOffset(uint16_t offset);
signals:

public slots:
};

#endif // CREGISTERSMAP_H
