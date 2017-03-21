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

    enum OutputValue {
    };
    Q_ENUM(OutputValue)

    enum G00_Group{
        DataChannel1Address = 0x00,
        DataChannel2Address = 0x02,
        DataChannel3Address = 0x04,
        DataChannel4Address = 0x06,
        DataChannel1Lenght = 2, //  (2 registers)
        DataChannel2Lenght = 2,
        DataChannel3Lenght = 2,
        DataChannel41Lenght = 2
    };
    Q_ENUM(G00_Group)

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

    };
    Q_ENUM(G01_Group)
};

#endif // UARTDRIVER_H
