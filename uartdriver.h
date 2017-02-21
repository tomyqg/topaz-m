#ifndef UARTDRIVER_H
#define UARTDRIVER_H

#include <QDialog>
#include <mainwindow.h>
#include <channel1.h>

class UartDriver:public QObject
{
    Q_OBJECT

private slots:
    void readuart();

public slots:
    double readchannelvalue(int channel);
    void writechannelvalue(int channel, double value);
    void just();

private:
    static double channelinputbuffer[4];

public:
//    explicit  UartDriver();
};

#endif // UARTDRIVER_H

