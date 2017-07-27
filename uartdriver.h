#ifndef UARTDRIVER_H
#define UARTDRIVER_H

#include <QDialog>
#include <mainwindow.h>
#include <channel1.h>
#include <mathresolver.h>

class UartDriver:public QObject
{
    Q_OBJECT

public slots:
    void writechannelvalue(int channel, double value);
    double readchannelvalue(int channel);
    QString ReadAllAvailableCOMPorts();
    QString ReadAllUartDataStringFormat();
    QByteArray ReadAllUartDataByteFormat();

private:
    static double channeltempbuffer[4];
    QString GetPathToRTSPinValue () {return "/sys/class/gpio/gpio66/value";}
    QString GetPathToRTSPinDirection () {return "/sys/class/gpio/gpio66/direction";}


protected:
    void DelayMsec(int n);
    void SetRTS(bool newstate);
    QByteArray UartWriteData(QByteArray data);

public:
    static double channelinputbuffer[4];
    static bool needtoupdatechannel[4];
    void SetRTSPinDirection();
};

class ModBus: public UartDriver
{
    Q_OBJECT

private:

    static uint ConnectFailure;

    ChannelOptions channel1objectModbus;
    ChannelOptions channel2objectModbus;
    ChannelOptions channel3objectModbus;
    ChannelOptions channel4objectModbus;

public slots:

    double ReadDataChannel(int channeladdress);
    double ReadOnBoardTemperature ();
    double ReadOnBoardVoltage();

    uint16_t  GetChannelSignalType(uint8_t channel);

    void  SetChannelSignalType(uint8_t channel, uint8_t signaltype);


    void SetSingleCoil(char channel, uint16_t Address, bool newstate);
    double ClickRelay(char channel);
    quint16 Calculate_crc16_modbus(const QByteArray &array);
    void ReadAllChannelsThread();

public:

    uint GetConnectFailureStatus(){return ConnectFailure;}
    void SetConnectFailure(uint newconnectfailurestate) {ConnectFailure = newconnectfailurestate; }

    QByteArray ModBusMakeRequest(
            char DeviceAdress,
            char Function,
            uint16_t Address,
            uint16_t Lenght
            );

    QByteArray ModBusMakeRequest(
            char DeviceAdress,
            char Function,
            uint16_t Address,
            uint16_t AddressBias,
            uint16_t Lenght
            );

    float ModBusGetRegister(
            char DeviceAdress,
            char Function,
            uint16_t Address,
            uint16_t Lenght
            );

    void ModBusSetRegister(char DeviceAdress,char Function,uint16_t Address,uint16_t Value);

public:
    double DataChannelRead (char channel);
    double DataChannel1Read();

public:

    enum UartErrors {
        NoResponse = 0x01 ,
        CrcError = 0x02
    };
    Q_ENUM(UartErrors)


    enum DeviceAdress {
        MainDeviceAddress = 0x01 ,
        Board4AIAddress = 0x01 ,
        UniversalChannel2 = 0x02 ,
        UniversalChannel3 = 0x04 ,
        UniversalChannel4 = 0x08
    };
    Q_ENUM(DeviceAdress)

    enum RegisterAdress {
        TemperetureAdress     = 0x0000,
        VoltageAdressHi       = 0x02,
        CurrentAdressHi       = 0x04,
        ResistanceAdressHi    = 0x08,
        FrequencyAdressHi     = 0x10,
        TemperetureAdressLo   = 0x00, // возможно, не нужно
        VoltageAdressLo       = 0x00,
        CurrentAdressLo       = 0x00,
        ResistanceAdressLo    = 0x00,
        FrequencyAdressLo     = 0x00,
        ElmetroRelayAddress = 0x01,
        ElmetroChannelAB1Address = 0x00,
        ElmetroChannelAB2Address = 0x02,
        ElmetroChannelAB3Address = 0x04,
        ElmetroChannelAB4Address = 0x06

    };
    Q_ENUM(RegisterAdress)

    enum RegisterCount{
        TemperetureRegLenght = 0x000A,
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
    enum G00_Group
    {
        G00Bias = 0,
        DataChannel1 = G00Bias + 0,
        DataChannel2 = G00Bias + 2,
        DataChannel3 = G00Bias + 4,
        DataChannel4 = G00Bias + 6,
        DataChannelLenght = 2
    };
    Q_ENUM(G00_Group)

