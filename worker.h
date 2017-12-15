#ifndef WORKER_H
#define WORKER_H

#include <QQueue>
#include "uartdriver.h"
#include "channelOptions.h"
#include "device.h"
#include "src/modbus.h"

class worker : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

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
    void finished();
    void ModbusConnectionError();
    void sendTrans(transaction * tr);

public slots:
    void StopWorkSlot();
    void StartWorkSlot();
    void GetObectsSlot(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4);
    void run();
    void getTransSlot(transaction * tr);

private slots:
    void do_Work();
    void sendModbusRequest( int slave, int func, int addr, int num, int state, const uint16_t *data_src, float *data_dest_float);
    void sendModbusRequest(const deviceparametrs* dp);
    void ReadModbusData(const deviceparametrs* dp, float *data_dest);
    void WriteModbusData(const deviceparametrs* dp, float value);
    void OpenSerialPort( int );


private:
    bool isrunning,isstopped;
    bool m_running;
    modbus_t * m_modbus;
    QList<ChannelOptions *> ChannelsObjectsList;
    QMutex mChList;
    QList<QVector<double> *> arrayarray;
    QList< Device * > Devices;
    Device device;
    QMutex ReadModbusDataMutex;
    QMutex TestMutex;
    QQueue<transaction *> trans;
};

#endif // WORKER_H
