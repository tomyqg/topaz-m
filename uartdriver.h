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
        TemperetureAdressLo   = 0x00,
        VoltageAdressLo       = 0x00,
        CurrentAdressLo       = 0x00,
        ResistanceAdressLo    = 0x00,
        FrequencyAdressLo     = 0x00
    };

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

    Q_ENUM(RegisterAdress)

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

    enum ByteQueues {
        Fone = 0x02,
        Ftwo = 0x03
    };
    Q_ENUM(ByteQueues)
};

#endif // UARTDRIVER_H