    /* Доступ к параметрам данного раздела осуществляется
     * с использованием команды 03 (0x03)
     * Read Holding Registers
     * */

    enum G01_Group{
        G01Bias = 0x4000,
        ProtocolVersionAddress  = G01Bias +  0,
        ProtocolVersionLenght = 1, // (1 register)
        HardwareVersionAddress  = G01Bias + 1,
        HardwareVersionLenght = 1,
        SoftwareVersionAddress  = G01Bias + 2,
        SoftwareVersionLenght = 1,
        SoftwareRevisionAddress  = G01Bias + 3,
        SoftwareRevisionLenght = 1,
        DeviceTypeAddress  = G01Bias + 4,
        DeviceTypeLenght = 1,
        ModbusAddress  = G01Bias + 5,
        ModbusAddressLenght = 1,
        ModbusbaudrateAddress  = G01Bias + 6,
        ModbusbaudrateLenght = 1,
        ModbusParityAddress  = G01Bias + 7,
        ModbusParityLenght = 1,
        ModbusStopbitAddress  = G01Bias + 8,
        ModbusStopbitLenght = 1,
        BraceControlAddress  = G01Bias + 10,
        BraceControlLenght = 1,
        SerialNumberAddress  = G01Bias + 11,
        SerialNumberLenght = 2,
        UptimeAddress  = G01Bias + 13,
        UptimeLenght = 2,
        DeviceStateAddress  = G01Bias + 15,
        DeviceStateLenght = 1,
        DipStateAddress  = G01Bias + 16,
        DipStateLenght = 2,
        AccessStateAddress  = G01Bias + 18,
        AccessStateLenght = 1,
        DataOrderAddress  = G01Bias + 19,
        DataOrderLenght = 1,
        ModbusCommCountAddress  = G01Bias + 20,
        ModbusCommCountLenght = 1,
        ModbusCommErrorAddress  = G01Bias + 21,
        ModbusCommErrorLenght = 1,
        StatusAddress  = G01Bias + 22,
        StatusLenght = 1,
        ErrorsAddress  = G01Bias + 23,
        ErrorsLenght = 1,
        FactoryDateAddress  = G01Bias + 24,
        FactoryDateLenght = 2,
        SoftwareCRC32Address  = G01Bias + 26,
        SoftwareCRC32Lenght = 2,
        UniqueIDAddress  = G01Bias + 28,
        UniqueIDLenght = 6,
        OnboardTempAddress  = G01Bias + 35,
        OnboardTempLenght = 2,
        OnboardVoltageAddress  = G01Bias + 37,
        OnboardVoltageLenght = 2,
        AICountAddress  = G01Bias + 40,
        AICountLenght = 1,
        DICountAddress  = G01Bias + 41,
        DICountLenght = 1
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
        G02Bias = 0x8000,
        ChannelAddressBias = 128,
        Channel1AddressBias = 0,
        Channel2AddressBias = 128,
        Channel3AddressBias = 256,
        Channel4AddressBias = 512,
        DataMeasureGenerateAddress =  G02Bias + 0,
        DataMeasureGenerateLenght = 2,
        DataFlagsAddress =  G02Bias + 2,
        DataFlagsLenght = 1,
        //        StatusAddress =  G02Bias + 3,
        //        StatusLenght = 1,
        ErrorAddress =  G02Bias + 4,
        ErrorLenght = 1,
        QuantityAddress =  G02Bias + 5,
        QuantityLenght = 2,
        //        UptimeAddress =  G02Bias + 7,
        //        UptimeLenght = 2,
        DataRAWAddress =  G02Bias + 9,
        DataRAWLenght = 2,
        DataFlagsRAWAddress =  G02Bias + 11,
        DataFlagsRAWLenght = 1,
        SupportedSignalsAddress =  G02Bias + 12,
        SupportedSignalsLenght = 1,
        SignalTypeAddress =  G02Bias + 13,
        SignalTypeLenght = 1,
        FilterTypeAddress =  G02Bias + 27,
        FilterTypeLenght = 1,
        FilterLenghtAddress =  G02Bias + 28,
        FilterLenghtLenght = 1,
        TransferFunctionAddress =  G02Bias + 29,
        TransferFunctionLenght = 1,
        TransferSignalLowLimitAddress =  G02Bias + 31,
        TransferSignalLowLimitLenght = 2,
        TransferSignalHighLimitAddress =  G02Bias + 33,
        TransferSignalHighLimitLenght = 2,
        TransferScaleLowLimitAddress =  G02Bias + 35,
        TransferScaleLowLimitLenght = 2,
        TransferScaleHighLimitAddress =  G02Bias + 37,
        TransferScaleHighLimitLenght = 2,
        BadGoodCommAddress =  G02Bias + 39,
        BadGoodCommLenght = 2,

