#ifndef WORKER_H
#define WORKER_H

#include "uartdriver.h"
#include "channel1.h"
#include "device.h"
#include "src/modbus.h"

class worker : public QObject
{
    Q_OBJECT
public:
    explicit worker(QObject *parent = 0);

    ChannelOptions* ThreadChannelOptions1;
    ChannelOptions* ThreadChannelOptions2;
    ChannelOptions* ThreadChannelOptions3;
    ChannelOptions* ThreadChannelOptions4;

signals:
    void SignalToObj_mainThreadGUI();
    void running();
    void stopped();
    void Finished();
    void ModbusConnectionError();

public slots:
    void StopWorkSlot();
    void StartWorkSlot();
    void GetObectsSlot(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4);


private slots:
    void do_Work();
    void sendModbusRequest( int slave, int func, int addr, int num, int state, const uint16_t *data_src, float *data_dest_float);
    void sendModbusRequest(const deviceparametrs* dp);
    void ReadModbusData(const deviceparametrs* dp, float *data_dest);
    void WriteModbusData(const deviceparametrs* dp, float value);
    void OpenSerialPort( int );


private:
    //    ChannelOptions* ThreadChannelOptions1,ThreadChannelOptions2,ThreadChannelOptions3,ThreadChannelOptions4;
    bool isrunning,isstopped;
    modbus_t * m_modbus;
    QList<ChannelOptions *> ChannelsObjectsList;
    QList<QVector<double> *> arrayarray;
    QList< Device * > Devices;
    Device device;
    QMutex ReadModbusDataMutex;
};

#endif // WORKER_H
