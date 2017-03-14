#ifndef UARTDRIVER_H
#define UARTDRIVER_H

#include <QDialog>
#include <mainwindow.h>
#include <channel1.h>

class UartDriver:public QObject
{
    Q_OBJECT

public slots:
    float readchannel1value(int channel);
    double readchannelvalue(int channel);
    void readuart();
    void writechannelvalue(int channel, double value);
    void writedata();
    QString readalluartports();
    QString ReadAllUartStringData();
    QByteArray ReadAllUartByteData();

private:
    static double channeltempbuffer[4];
    void SetRTS(bool newstate);

protected:
    void delay(int n);
    int  GetXOR(QByteArray bytearray);

public:
    static double channelinputbuffer[4];
};

class ModBus:UartDriver
{
    Q_OBJECT

public slots:
    double ReadTemperature(char channel);

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
        TemperetureAdress   = 0x01,
        VoltageAdress       = 0x02,
        CurrentAdress       = 0x04,
        ResistanceAdress    = 0x08,
        FrequencyAdress     = 0x10
    };

    enum RegisterCount{
        TemperetureRegCount = 0x01,
        VoltageRegCount = 0x01,
        CurrentRegCount = 0x01,
        ResistanceRegCount = 0x01,
        FrequencyRegCount = 0x01
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