        UserCalibration1GainAddress =  G02Bias + 42,
        UserCalibration1GainLenght = 2,
        UserCalibration1OffsetAddress =  G02Bias + 44,
        UserCalibration1OffsetLenght = 2,
        UserCalibration1DateAddress =  G02Bias + 46,
        UserCalibration1DateLenght = 2,
        UserCalibration2GainAddress =  G02Bias + 48,
        UserCalibration2GainLenght = 2,
        UserCalibration2OffsetAddress =  G02Bias + 50,
        UserCalibration2OffsetLenght = 2,
        UserCalibration2DateAddress =  G02Bias + 52,
        UserCalibration2DateLenght = 2,
        UserCalibration3GainAddress =  G02Bias + 54,
        UserCalibration3GainLenght = 2,
        UserCalibration3OffsetAddress =  G02Bias + 56,
        UserCalibration3OffsetLenght = 2,
        UserCalibration3DateAddress =  G02Bias + 58,
        UserCalibration3DateLenght = 2,
        UserCalibration4GainAddress =  G02Bias + 60,
        UserCalibration4GainLenght = 2,
        UserCalibration4OffsetAddress =  G02Bias + 62,
        UserCalibration4OffsetLenght = 2,
        UserCalibration4DateAddress =  G02Bias + 64,
        UserCalibration4DateLenght = 2,

        FactoryCalibration1GainAddress =  G02Bias + 66,
        FactoryCalibration1GainLenght = 2,
        FactoryCalibration1OffsetAddress =  G02Bias + 68,
        FactoryCalibration1OffsetLenght = 2,
        FactoryCalibration1DateAddress =  G02Bias + 70,
        FactoryCalibration1DateLenght = 2,
        FactoryCalibration2GainAddress =  G02Bias + 72,
        FactoryCalibration2GainLenght = 2,
        FactoryCalibration2OffsetAddress =  G02Bias + 74,
        FactoryCalibration2OffsetLenght = 2,
        FactoryCalibration2DateAddress =  G02Bias + 76,
        FactoryCalibration2DateLenght = 2,
        FactoryCalibration3GainAddress =  G02Bias + 78,
        FactoryCalibration3GainLenght = 2,
        FactoryCalibration3OffsetAddress =  G02Bias + 80,
        FactoryCalibration3OffsetLenght = 2,
        FactoryCalibration3DateAddress =  G02Bias + 82,
        FactoryCalibration3DateLenght = 2,
        FactoryCalibration4GainAddress =  G02Bias + 84,
        FactoryCalibration4GainLenght = 2,
        FactoryCalibration4OffsetAddress =  G02Bias + 86,
        FactoryCalibration4OffsetLenght = 2,
        FactoryCalibration4DateAddress =  G02Bias + 88,
        FactoryCalibration4DateLenght = 2,

        MeasuredCJValueAddress =  G02Bias + 90,
        MeasuredCJValueLenght = 2,
        ResultCJValueAddress =  G02Bias + 92,
        ResultCJValueLenght = 2,

        AdditionalParameterAddress =  G02Bias + 14,
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

    enum TermoResistorType{
        Pt1dot3910 = 1,
        Pt1dot3850  = 2,
        Cu1dot4280 = 3,
        Cu1dot4260  = 4,
        Ni1dot6170 = 5,
        m5m3m = 6,
        PP46P = 7
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
    Q_ENUM(Errors)
};

#endif // UARTDRIVER_H
