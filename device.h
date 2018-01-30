#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QDebug>
#include "registermap.h"

class deviceparametrs
{
public:
    explicit deviceparametrs(RegisterMap::registr_t reg)
    {
        name = reg.name;
        Offset = reg.Offset;
        StorageType = reg.StorageType;
        ParamType = reg.ParamType;
        WorkLevelAccess = reg.WorkLevelAccess;
        RegisterType = reg.RegisterType;
    }

public:
    QString name;
    uint16_t Offset;
    uint8_t StorageType;
    uint8_t ParamType;
    uint8_t WorkLevelAccess;
    uint8_t RegisterType;
};

class channelparametrs
{
//public:
//    explicit channelparametrs();

public:
    QList <deviceparametrs> parametrs;
    int BaseAddress;
    int AddressOffset;
};

class Device
{
public:
    Device()
    {
        int size = sizeof(rm)/sizeof(RegisterMap::registr_t);
        for(int i = 0; i < size; i++)
        {
            RegisterMap::registr_t r = rm[i];
            deviceparametrs parametr(r);
            if(r.name.contains("chan0"))
            {
                channel0.parametrs.append(parametr);
            }
            else if(r.name.contains("chan1"))
            {
                channel1.parametrs.append(parametr);
            }
            else if(r.name.contains("chan2"))
            {
                channel2.parametrs.append(parametr);
            }
            else if(r.name.contains("chan3"))
            {
                channel3.parametrs.append(parametr);
            }
            else
            {
                parametrs.append(parametr);
            }
        }
        channels.append(channel0);
        channels.append(channel1);
        channels.append(channel2);
        channels.append(channel3);
    }

    channelparametrs channel0;
    channelparametrs channel1;
    channelparametrs channel2;
    channelparametrs channel3;

    QList <channelparametrs> channels;
    QList <deviceparametrs> parametrs;

    deviceparametrs getDevParam(uint16_t addr);

    enum StorageType { D  = 0,
                       S  = 1,
                       NV = 2,
                     };

    enum ParamType { U16 = 0,
                     U32 = 1,
                     F32 = 2,
                     A12 = 3
                   };

    enum RegisterType { HoldingReg = 0,
                        InputReg = 1
                      };

    enum WorkLevelAccess{ R  = 0,
                          W  = 1,
                          RW = 2,
                        };
};

#endif // DEVICE_H
