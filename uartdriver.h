#ifndef UARTDRIVER_H
#define UARTDRIVER_H

#include <QDialog>
#include <mainwindow.h>
#include <channel1.h>

class UartDriver:public QObject
{
    Q_OBJECT

public slots:
    double readchannelvalue(int channel);
    void readuart();
    void writechannelvalue(int channel, double value);
    void writedata();
    QString readalluartports();
    QString ReadAllUartStringData();
    QByteArray ReadAllUartByteData();

private:
    static double channeltempbuffer[4];

protected:
    void delay(int n);
    int  GetXOR(QByteArray bytearray);
    void SetRTS(bool newstate);

public:
    static double channelinputbuffer[4];
};

class ModBus:UartDriver
{
    Q_OBJECT

public slots:
    double ReadTemperature(char channel);
    double ReadVoltage(char channel);
    quint16 crc16_modbus(const QByteArray &array);

private:
    QByteArray ModBusMakeRequest(
            char DeviceAdress,
            char Function,
            char StartingAddressHi,
            char StartingAddressLo,
            char QuantityofInputRegHi,
            char QuantityofInputRegLo
            );

public:
    enum DeviceAdress {
        UniversalChannel1 = 0x01,
        UniversalChannel2 = 0x02,
        UniversalChannel3 = 0x04,
        UniversalChannel4 = 0x08
    };
    Q_ENUM(DeviceAdress)

    enum RegisterAdress {
        TemperetureAdressHi   = 0x01,
        VoltageAdressHi       = 0x02,
        CurrentAdressHi       = 0x04,
        ResistanceAdressHi    = 0x08,
        FrequencyAdressHi     = 0x10,
        TemperetureAdressLo   = 0x00, // возможно, не нужно
        VoltageAdressLo       = 0x00,
        CurrentAdressLo       = 0x00,
        ResistanceAdressLo    = 0x00,
        FrequencyAdressLo     = 0x00
    };
    Q_ENUM(RegisterAdress)

    enum RegisterCount{
        TemperetureRegCountHi = 0x01,
        VoltageRegCountHi = 0x01,
        CurrentRegCountHi = 0x01,
        ResistanceRegCountHi = 0x01,
        FrequencyRegCountHi = 0x01,
        TemperetureRegCountLo = 0x00,
        VoltageRegCountLo = 0x00,
        CurrentRegCountLo = 0x00,
        ResistanceRegCountLo = 0x00,
        FrequencyRegCountLo = 0x00
    };
    Q_ENUM(RegisterCount)

    enum Function {
        ReadCoils = 0x01,
        ReadDiscreteInputs = 0x02,
        ReadHoldingRegisters = 0x03,
        ReadInputRegisters = 0x04,
        WriteSingleCoil = 0x05,
        WriteSingleRegister = 0x06,
        ReadExceptionStatus = 0x07,
        DiagnosticRequest = 0x08,
        GetCommEventCounter = 0x0B,
        GetCommEventLog = 0x0C,
        WriteMultipleRegisters = 0x10
    };
    Q_ENUM(Function)


    /* Параметры устройства для группы G00 (выходные данные канала):
     * Для модуля типа 4AI при доступе по команде 03 (0x03)
     * Read Holding Registers и 04 (0x04) Read Input Registers
     * (формат команд аналогичный).
     * */

    /* Доступ к параметрам данного раздела осуществляется
     * с использованием команды 03 (0x03)
     * Read Holding Registers
     * */

