#include <stdint.h>
#include "modbus_lookup_tables.h"

#define OFFSET(field) ( (uint16_t)offsetof(tExtDeviceBasicParams, field) )

// This is main storage for device parameters packed by 1 byte
#pragma pack(push, 1)
tExtDeviceBasicParams g_extModbusDataStorage = {0};
#pragma pack(pop)


extern uint8_t mbExtRegRwCheckFunc(void* param, void* buffer);


// Register Map Definition
const tExtLookupRegisters g_extModbusLookupRegisters[] =
{
    // {Par Offset, typeMapping, Check Func, Parameter Offset, Data Type, Data Size}

    // Parameter: Modbus Protocol Version : modbusProtocolVersion
    {0, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(modbusProtocolVersion), "modbusProtocolVersion", LKUP_TYPE_U32, 4},
    // Parameter: Software Version : softwareVersion
    {102, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(softwareVersion), "softwareVersion", LKUP_TYPE_U32, 4},
    // Parameter: Device Type : deviceType
    {104, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(deviceType), "deviceType", LKUP_TYPE_U16, 2},
    // Parameter: Device Name : deviceName
    {105, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(deviceName), "deviceName", LKUP_TYPE_ASCII, 32},
    // Parameter: Analog channel 1 : analogChan1
    {5000, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan1), "analogChan1", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 2 : analogChan2
    {5002, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan2), "analogChan2", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 3 : analogChan3
    {5004, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan3), "analogChan3", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 4 : analogChan4
    {5006, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan4), "analogChan4", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 5 : analogChan5
    {5008, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan5), "analogChan5", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 6 : analogChan6
    {5010, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan6), "analogChan6", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 7 : analogChan7
    {5012, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan7), "analogChan7", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 8 : analogChan8
    {5014, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan8), "analogChan8", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 9 : analogChan9
    {5016, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan9), "analogChan9", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 10 : analogChan10
    {5018, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan10), "analogChan10", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 11 : analogChan11
    {5020, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan11), "analogChan11", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 12 : analogChan12
    {5022, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan12), "analogChan12", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 13 : analogChan13
    {5024, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan13), "analogChan13", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 14 : analogChan14
    {5026, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan14), "analogChan14", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 15 : analogChan15
    {5028, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan15), "analogChan15", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 16 : analogChan16
    {5030, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan16), "analogChan16", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 17 : analogChan17
    {5016, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan17), "analogChan17", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 18 : analogChan18
    {5018, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan18), "analogChan18", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 19 : analogChan19
    {5020, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan19), "analogChan19", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 20 : analogChan20
    {5022, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan20), "analogChan20", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 21 : analogChan21
    {5024, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan21), "analogChan21", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 22 : analogChan22
    {5026, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan22), "analogChan22", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 23 : analogChan23
    {5028, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan23), "analogChan23", LKUP_TYPE_FLOAT, 4},
    // Parameter: Analog channel 24 : analogChan24
    {5030, LKUP_MAP_INPUTS, mbExtRegRwCheckFunc, OFFSET(analogChan24), "analogChan24", LKUP_TYPE_FLOAT, 4},

    //...

    // Parameter: Display Mode : displayMode
    {100, LKUP_MAP_HILDING, mbExtRegRwCheckFunc, OFFSET(displayMode), "displayMode", LKUP_TYPE_U16, 2},
    // Parameter: Brightness : brightness
    {103, LKUP_MAP_HILDING, mbExtRegRwCheckFunc, OFFSET(brightness), "brightness", LKUP_TYPE_U16, 2},
};
const uint16_t numLookupRegisters = (sizeof(g_extModbusLookupRegisters)/sizeof(g_extModbusLookupRegisters[0]));

