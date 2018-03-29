#ifndef CCHANNELSLOTCONTROLLER_H
#define CCHANNELSLOTCONTROLLER_H

#include <QObject>
#include "channelslotconnector.h"

class cChannelSlotController : public QObject
{
    Q_OBJECT
public:
    explicit cChannelSlotController(QObject *parent = 0);
    ~cChannelSlotController();
    int addChannelSlot(int userCh, int devCh, int slot);
    int getDevChannel(int userCh);
    int getSlotByChannel(int userCh);
signals:

public slots:

private:
    QList<cChannelSlotConnector *> listCS;
};

#endif // CCHANNELSLOTCONTROLLER_H