    enum G01_Group{
        ProtocolVersionAddress  = 0,
        ProtocolVersionLenght = 1, // (1 register)
        HardwareVersionAddress  = 1,
        HardwareVersionLenght = 1,
        SoftwareVersionAddress  = 2,
        SoftwareVersionLenght = 1,
        SoftwareRevisionAddress  = 3,
        SoftwareRevisionLenght = 1,
        DeviceTypeAddress  = 4,
        DeviceTypeLenght = 1,
        ModbusAddressAddress  = 5,
        ModbusAddressLenght = 1,
        ModbusbaudrateAddress  = 6,
        ModbusbaudrateLenght = 1,
        ModbusParityAddress  = 7,
        ModbusParityLenght = 1,
        ModbusStopbitAddress  = 8,
        ModbusStopbitLenght = 1,
        BraceControlAddress  = 10,
        BraceControlLenght = 1,
        SerialNumberAddress  = 11,
        SerialNumberLenght = 2,
        UptimeAddress  = 13,
        UptimeLenght = 2,
        DeviceStateAddress  = 15,
        DeviceStateLenght = 1,
        DipStateAddress  = 16,
        DipStateLenght = 2,
        AccessStateAddress  = 18,
        AccessStateLenght = 1,
        DataOrderAddress  = 19,
        DataOrderLenght = 1,
        ModbusCommCountAddress  = 20,
        ModbusCommCountLenght = 1,
        ModbusCommErrorAddress  = 21,
        ModbusCommErrorLenght = 1,
        StatusAddress  = 22,
        StatusLenght = 1,
        ErrorsAddress  = 23,
        ErrorsLenght = 1,
        FactoryDateAddress  = 24,
        FactoryDateLenght = 2,
        SoftwareCRC32Address  = 26,
        SoftwareCRC32Lenght = 2,
        UniqueIDAddress  = 28,
        UniqueIDLenght = 6,
        OnboardTempAddress  = 35,
        OnboardTempLenght = 2,
        OnboardVoltageAddress  = 37,
        OnboardVoltageLenght = 2,
        AICountAddress  = 40,
        AICountLenght = 1,
        DICountAddress  = 41,
        DICountLenght = 1,
    };
    Q_ENUM(G01_Group)

    /* Параметры устройства для группы G02 (конфигурация канала):
     * Абсолютные адреса параметров указаны для первого канала
     * (0 в блоке «Номер канала», см. п. Адресация параметров).
     * Адреса для остальных каналов модуля получаются по формуле Addr + 128*N,
     * где N= номер канала модуля – 1. Например, адрес параметра Status для
     * первого канал модуля равен 32771, для второго – 32899 (32771+128*1),
     * для третьего – 33027 (32771+128*2)
     * */

    enum G02_Group{
        DataMeasureGenerateAddress = 0,
        DataMeasureGenerateLenght = 2,
        DataFlagsAddress = 2,
        DataFlagsLenght = 1,
//        StatusAddress = 3,
//        StatusLenght = 1,
        ErrorAddress = 4,
        ErrorLenght = 1,
        QuantityAddress = 5,
        QuantityLenght = 2,
//        UptimeAddress = 7,
//        UptimeLenght = 2,
        DataRAWAddress = 9,
        DataRAWLenght = 2,
        DataFlagsRAWAddress = 11,
        DataFlagsRAWLenght = 1,
        SupportedSignalsAddress = 12,
        SupportedSignalsLenght = 1,
        SignalTypeAddress = 13,
        SignalTypeLenght = 1,
        FilterTypeAddress = 27,
        FilterTypeLenght = 1,
        FilterLenghtAddress = 28,
        FilterLenghtLenght = 1,
        TransferFunctionAddress = 29,
        TransferFunctionLenght = 1,
        TransferSignalLowLimitAddress = 31,
        TransferSignalLowLimitLenght = 2,
        TransferSignalHighLimitAddress = 33,
        TransferSignalHighLimitLenght = 2,
        TransferScaleLowLimitAddress = 35,
        TransferScaleLowLimitLenght = 2,
        TransferScaleHighLimitAddress = 37,
        TransferScaleHighLimitLenght = 2,
        BadGoodCommAddress = 39,
        BadGoodCommLenght = 2,

        UserCalibration1GainAddress = 42,
        UserCalibration1GainLenght = 2,
        UserCalibration1OffsetAddress = 44,
        UserCalibration1OffsetLenght = 2,
        UserCalibration1DateAddress = 46,
        UserCalibration1DateLenght = 2,
        UserCalibration2GainAddress = 48,
        UserCalibration2GainLenght = 2,
        UserCalibration2OffsetAddress = 50,
        UserCalibration2OffsetLenght = 2,
        UserCalibration2DateAddress = 52,
        UserCalibration2DateLenght = 2,
        UserCalibration3GainAddress = 54,
        UserCalibration3GainLenght = 2,
        UserCalibration3OffsetAddress = 56,
        UserCalibration3OffsetLenght = 2,
        UserCalibration3DateAddress = 58,
        UserCalibration3DateLenght = 2,
        UserCalibration4GainAddress = 60,
        UserCalibration4GainLenght = 2,
        UserCalibration4OffsetAddress = 62,
        UserCalibration4OffsetLenght = 2,
        UserCalibration4DateAddress = 64,
        UserCalibration4DateLenght = 2,

