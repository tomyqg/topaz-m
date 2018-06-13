#ifndef WORKER_H
#define WORKER_H

#include <QQueue>
#include "uartdriver.h"
#include "Channels/channelOptions.h"
#include "device.h"
#include "src/modbus.h"
#include "transaction.h"

#define TOTAL_BAD_TR_MODBAS 5       //количество ошибок до фиксации в журнале
#define MODBUS_TIMEVAL_MKS  200000  //время ожидания огтвета от слэйва
#define DEBUG_WORKER

typedef struct
{
    int state;      //0-нома, 1-нет ответа от устройства
    int cntBad;     //счётчик ошибок
    int cntBadCurr; //текущий счётчик ошибок до фиксации
    int cntGood;    //счётчик удачных посылок
}
typeStateSlave;

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
    void finished();
    void ModbusConnectionError();
    void sendTrans(Transaction tr);
    void sendMessToLog(QString mess);

public slots:
    void run();
    void getTransSlot(Transaction tr);

private slots:
//    void do_Work();
    int sendModbusRequest( int slave, int func, int addr, int num, int state, const uint16_t *data_src, uint32_t *data_dest);
    int ReadModbusData(uint8_t sl, const deviceparametrs* dp, uint32_t *data_dest);
    int WriteModbusData(uint8_t sl, const deviceparametrs * dp, uint32_t * data32);
    void OpenSerialPort( int );


private:
    bool isrunning,isstopped;
    bool m_running;
    bool fQueueOver1000;    //признак переполнения очереди
    modbus_t * m_modbus;
    QList<ChannelOptions *> ChannelsObjectsList;
    QMutex mQueue;
    QList<QVector<double> *> arrayarray;
    QList< Device * > Devices;
    Device device;
    QMutex ReadModbusDataMutex;
    QMutex TestMutex;
    QQueue<Transaction> trans;
    typeStateSlave slaves[6];

};

#endif // WORKER_H
