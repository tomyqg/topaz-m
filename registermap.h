#ifndef REGISTERMAP_H
#define REGISTERMAP_H

#include <QObject>

class RegisterMap
{
public:
    enum StorageType
    {
        D  = 0,
        S  = 1,
        NV = 2,
    };

    enum ParamType
    {
        U16 = 0,
        U32 = 1,
        F32 = 2,
        A12 = 3
    };

    enum RegisterType
    {
        HoldingReg = 0,
        InputReg = 1
    };

    enum WorkLevelAccess
    {
        R  = 0,
        W  = 1,
        RW = 2,
    };

    typedef struct {
        QString name;
        uint16_t Offset;
        uint8_t StorageType;
        uint8_t ParamType;
        uint8_t WorkLevelAccess;
        uint8_t RegisterType;
    } registr_t;

    enum {SIZE_RM = 33};
};

const RegisterMap::registr_t rm[] =
{
    //----------------------- Device ------------------------

    {"protocolVersion",         16384, RegisterMap::S,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},
    {"hardwareVersion",         16385, RegisterMap::S,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},

    {"deviceType",              16388, RegisterMap::S,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},
    {"modbusAddress",           16389, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},

    {"deviceStatus",            16406, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},

    {"factoryDate",             16408, RegisterMap::NV, RegisterMap::U32, RegisterMap::R,  RegisterMap::HoldingReg},

    {"onBoardTemp",             16419, RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::HoldingReg},
    {"onBoardVoltage",          16421, RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::HoldingReg},

    {"deviceModel",             16421, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},

    //--------------------- Channel 0 -----------------------

    {"chan0Data",               0,     RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::InputReg},

    {"chan0Status",             32771, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},

    {"chan0RawData",            32777, RegisterMap::D,  RegisterMap::U32, RegisterMap::R,  RegisterMap::HoldingReg},

    {"chan0SupportedSignals",   32780, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},
    {"chan0SignalType",         32781, RegisterMap::NV, RegisterMap::U16, RegisterMap::RW, RegisterMap::HoldingReg},

    {"chan0TransferSignalLowLim",    32799, RegisterMap::NV, RegisterMap::F32, RegisterMap::RW, RegisterMap::HoldingReg},
    {"chan0TransferSignalHighLim",   32801, RegisterMap::NV, RegisterMap::F32, RegisterMap::RW, RegisterMap::HoldingReg},

    {"chan0CjValue",            32858, RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::HoldingReg},

    //--------------------- Channel 1 -----------------------

    {"chan1Data",               2,     RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::InputReg},

    {"chan1Status",             32899, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},

    {"chan1RawData",            32905, RegisterMap::D,  RegisterMap::U32, RegisterMap::R,  RegisterMap::HoldingReg},

    {"chan1SupportedSignals",   32908, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},
    {"chan1SignalType",         32909, RegisterMap::NV, RegisterMap::U16, RegisterMap::RW, RegisterMap::HoldingReg},

    {"chan1TransferSignalLowLim",    32927, RegisterMap::NV, RegisterMap::F32, RegisterMap::RW, RegisterMap::HoldingReg},
    {"chan1TransferSignalHighLim",   32929, RegisterMap::NV, RegisterMap::F32, RegisterMap::RW, RegisterMap::HoldingReg},

    {"chan1CjValue",            32986, RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::HoldingReg},

    //--------------------- Channel 2 -----------------------

    {"chan2Data",               4,     RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::InputReg},

    {"chan2Status",             33027, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},

    {"chan2RawData",            33033, RegisterMap::D,  RegisterMap::U32, RegisterMap::R,  RegisterMap::HoldingReg},

    {"chan2SupportedSignals",   33036, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},
    {"chan2SignalType",         33037, RegisterMap::NV, RegisterMap::U16, RegisterMap::RW, RegisterMap::HoldingReg},

    {"chan2TransferSignalLowLim",    33055, RegisterMap::NV, RegisterMap::F32, RegisterMap::RW, RegisterMap::HoldingReg},
    {"chan2TransferSignalHighLim",   33057, RegisterMap::NV, RegisterMap::F32, RegisterMap::RW, RegisterMap::HoldingReg},

    {"chan2CjValue",            33114, RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::HoldingReg},

    //--------------------- Channel 3 -----------------------

    {"chan3Data",               6,     RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::InputReg},

    {"chan3Status",             33155, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},

    {"chan3RawData",            33161, RegisterMap::D,  RegisterMap::U32, RegisterMap::R,  RegisterMap::HoldingReg},

    {"chan3SupportedSignals",   33164, RegisterMap::D,  RegisterMap::U16, RegisterMap::R,  RegisterMap::HoldingReg},
    {"chan3SignalType",         33165, RegisterMap::NV, RegisterMap::U16, RegisterMap::RW, RegisterMap::HoldingReg},

    {"chan3TransferSignalLowLim",    33183, RegisterMap::NV, RegisterMap::F32, RegisterMap::RW, RegisterMap::HoldingReg},
    {"chan3TransferSignalHighLim",   33185, RegisterMap::NV, RegisterMap::F32, RegisterMap::RW, RegisterMap::HoldingReg},

    {"chan3CjValue",            33242, RegisterMap::D,  RegisterMap::F32, RegisterMap::R,  RegisterMap::HoldingReg}

};



#endif // REGISTERMAP_H
