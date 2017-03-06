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
    double readchannelvalue(int channel);
    float readchannel1value(int channel);
    void writechannelvalue(int channel, double value);
    void readuart();
    void just();

    void writedata();
    QString readalluartports();
    QString ReadAllUartStringData();
    QByteArray ReadAllUartByteData();

private:
    static double channeltempbuffer[4];
    void delay(int n);

public:
    static double channelinputbuffer[4];
    //    explicit  UartDriver();
};

#endif // UARTDRIVER_H
