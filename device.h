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

        {
            //configure chanel

            channel0.BaseAddress = 32768;
            channel0.AddressOffset = 0;

            channel0.Data.Offset = 0 ; // 32768
            channel0.Data.StorageType = Device::D;
            channel0.Data.ParamType = Device::F32;
            channel0.Data.WorkLevelAccess = Device::R;
            channel0.Data.RegisterType = Device::InputReg;
            channel0.Data.name = "chanData";


            channel0.DataFlags.Offset = 32770 ;
            channel0.DataFlags.StorageType = Device::D;
            channel0.DataFlags.ParamType = Device::U16;
            channel0.DataFlags.WorkLevelAccess = Device::R;
            channel0.DataFlags.RegisterType = Device::HoldingReg;
            channel0.DataFlags.name = "DataFlags";


            channel0.Status.Offset = 32771;
            channel0.Status.StorageType = Device::D;
            channel0.Status.ParamType = Device::U16;
            channel0.Status.RegisterType = Device::HoldingReg;
            channel0.Status.WorkLevelAccess = Device::R;
            channel0.Status.name = "chanStatus";

            channel0.Error.Offset = 32772;
            channel0.Error.StorageType = Device::D;
            channel0.Error.ParamType = Device::U16;
            channel0.Error.RegisterType = Device::HoldingReg;
            channel0.Error.WorkLevelAccess = Device::R;
            channel0.Error.name = "Error";

            channel0.Quantity.Offset = 32773;
            channel0.Quantity.StorageType = Device::D;
            channel0.Quantity.ParamType = Device::U32;
            channel0.Quantity.RegisterType = Device::HoldingReg;
            channel0.Quantity.WorkLevelAccess = Device::R;
            channel0.Quantity.name = "Error";

            channel0.Uptime.Offset = 32775;
            channel0.Uptime.StorageType = Device::D;
            channel0.Uptime.ParamType = Device::U32;
            channel0.Uptime.RegisterType = Device::HoldingReg;
            channel0.Uptime.WorkLevelAccess = Device::R;
            channel0.Uptime.name = "Uptime";

            channel0.RawData.Offset = 32777;
            channel0.RawData.StorageType = Device::D;
            channel0.RawData.ParamType = Device::U32;
            channel0.RawData.RegisterType = Device::HoldingReg;
            channel0.RawData.WorkLevelAccess = Device::R;
            channel0.RawData.name = "RawData";

            channel0.RawDataFlags.Offset = 32779;
            channel0.RawDataFlags.StorageType = Device::D;
            channel0.RawDataFlags.ParamType = Device::U16;
            channel0.RawDataFlags.RegisterType = Device::HoldingReg;
            channel0.RawDataFlags.WorkLevelAccess = Device::R;
            channel0.RawDataFlags.name = "RawDataFlags";


            channel0.SupportedSignals.Offset = 32780;
            channel0.SupportedSignals.StorageType = Device::D;
            channel0.SupportedSignals.ParamType = Device::U16;
            channel0.SupportedSignals.RegisterType = Device::HoldingReg;
            channel0.SupportedSignals.WorkLevelAccess = Device::R;
            channel0.SupportedSignals.name = "SupportedSignals";

            channel0.SignalType.Offset = 32781;
            channel0.SignalType.StorageType = Device::NV;
            channel0.SignalType.ParamType = Device::U16;
            channel0.SupportedSignals.RegisterType = Device::HoldingReg;
            channel0.SignalType.WorkLevelAccess = Device::RW;
            channel0.SignalType.name = "SignalType";

            channel0.AdditionalParameter1.Offset = 32782;
            channel0.AdditionalParameter1.StorageType = Device::NV;
            channel0.AdditionalParameter1.ParamType = Device::A12;
            channel0.AdditionalParameter1.RegisterType = Device::HoldingReg;
            channel0.AdditionalParameter1.WorkLevelAccess = Device::RW;
            channel0.AdditionalParameter1.name = "AdditionalParameter1";

            channel0.AdditionalParameter2.Offset = 32788;
            channel0.AdditionalParameter2.StorageType = Device::NV;
            channel0.AdditionalParameter2.ParamType = Device::A12;
            channel0.AdditionalParameter2.RegisterType = Device::HoldingReg;
            channel0.AdditionalParameter2.WorkLevelAccess = Device::RW;
            channel0.AdditionalParameter2.name = "AdditionalParameter2";


            channel0.FilterType.Offset = 32795;
            channel0.FilterType.StorageType = Device::D;
            channel0.FilterType.ParamType = Device::U16;
            channel0.FilterType.RegisterType = Device::HoldingReg;
            channel0.FilterType.WorkLevelAccess = Device::RW;
            channel0.FilterType.name = "FilterType";


            channel0.FilterLength.Offset = 32796;
            channel0.FilterLength.StorageType = Device::D;
            channel0.FilterLength.ParamType = Device::U16;
            channel0.FilterLength.RegisterType = Device::HoldingReg;
            channel0.FilterLength.WorkLevelAccess = Device::RW;
            channel0.FilterLength.name = "FilterLength";


            channel0.FilterLength.Offset = 32796;
            channel0.FilterLength.StorageType = Device::NV;
            channel0.FilterLength.ParamType = Device::U16;
            channel0.FilterLength.RegisterType = Device::HoldingReg;
            channel0.FilterLength.WorkLevelAccess = Device::RW;
            channel0.FilterLength.name = "FilterLength";


            channel0.TransferFunction.Offset = 32797;
            channel0.TransferFunction.StorageType = Device::NV;
            channel0.TransferFunction.ParamType = Device::U16;
            channel0.TransferFunction.RegisterType = Device::HoldingReg;
            channel0.TransferFunction.WorkLevelAccess = Device::RW;
            channel0.TransferFunction.name = "FilterLength";


            channel0.TransferSignalLowLim.Offset = 32799;
            channel0.TransferSignalLowLim.StorageType = Device::NV;
            channel0.TransferSignalLowLim.ParamType = Device::F32;
            channel0.TransferSignalLowLim.RegisterType = Device::HoldingReg;
            channel0.TransferSignalLowLim.WorkLevelAccess = Device::RW;
            channel0.TransferSignalLowLim.name = "TransferSignalLowLim";


            channel0.TransferSignalHighLim.Offset = 32801;
            channel0.TransferSignalHighLim.StorageType = Device::NV;
            channel0.TransferSignalHighLim.ParamType = Device::F32;
            channel0.TransferSignalHighLim.RegisterType = Device::HoldingReg;
            channel0.TransferSignalHighLim.WorkLevelAccess = Device::RW;
            channel0.TransferSignalHighLim.name = "TransferSignalHighLim";


            channel0.TransferScaleLowLim.Offset = 32803;
            channel0.TransferScaleLowLim.StorageType = Device::NV;
            channel0.TransferScaleLowLim.ParamType = Device::F32;
            channel0.TransferScaleLowLim.RegisterType = Device::HoldingReg;
            channel0.TransferScaleLowLim.WorkLevelAccess = Device::RW;
            channel0.TransferScaleLowLim.name = "TransferScaleLowLim";


            channel0.TransferScaleHighLim.Offset = 32805;
            channel0.TransferScaleHighLim.StorageType = Device::NV;
            channel0.TransferScaleHighLim.ParamType = Device::F32;
            channel0.TransferScaleHighLim.RegisterType = Device::HoldingReg;
            channel0.TransferScaleHighLim.WorkLevelAccess = Device::RW;
            channel0.TransferScaleHighLim.name = "TransferScaleHighLim";


            channel0.BadGoodComm.Offset = 32807;
            channel0.BadGoodComm.StorageType = Device::D;
            channel0.BadGoodComm.ParamType = Device::F32;
            channel0.BadGoodComm.RegisterType = Device::HoldingReg;
            channel0.BadGoodComm.WorkLevelAccess = Device::RW;
            channel0.BadGoodComm.name = "BadGoodComm";

            channel0.UserCalibGain1.Offset = 32810;
            channel0.UserCalibGain1.StorageType = Device::NV;
            channel0.UserCalibGain1.ParamType = Device::F32;
            channel0.UserCalibGain1.WorkLevelAccess = Device::RW;
            channel0.UserCalibGain1.RegisterType = Device::HoldingReg;
            channel0.UserCalibGain1.name = "UserCalibGain1";

            channel0.UserCalibOffset1.Offset = 32812;
            channel0.UserCalibOffset1.StorageType = Device::NV;
            channel0.UserCalibOffset1.ParamType = Device::F32;
            channel0.UserCalibOffset1.WorkLevelAccess = Device::RW;
            channel0.UserCalibOffset1.RegisterType = Device::HoldingReg;
            channel0.UserCalibOffset1.name = "UserCalibOffset1";

            channel0.UserCalibDate1.Offset = 32814;
            channel0.UserCalibDate1.StorageType = Device::NV;
            channel0.UserCalibDate1.ParamType = Device::U32;
            channel0.UserCalibDate1.WorkLevelAccess = Device::RW;
            channel0.UserCalibDate1.RegisterType = Device::HoldingReg;
            channel0.UserCalibDate1.name = "UserCalibDate1";


            channel0.UserCalibGain2.Offset = 32816;
            channel0.UserCalibGain2.StorageType = Device::NV;
            channel0.UserCalibGain2.ParamType = Device::F32;
            channel0.UserCalibGain2.WorkLevelAccess = Device::RW;
            channel0.UserCalibGain2.RegisterType = Device::HoldingReg;
            channel0.UserCalibGain2.name = "UserCalibGain1";

            channel0.UserCalibOffset2.Offset = 32818;
            channel0.UserCalibOffset2.StorageType = Device::NV;
            channel0.UserCalibOffset2.ParamType = Device::F32;
            channel0.UserCalibOffset2.WorkLevelAccess = Device::RW;
            channel0.UserCalibOffset2.RegisterType = Device::HoldingReg;
            channel0.UserCalibOffset2.name = "UserCalibOffset1";

            channel0.UserCalibDate2.Offset = 32820;
            channel0.UserCalibDate2.StorageType = Device::NV;
            channel0.UserCalibDate2.ParamType = Device::U32;
            channel0.UserCalibDate2.WorkLevelAccess = Device::RW;
            channel0.UserCalibDate2.RegisterType = Device::HoldingReg;
            channel0.UserCalibDate2.name = "UserCalibDate1";

            channel0.UserCalibGain3.Offset = 32822;
            channel0.UserCalibGain3.StorageType = Device::NV;
            channel0.UserCalibGain3.ParamType = Device::F32;
            channel0.UserCalibGain3.WorkLevelAccess = Device::RW;
            channel0.UserCalibGain3.RegisterType = Device::HoldingReg;
            channel0.UserCalibGain3.name = "UserCalibGain1";

            channel0.UserCalibOffset3.Offset = 32824;
            channel0.UserCalibOffset3.StorageType = Device::NV;
            channel0.UserCalibOffset3.ParamType = Device::F32;
            channel0.UserCalibOffset3.WorkLevelAccess = Device::RW;
            channel0.UserCalibOffset3.RegisterType = Device::HoldingReg;
            channel0.UserCalibOffset3.name = "UserCalibOffset1";

            channel0.UserCalibDate3.Offset = 32826;
            channel0.UserCalibDate3.StorageType = Device::NV;
            channel0.UserCalibDate3.ParamType = Device::U32;
            channel0.UserCalibDate3.WorkLevelAccess = Device::RW;
            channel0.UserCalibDate3.RegisterType = Device::HoldingReg;
            channel0.UserCalibDate3.name = "UserCalibDate1";


            channel0.UserCalibGain4.Offset = 32828;
            channel0.UserCalibGain4.StorageType = Device::NV;
            channel0.UserCalibGain4.ParamType = Device::F32;
            channel0.UserCalibGain4.WorkLevelAccess = Device::RW;
            channel0.UserCalibGain4.RegisterType = Device::HoldingReg;
            channel0.UserCalibGain4.name = "UserCalibGain1";

            channel0.UserCalibOffset4.Offset = 32830;
            channel0.UserCalibOffset4.StorageType = Device::NV;
            channel0.UserCalibOffset4.ParamType = Device::F32;
            channel0.UserCalibOffset4.WorkLevelAccess = Device::RW;
            channel0.UserCalibOffset4.RegisterType = Device::HoldingReg;
            channel0.UserCalibOffset4.name = "UserCalibOffset1";

            channel0.UserCalibDate4.Offset = 32832;
            channel0.UserCalibDate4.StorageType = Device::NV;
            channel0.UserCalibDate4.ParamType = Device::U32;
            channel0.UserCalibDate4.WorkLevelAccess = Device::RW;
            channel0.UserCalibDate4.RegisterType = Device::HoldingReg;
            channel0.UserCalibDate4.name = "UserCalibDate1";


            channel0.FactoryCalibGain1.Offset = 32834;
            channel0.FactoryCalibGain1.StorageType = Device::NV;
            channel0.FactoryCalibGain1.ParamType = Device::F32;
            channel0.FactoryCalibGain1.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibGain1.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibGain1.name = "FactoryCalibGain1";

            channel0.FactoryCalibOffset1.Offset = 32836;
            channel0.FactoryCalibOffset1.StorageType = Device::NV;
            channel0.FactoryCalibOffset1.ParamType = Device::F32;
            channel0.FactoryCalibOffset1.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibOffset1.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibOffset1.name = "FactoryCalibOffset1";

            channel0.FactoryCalibDate1.Offset = 32838;
            channel0.FactoryCalibDate1.StorageType = Device::NV;
            channel0.FactoryCalibDate1.ParamType = Device::U32;
            channel0.FactoryCalibDate1.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibDate1.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibDate1.name = "FactoryCalibDate1";


            channel0.FactoryCalibGain2.Offset = 32840;
            channel0.FactoryCalibGain2.StorageType = Device::NV;
            channel0.FactoryCalibGain2.ParamType = Device::F32;
            channel0.FactoryCalibGain2.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibGain2.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibGain2.name = "FactoryCalibGain2";


            channel0.FactoryCalibOffset2.Offset = 32842;
            channel0.FactoryCalibOffset2.StorageType = Device::NV;
            channel0.FactoryCalibOffset2.ParamType = Device::F32;
            channel0.FactoryCalibOffset2.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibOffset2.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibOffset2.name = "FactoryCalibOffset2";


            channel0.FactoryCalibDate2.Offset = 32844;
            channel0.FactoryCalibDate2.StorageType = Device::NV;
            channel0.FactoryCalibDate2.ParamType = Device::U32;
            channel0.FactoryCalibDate2.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibDate2.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibDate2.name = "FactoryCalibDate2";


            channel0.FactoryCalibGain3.Offset = 32846;
            channel0.FactoryCalibGain3.StorageType = Device::NV;
            channel0.FactoryCalibGain3.ParamType = Device::F32;
            channel0.FactoryCalibGain3.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibGain3.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibGain3.name = "FactoryCalibGain3";


            channel0.FactoryCalibOffset3.Offset = 32848;
            channel0.FactoryCalibOffset3.StorageType = Device::NV;
            channel0.FactoryCalibOffset3.ParamType = Device::F32;
            channel0.FactoryCalibOffset3.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibOffset3.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibOffset3.name = "FactoryCalibOffset3";


            channel0.FactoryCalibDate3.Offset = 32850;
            channel0.FactoryCalibDate3.StorageType = Device::NV;
            channel0.FactoryCalibDate3.ParamType = Device::U32;
            channel0.FactoryCalibDate3.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibDate3.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibDate3.name = "FactoryCalibDate3";


            channel0.FactoryCalibGain4.Offset = 32852;
            channel0.FactoryCalibGain4.StorageType = Device::NV;
            channel0.FactoryCalibGain4.ParamType = Device::F32;
            channel0.FactoryCalibGain4.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibGain4.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibGain4.name = "FactoryCalibGain4";

            channel0.FactoryCalibOffset4.Offset = 32854;
            channel0.FactoryCalibOffset4.StorageType = Device::NV;
            channel0.FactoryCalibOffset4.ParamType = Device::F32;
            channel0.FactoryCalibOffset4.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibOffset4.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibOffset4.name = "FactoryCalibOffset4";

            channel0.FactoryCalibDate4.Offset = 32856;
            channel0.FactoryCalibDate4.StorageType = Device::NV;
            channel0.FactoryCalibDate4.ParamType = Device::U32;
            channel0.FactoryCalibDate4.WorkLevelAccess = Device::RW;
            channel0.FactoryCalibDate4.RegisterType = Device::HoldingReg;
            channel0.FactoryCalibDate4.name = "FactoryCalibDate4";

            channel0.CjValue.Offset = 32858;
            channel0.CjValue.StorageType = Device::D;
            channel0.CjValue.ParamType = Device::F32;
            channel0.CjValue.WorkLevelAccess = Device::R;
            channel0.CjValue.RegisterType = Device::HoldingReg;
            channel0.CjValue.name = "CjValue";

            channel0.ResultCjValue.Offset = 32860;
            channel0.ResultCjValue.StorageType = Device::D;
            channel0.ResultCjValue.ParamType = Device::F32;
            channel0.ResultCjValue.WorkLevelAccess = Device::R;
            channel0.ResultCjValue.RegisterType = Device::HoldingReg;
            channel0.ResultCjValue.name = "ResultCjValue";
        }


        channel1 = channel0;// делаем 1 канал как 0 за исключением адресов


        channel1.AddressOffset = 128;
        channel1.Data.Offset = 4;

        channel2 = channel0;
        channel2.AddressOffset = 128*2;
        channel2.Data.Offset = 8;

        channel3 = channel0;
        channel3.AddressOffset = 128*3;
        channel3.Data.Offset = 12;


        protocolVersion.Offset = 16384;
        protocolVersion.StorageType = Device::S;
        protocolVersion.ParamType = Device::U16;
        protocolVersion.RegisterType = Device::HoldingReg;
        protocolVersion.WorkLevelAccess = Device::R;
        protocolVersion.name = "protocolVersion";


        hardwareVersion.Offset = 16385;
        hardwareVersion.StorageType = Device::S;
        hardwareVersion.ParamType = Device::U16;
        hardwareVersion.RegisterType = Device::HoldingReg;
        hardwareVersion.WorkLevelAccess = Device::R;
        hardwareVersion.name = "hardwareVersion";

        softwareVersion.Offset = 16386;
        softwareVersion.StorageType = Device::S;
        softwareVersion.ParamType = Device::U16;
        softwareVersion.RegisterType = Device::HoldingReg;
        softwareVersion.WorkLevelAccess = Device::R;
        softwareVersion.name = "softwareVersion";

        softwareRevision.Offset = 16387;
        softwareRevision.StorageType = Device::S;
        softwareRevision.ParamType = Device::U16;
        softwareRevision.RegisterType = Device::HoldingReg;
        softwareRevision.WorkLevelAccess = Device::R;
        softwareRevision.name = "softwareRevision";

        deviceType.Offset = 16388;
        deviceType.StorageType = Device::S;
        deviceType.ParamType = Device::U16;
        deviceType.RegisterType = Device::HoldingReg;
        deviceType.WorkLevelAccess = Device::R;
        deviceType.name = "deviceType";


        modbusBaud.Offset = 16390;
        modbusBaud.StorageType = Device::D;
        modbusBaud.ParamType = Device::U16;
        modbusBaud.RegisterType = Device::HoldingReg;
        modbusBaud.WorkLevelAccess = Device::R;
        modbusBaud.name = "modbusBaud";



        modbusParity.Offset = 16391;
        modbusParity.StorageType = Device::D;
        modbusParity.ParamType = Device::U16;
        modbusParity.RegisterType = Device::HoldingReg;
        modbusParity.WorkLevelAccess = Device::R;
        modbusParity.name = "modbusParity";


        mbStopBits.Offset = 16392;
        mbStopBits.StorageType = Device::D;
        mbStopBits.ParamType = Device::U16;
        mbStopBits.RegisterType = Device::HoldingReg;
        mbStopBits.WorkLevelAccess = Device::R;
        mbStopBits.name = "mbStopBits";


        mbBraceCtrl.Offset = 16393;
        mbBraceCtrl.StorageType = Device::S;
        mbBraceCtrl.ParamType = Device::U16;
        mbBraceCtrl.RegisterType = Device::HoldingReg;
        mbBraceCtrl.WorkLevelAccess = Device::R;
        mbBraceCtrl.name = "mbBraceCtrl";

        serialNumber.Offset = 16395;
        serialNumber.StorageType = Device::S;
        serialNumber.ParamType = Device::U32;
        serialNumber.RegisterType = Device::HoldingReg;
        serialNumber.WorkLevelAccess = Device::R;
        serialNumber.name = "serialNumber";

        uptime.Offset = 16397;
        uptime.StorageType = Device::D;
        uptime.ParamType = Device::U32;
        uptime.RegisterType = Device::HoldingReg;
        uptime.WorkLevelAccess = Device::R;
        uptime.name = "uptime";


        deviceState.Offset = 16399;
        deviceState.StorageType = Device::D;
        deviceState.ParamType = Device::U16;
        deviceState.RegisterType = Device::HoldingReg;
        deviceState.WorkLevelAccess = Device::R;
        deviceState.name = "deviceState";

        disState.Offset = 16400;
        disState.StorageType = Device::D;
        disState.ParamType = Device::U16;
        disState.RegisterType = Device::HoldingReg;
        disState.WorkLevelAccess = Device::R;
        disState.name = "disState";

        accessType.Offset = 16402;
        accessType.StorageType = Device::D;
        accessType.ParamType = Device::U16;
        accessType.RegisterType = Device::HoldingReg;
        accessType.WorkLevelAccess = Device::R;
        accessType.name = "accessType";


        dataOrder.Offset = 16403;
        dataOrder.StorageType = Device::NV;
        dataOrder.ParamType = Device::U16;
        dataOrder.RegisterType = Device::HoldingReg;
        dataOrder.WorkLevelAccess = Device::R;
        dataOrder.name = "dataOrder";


        mbCommCount.Offset = 16404;
        mbCommCount.StorageType = Device::D;
        mbCommCount.ParamType = Device::U16;
        mbCommCount.RegisterType = Device::HoldingReg;
        mbCommCount.WorkLevelAccess = Device::R;
        mbCommCount.name = "mbCommCount";



        mbCommError.Offset = 16405;
        mbCommError.StorageType = Device::D;
        mbCommError.ParamType = Device::U16;
        mbCommError.RegisterType = Device::HoldingReg;
        mbCommError.WorkLevelAccess = Device::R;
        mbCommError.name = "mbCommError";


        deviceStatus.Offset = 16406;
        deviceStatus.StorageType = Device::D;
        deviceStatus.ParamType = Device::U16;
        deviceStatus.RegisterType = Device::HoldingReg;
        deviceStatus.WorkLevelAccess = Device::R;
        deviceStatus.name = "deviceStatus";

        devErrors.Offset = 16407;
        devErrors.StorageType = Device::D;
        devErrors.ParamType = Device::U16;
        devErrors.RegisterType = Device::HoldingReg;
        devErrors.WorkLevelAccess = Device::R;
        devErrors.name = "devErrors";


        factoryDate.Offset = 16408;
        factoryDate.StorageType = Device::NV;
        factoryDate.ParamType = Device::U32;
        factoryDate.RegisterType = Device::HoldingReg;
        factoryDate.WorkLevelAccess = Device::R;
        factoryDate.name = "factoryDate";


        softwareCrc32.Offset = 16410;
        softwareCrc32.StorageType = Device::NV;
        softwareCrc32.ParamType = Device::U32;
        softwareCrc32.RegisterType = Device::HoldingReg;
        softwareCrc32.WorkLevelAccess = Device::R;
        softwareCrc32.name = "softwareCrc32";


        uniqueId.Offset = 16412;
        uniqueId.StorageType = Device::D;
        uniqueId.ParamType = Device::A12;
        uniqueId.RegisterType = Device::HoldingReg;
        uniqueId.WorkLevelAccess = Device::R;
        uniqueId.name = "uniqueId";


        onBoardTemp.Offset = 16419;
        onBoardTemp.StorageType = Device::D;
        onBoardTemp.ParamType = Device::F32;
        onBoardTemp.RegisterType = Device::HoldingReg;
        onBoardTemp.WorkLevelAccess = Device::R;
        onBoardTemp.name = "onBoardTemp";


        onBoardVoltage.Offset = 16421;
        onBoardVoltage.StorageType = Device::D;
        onBoardVoltage.ParamType = Device::F32;
        onBoardVoltage.RegisterType = Device::HoldingReg;
        onBoardVoltage.WorkLevelAccess = Device::R;
        onBoardVoltage.name = "onBoardVoltage";

        aiCount.Offset = 16423;
        aiCount.StorageType = Device::D;
        aiCount.ParamType = Device::U16;
        aiCount.RegisterType = Device::HoldingReg;
        aiCount.WorkLevelAccess = Device::R;
        aiCount.name = "aiCount";


        diCount.Offset = 16425;
        diCount.StorageType = Device::D;
        diCount.ParamType = Device::U16;
        diCount.RegisterType = Device::HoldingReg;
        diCount.WorkLevelAccess = Device::R;
        diCount.name = "diCount";


        deviceModel.Offset = 16428;
        deviceModel.StorageType = Device::D;
        deviceModel.ParamType = Device::U16;
        deviceModel.RegisterType = Device::HoldingReg;
        deviceModel.WorkLevelAccess = Device::R;
        deviceModel.name = "deviceModel";

    }

    QList<deviceparametrs> DeviceParametrsList;

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
