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
    QByteArray ModBusMakeRequest(char deviceadress,char functcode, char startaddress, char registercountforread);

public:
    enum DeviceAdress {
        MainDevice = 0x01
    };
    Q_ENUM(DeviceAdress)

    enum StartAdress {
        ZeroAdress = 0x00,
        AB1Adress = 0x03
    };
    Q_ENUM(StartAdress)

    enum Function {
       /* FunctionOne = 0x0A,
        FunctionTwo = 0x03*/
        ReadCoilsState = 0x01,
        ReadDiscreteInputs = 0x02,
        ReadHoldingfRegisters = 0x03,
        ReadInputRegisters = 0x04,
        SetCoilsState = 0x05,
        StatusDeviceRequest = 0x07,
        DiagnosticRequest = 0x08,
        ReadStatusAndEventsCount = 0x0B,
        ReadEventsJournal = 0x0C
    };
    Q_ENUM(Function)

    enum ByteQueues {
        one = 0x02,
        two = 0x03
    };
    Q_ENUM(ByteQueues)
};

#endif // UARTDRIVER_H
