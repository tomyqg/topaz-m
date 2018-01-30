#ifndef CRELAYSLOTCONTROLLER_H
#define CRELAYSLOTCONTROLLER_H

#include <QObject>
#include "relayslotconnector.h"

class cRelaySlotController : public QObject
{
    Q_OBJECT
public:
    explicit cRelaySlotController(QObject *parent = 0);
    int addRelaySlot(int userR, int devR, int slot);
    int getDevRelay(int userR);
    int getSlotByRelay(int userR);
signals:

public slots:

private:
    QList<cRelaySlotConnector *> listRS;
};

#endif // CRELAYSLOTCONTROLLER_H