        FactoryCalibration1GainAddress = 66,
        FactoryCalibration1GainLenght = 2,
        FactoryCalibration1OffsetAddress = 68,
        FactoryCalibration1OffsetLenght = 2,
        FactoryCalibration1DateAddress = 70,
        FactoryCalibration1DateLenght = 2,
        FactoryCalibration2GainAddress = 72,
        FactoryCalibration2GainLenght = 2,
        FactoryCalibration2OffsetAddress = 74,
        FactoryCalibration2OffsetLenght = 2,
        FactoryCalibration2DateAddress = 76,
        FactoryCalibration2DateLenght = 2,
        FactoryCalibration3GainAddress = 78,
        FactoryCalibration3GainLenght = 2,
        FactoryCalibration3OffsetAddress = 80,
        FactoryCalibration3OffsetLenght = 2,
        FactoryCalibration3DateAddress = 82,
        FactoryCalibration3DateLenght = 2,
        FactoryCalibration4GainAddress = 84,
        FactoryCalibration4GainLenght = 2,
        FactoryCalibration4OffsetAddress = 86,
        FactoryCalibration4OffsetLenght = 2,
        FactoryCalibration4DateAddress = 88,
        FactoryCalibration4DateLenght = 2,

        MeasuredCJValueAddress = 90,
        MeasuredCJValueLenght = 2,
        ResultCJValueAddress = 92,
        ResultCJValueLenght = 2,

        AdditionalParameterAddress = 14,
        AdditionalParameterLenght = 12,
    };
    Q_ENUM(G02_Group)

    enum MeasureTypeVoltage{
        Voltage1VoltNoBreakControl = 1,
        Voltage1VoltWithBreakControl = 2,
        Voltage100mVoltNoBreakControl = 3,
        Voltage100mVoltWithBreakControl = 4,
        Voltage5VoltNoBreakControl = 5,
        Voltage5VoltWithBreakControl = 6,
        Voltage10VoltNoBreakControl = 7,
        Voltage10VoltWithBreakControl = 8
    };
    Q_ENUM(MeasureTypeVoltage)

    enum MeasureTypeWireMode{
        Wire3NoBreakControl = 3,
        Wire3WithBreakControl = 4,
        Wire4NoBreakControl = 5,
        Wire4WithBreakControl = 6
    };
    Q_ENUM(MeasureTypeWireMode)

    enum TermoCoupleType{
        R = 1,  //(ТПП 13)
        S = 2,  //(ТПП 10)
        B = 3,  //(ТПР)
        J = 4,  //(ТЖК)
        T = 5,  //(ТМКн)
        E = 6,  //(ТХКн)
        K = 7,  //(ТХА)
        N = 8,  //(ТНН)
        A1 = 9, //(ТВР)
        A2 = 10,//(ТВР)
        A3 = 11,//(ТВР)
        L = 12, //(ТХК)
        M = 14  //(ТМК)
    };
    Q_ENUM(TermoCoupleType)
/*
    enum TermoResistorType{
        Pt1dot3910 = 1,
        Pt1dot3850  = 2,
        Cu1dot4280 = 3,
        Cu1dot4260  = 4,
        Ni1dot6170 = 5,
        М53  = 6,
        P46 = 7
    };
    Q_ENUM(TermoResistorType)

    enum SignalTypeSelect{
        MeasureOff = 0,
        MeasureCurrent = 1,
        MeasureVoltage = 2,
        MeasureResistance = 3,
        MeasureTC = 4,
        MeasureTermoResistance = 5,
        MeasureDiscrette = 6,
        MeasureImpulseCount = 7,
        MeasureImpulseFrequency = 8
    };
    Q_ENUM(SignalTypeSelect)

    enum FilterTypeSelect{
        FilterOff = 0,
        FilterMediama = 1,
        FilterAverage = 2

    };
    Q_ENUM(FilterTypeSelect)

    enum TransmissionFunction{
        LinearTransmission = 0,
        QuadroTransmission = 1,
        SQRTTransmission = 2
    };
    Q_ENUM(TransmissionFunction)

    enum Errors{
    };
    Q_ENUM(Errors)*/
};

#endif // UARTDRIVER_H
