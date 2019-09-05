#ifndef CEXTMODBUS_H
#define CEXTMODBUS_H

#include <QObject>
//#include <QString>
#include <QMutex>

#include "src/modbus.h"
#include "modbus_lookup_tables.h"

typedef struct {
    uint8_t data[256];
} tModbusBuffer;

class cExtModbus : public QObject
{
    Q_OBJECT
public:
    explicit cExtModbus(QObject *parent = 0);
    ~cExtModbus();
    int init(int type);

    enum {
        TCP,
        RTU
    };

signals:
    void signalUpdateParam(QString name, tModbusBuffer buffer);
    void signalActualizeParam(QString name);

public slots:
    void run();
    void updateData(QString name, tModbusBuffer buffer);
private:
    int socket;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int use_backend;
    uint8_t *query;
    int header_length;
    int maxNbInputRegisters;
    int maxNbHoldingRegisters;
    int maxNbCoil;
    int maxNbDiscrete;
    QMutex mutex;
    int master_socket;
    fd_set refset;
    fd_set rdset;
    /* Maximum file descriptor number */
    int fdmax;

    const tExtLookupRegisters *getLookupElementByOffsetAndFunc(uint16_t offset, uint8_t func);
    uint8_t updateParam(const void *param);
    void reply();

    bool isAccess(uint8_t func, uint8_t access);
};


#endif // CEXTMODBUS_H




