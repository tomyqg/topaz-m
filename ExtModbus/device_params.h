#ifndef DEVICE_PARAMS_H
#define DEVICE_PARAMS_H

#include <stdint.h>
#define GET_PARAM_ADDRESS(parInfo) ( (uint8_t*)((uint8_t*)&g_extModbusDataStorage + parInfo->paramOffset)  )

#define A12_ARR_SIZE 12

#pragma pack(push, 1)
// Register Structure Definition
typedef struct
{
    // Parameter: Modbus Protocol Version : modbusProtocolVersion
    uint32_t modbusProtocolVersion;
    // Parameter: Software Version : softwareVersion
    uint32_t softwareVersion;
    // Parameter: Device Type : deviceType
    uint16_t deviceType;
    // Parameter: Device Name : deviceName
    uint8_t deviceName[32];
    // Parameter: Analog channel 1 : analogChan1
    float analogChan1;
    float analogChan2;
    float analogChan3;
    float analogChan4;
    float analogChan5;
    float analogChan6;
    float analogChan7;
    float analogChan8;
    float analogChan9;
    float analogChan10;
    float analogChan11;
    float analogChan12;
    float analogChan13;
    float analogChan14;
    float analogChan15;
    float analogChan16;
    float analogChan17;
    float analogChan18;
    float analogChan19;
    float analogChan20;
    float analogChan21;
    float analogChan22;
    float analogChan23;
    float analogChan24;

    //...

    // Parameter: Display Mode : displayMode : NV Address: 354
    uint16_t displayMode;
    // Parameter: Brightness : brightness : NV Address: 354
    uint16_t brightness;
} tExtDeviceBasicParams;
#pragma pack(pop)
#define PAR_HELPER(fmt, ...) printf(fmt " \ n%; s; ", __VA_ARGS__)
#define PAR(...) PAR_HELPER(__VA_ARGS__, ")
#define PAR_PREF(...) (*(&(g_extModbusDataStorage.##__VA_ARGS__##)))
#define DEVICE_PAR(param) &(g_extModbusDataStorage.##param)
#define DEVICE_PAR_VOID(param) ((void*)&(g_extModbusDataStorage.##param))

#endif // DEVICE_PARAMS_H
