#ifndef MODBUS_LOOKUP_TABLE_H
#define MODBUS_LOOKUP_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif
//#include "types.h"
#include <stdint.h>
#include <stddef.h>
#include "device_params.h"

// Structure Definitions
typedef struct
{
    uint16_t  Offset;        // This is the Modbus location. This is the 0 based value.
                            // ie ModbusMap Offset = 101, this will be 100.
    uint16_t  IpcIndex;      // Index for the IPC parameters and I/O Processor Parameters. >255
                            // are IPC and are defined in the ipc_indexes.h shared header file.
    uint8_t   BitMask;       // Bit mask for the U8 parameters. All coils are U8's.
    uint8_t   Access;        // Access permissions based on mode
} tExtLookupCoils;

// pointer to function which is used for handling request
typedef uint8_t (*tCmdExecuteFuncPtr)(void* param, void* buffer);
typedef struct
{
    uint16_t            offset;           // This is the Modbus location. This is the 0 based value.
                                          // ie ModbusMap Offset = 101, this will be 100.
    uint8_t             typeMapping;      // Тип данных в карте регистров
    tCmdExecuteFuncPtr  cmdFuncPtr;       // W/R check function
    uint16_t            paramOffset;      // Parameter name (OFFSET in the parameter structure)
    const char*         nameParam;        // Parameter name string
    uint8_t             type;             // Float, U8, U16, U32, ASCII, etc.

    uint8_t             size;             // Number of bytes in parameter. U8 = 1, Float = 4, ASCII size
                                            //  depends on parameter.
//    uint8_t             access;           // Access permissions based on mode
} tExtLookupRegisters;

// Type Mapping
#define LKUP_MAP_DISCRETE              0
#define LKUP_MAP_COILS                 1
#define LKUP_MAP_INPUTS                2
#define LKUP_MAP_HILDING               3

// Parameter Type
#define LKUP_TYPE_U8                   0
#define LKUP_TYPE_U16                  1
#define LKUP_TYPE_U32                  2
#define LKUP_TYPE_FLOAT                3
#define LKUP_TYPE_ASCII                4

// Parameter Size
#define LKUP_SIZE_U8                   1
#define LKUP_SIZE_U16                  2
#define LKUP_SIZE_U32                  4
#define LKUP_SIZE_FLOAT                4
#define LKUP_SIZE_ASCII                8

// Coil Mask
#define LKUP_COIL_MASK_BIT0  (uint8_t)0x01
#define LKUP_COIL_MASK_BIT1  (uint8_t)0x02
#define LKUP_COIL_MASK_BIT2  (uint8_t)0x04
#define LKUP_COIL_MASK_BIT4  (uint8_t)0x10
#define LKUP_COIL_MASK_BIT5  (uint8_t)0x20

extern const tExtLookupRegisters g_extModbusLookupRegisters[];
extern const uint16_t numLookupRegisters;
extern tExtDeviceBasicParams g_extModbusDataStorage;

#ifdef __cplusplus
}
#endif

#endif // MODBUS_LOOKUP_TABLE_H
