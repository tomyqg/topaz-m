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
//    deviceparametrs Data;
//    deviceparametrs DataFlags;
//    deviceparametrs Status;
//    deviceparametrs Error;
//    deviceparametrs Quantity;
//    deviceparametrs Uptime;
//    deviceparametrs RawData;
//    deviceparametrs RawDataFlags;
//    deviceparametrs SupportedSignals;
//    deviceparametrs SignalType;
//    deviceparametrs AdditionalParameter1;
//    deviceparametrs AdditionalParameter2;
//    deviceparametrs FilterType;
//    deviceparametrs FilterLength;
//    deviceparametrs TransferFunction;
//    deviceparametrs TransferSignalLowLim;
//    deviceparametrs TransferSignalHighLim;
//    deviceparametrs TransferScaleLowLim;
//    deviceparametrs TransferScaleHighLim;
//    deviceparametrs BadGoodComm;
//    deviceparametrs UserCalibGain1;
//    deviceparametrs UserCalibOffset1;
//    deviceparametrs UserCalibDate1;
//    deviceparametrs UserCalibGain2;
//    deviceparametrs UserCalibOffset2;
//    deviceparametrs UserCalibDate2;
//    deviceparametrs UserCalibGain3;
//    deviceparametrs UserCalibOffset3;
//    deviceparametrs UserCalibDate3;
//    deviceparametrs UserCalibGain4;
//    deviceparametrs UserCalibOffset4;
//    deviceparametrs UserCalibDate4;
//    deviceparametrs FactoryCalibGain1;
//    deviceparametrs FactoryCalibOffset1;
//    deviceparametrs FactoryCalibDate1;
//    deviceparametrs FactoryCalibGain2;
//    deviceparametrs FactoryCalibOffset2;
//    deviceparametrs FactoryCalibDate2;
//    deviceparametrs FactoryCalibGain3;
//    deviceparametrs FactoryCalibOffset3;
//    deviceparametrs FactoryCalibDate3;
//    deviceparametrs FactoryCalibGain4;
//    deviceparametrs FactoryCalibOffset4;
//    deviceparametrs FactoryCalibDate4;
//    deviceparametrs CjValue;
//    deviceparametrs ResultCjValue;
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
