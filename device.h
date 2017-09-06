#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QDebug>

struct deviceparametrs
{
    QString name;
    uint16_t Offset;
    uint8_t StorageType;
    uint8_t ParamType;
    uint8_t WorkLevelAccess;
    uint8_t RegisterType;
};

struct channelparametrs
{
    int BaseAddress;
    int AddressOffset;
    deviceparametrs Data;
    deviceparametrs DataFlags;
    deviceparametrs Status;
    deviceparametrs Error;
    deviceparametrs Quantity;
    deviceparametrs Uptime;
    deviceparametrs RawData;
    deviceparametrs RawDataFlags;
    deviceparametrs SupportedSignals;
    deviceparametrs SignalType;
    deviceparametrs AdditionalParameter1;
    deviceparametrs AdditionalParameter2;
    deviceparametrs FilterType;
    deviceparametrs FilterLength;
    deviceparametrs TransferFunction;
    deviceparametrs TransferSignalLowLim;
    deviceparametrs TransferSignalHighLim;
    deviceparametrs TransferScaleLowLim;
    deviceparametrs TransferScaleHighLim;
    deviceparametrs BadGoodComm;
    deviceparametrs UserCalibGain1;
    deviceparametrs UserCalibOffset1;
    deviceparametrs UserCalibDate1;
    deviceparametrs UserCalibGain2;
    deviceparametrs UserCalibOffset2;
    deviceparametrs UserCalibDate2;
    deviceparametrs UserCalibGain3;
    deviceparametrs UserCalibOffset3;
    deviceparametrs UserCalibDate3;
    deviceparametrs UserCalibGain4;
    deviceparametrs UserCalibOffset4;
    deviceparametrs UserCalibDate4;
    deviceparametrs FactoryCalibGain1;
    deviceparametrs FactoryCalibOffset1;
    deviceparametrs FactoryCalibDate1;
    deviceparametrs FactoryCalibGain2;
    deviceparametrs FactoryCalibOffset2;
    deviceparametrs FactoryCalibDate2;
    deviceparametrs FactoryCalibGain3;
    deviceparametrs FactoryCalibOffset3;
    deviceparametrs FactoryCalibDate3;
    deviceparametrs FactoryCalibGain4;
    deviceparametrs FactoryCalibOffset4;
    deviceparametrs FactoryCalibDate4;
    deviceparametrs CjValue;
    deviceparametrs ResultCjValue;
};

