#ifndef CRELAYSLOTCONNECTOR_H
#define CRELAYSLOTCONNECTOR_H

#include <QObject>

class cRelaySlotConnector : public QObject
{
    Q_OBJECT
public:
    explicit cRelaySlotConnector(int userR, int devR, int slot, QObject *parent = 0);
    int userRelay;
    int deviceRelay;
    int numSlot;
signals:

public slots:
};

#endif // CRELAYSLOTCONNECTOR_H
