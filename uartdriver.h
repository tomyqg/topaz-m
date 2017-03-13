#ifndef UARTDRIVER_H
#define UARTDRIVER_H

#include <QDialog>
#include <mainwindow.h>
#include <channel1.h>

class UartDriver:public QObject
{
    Q_OBJECT

private slots:

public slots:
    float readchannel1value(int channel);
    double readchannelvalue(int channel);
    void just();
    void readuart();
    void SetRTS(bool newstate);
    void writechannelvalue(int channel, double value);
    void writedata();
    QString readalluartports();
    QString ReadAllUartStringData();
    QByteArray ReadAllUartByteData();
    QByteArray ModBusMakeRequest(char deviceaddress,char functcode, char startaddress, char registercountforread);

private:
    static double channeltempbuffer[4];
    void delay(int n);
    int GetXOR(QByteArray bytearray);

public:
    static double channelinputbuffer[4];
    //    explicit  UartDriver();


};


class ModBus:UartDriver
{
    Q_OBJECT

public:
    enum DeviceAdress {
        MainDevice = 0x01
    };

};

#endif // UARTDRIVER_H