class Device
{
public:
    Device()
    {
        // constructor blt

        modbusAddress.Offset = 16389;
        modbusAddress.StorageType = Device::D;
        modbusAddress.ParamType = Device::U16;
        modbusAddress.RegisterType = Device::HoldingReg;
        modbusAddress.WorkLevelAccess = Device::R;
        modbusAddress.name = "modbusAddress";

        deviceStatus.Offset = 16406;
        deviceStatus.StorageType = Device::D;
        deviceStatus.ParamType = Device::U16;
        deviceStatus.RegisterType = Device::HoldingReg;
        deviceStatus.WorkLevelAccess = Device::R;
        deviceStatus.name = "deviceStatus";


        elmetroAB1.Offset = 0;
        elmetroAB1.StorageType = Device::D;
        elmetroAB1.ParamType = Device::F32;
        elmetroAB1.WorkLevelAccess = Device::R;
        elmetroAB1.RegisterType = Device::InputReg;
        elmetroAB1.name = "elmetroAB1";


        badgoodcomm.Offset = 32935;
        badgoodcomm.StorageType = Device::D;
        badgoodcomm.ParamType = Device::F32;
        badgoodcomm.WorkLevelAccess = Device::RW;
        badgoodcomm.RegisterType = Device::HoldingReg;
        badgoodcomm.name = "badgoodcomm";

        channel0.BaseAddress = 32768;
        channel0.AddressOffset = 0;

        channel0.Data.Offset = 0 ; // 32768
        channel0.Data.StorageType = Device::D;
        channel0.Data.ParamType = Device::F32;
        channel0.Data.WorkLevelAccess = Device::R;
        channel0.Data.RegisterType = Device::InputReg;
        channel0.Data.name = "chan0.chanData";

        channel0.Status.Offset = 32771;
        channel0.Status.StorageType = Device::D;
        channel0.Status.ParamType = Device::U16;
        channel0.Status.RegisterType = Device::HoldingReg;
        channel0.Status.WorkLevelAccess = Device::R;
        channel0.Status.name = "chan0.chanStatus";


        channel0.SupportedSignals.Offset = 32780;
        channel0.SupportedSignals.StorageType = Device::D;
        channel0.SupportedSignals.ParamType = Device::U16;
        channel0.SupportedSignals.RegisterType = Device::HoldingReg;
        channel0.SupportedSignals.WorkLevelAccess = Device::R;
        channel0.SupportedSignals.name = "chan0SupportedSignals";

        channel0.SignalType.Offset = 32781;
        channel0.SignalType.StorageType = Device::NV;
        channel0.SignalType.ParamType = Device::U16;
        channel0.SignalType.WorkLevelAccess = Device::RW;
        channel0.SignalType.name = "chan0SignalType";

        channel0.AdditionalParameter1.Offset = 32782;
        channel0.AdditionalParameter1.StorageType = Device::NV;
        channel0.AdditionalParameter1.ParamType = Device::A12;
        channel0.AdditionalParameter1.RegisterType = Device::HoldingReg;
        channel0.AdditionalParameter1.WorkLevelAccess = Device::RW;
        channel0.AdditionalParameter1.name = "chan0AdditionalParameter1";

        channel0.AdditionalParameter2.Offset = 32788;
        channel0.AdditionalParameter2.StorageType = Device::NV;
        channel0.AdditionalParameter2.ParamType = Device::A12;
        channel0.AdditionalParameter2.RegisterType = Device::HoldingReg;
        channel0.AdditionalParameter2.WorkLevelAccess = Device::RW;
        channel0.AdditionalParameter2.name = "chan0AdditionalParameter2";

        channel0.UserCalibGain1.Offset = 32935; // по идее должен быть адрес 32810 но он в фрам, мы не хотим убить её
        channel0.UserCalibGain1.StorageType = Device::NV;
        channel0.UserCalibGain1.ParamType = Device::F32;
        channel0.UserCalibGain1.WorkLevelAccess = Device::RW;
        channel0.UserCalibGain1.RegisterType = Device::HoldingReg;
        channel0.UserCalibGain1.name = "badgoodcomm";

        channel1 = channel0;
        channel1.AddressOffset = 128;
        channel1.Data.Offset = 4;

        channel2 = channel0;
        channel2.AddressOffset = 128*2;
        channel2.Data.Offset = 8;

        channel3 = channel0;
        channel3.AddressOffset = 128*3;
        channel3.Data.Offset = 12;
    }

    QList<deviceparametrs> DeviceParametrsList;

    deviceparametrs DataChan0;
    deviceparametrs DataChan1;
    deviceparametrs DataChan2;
    deviceparametrs DataChan3;
    deviceparametrs protocolVersion;
    deviceparametrs hardwareVersion;
    deviceparametrs softwareVersion;
    deviceparametrs softwareRevision;
    deviceparametrs deviceType;
    deviceparametrs modbusAddress;
    deviceparametrs modbusBaud;
    deviceparametrs modbusParity;
    deviceparametrs mbStopBits;
    deviceparametrs mbBraceCtrl;
    deviceparametrs serialNumber;
    deviceparametrs uptime;
    deviceparametrs deviceState;
    deviceparametrs disState;
    deviceparametrs accessType;
    deviceparametrs dataOrder;
    deviceparametrs mbCommCount;
    deviceparametrs mbCommError;
    deviceparametrs deviceStatus;
    deviceparametrs devErrors;
    deviceparametrs factoryDate;
    deviceparametrs softwareCrc32;
    deviceparametrs uniqueId;
    deviceparametrs onBoardTemp;
    deviceparametrs onBoardVoltage;
    deviceparametrs aiCount;
    deviceparametrs diCount;
    deviceparametrs deviceModel;

    channelparametrs channel0;
    channelparametrs channel1;
    channelparametrs channel2;
    channelparametrs channel3;

    deviceparametrs chan1Data;
    deviceparametrs chan1Status;
    deviceparametrs chan1SupportedSignals;
    deviceparametrs chan1SignalType;
    deviceparametrs chan1AdditionalParameter1;
    deviceparametrs chan1AdditionalParameter2;

    deviceparametrs elmetroAB1;
    deviceparametrs badgoodcomm;

public:
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