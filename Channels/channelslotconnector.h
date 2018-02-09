#ifndef CCHANNELSLOTCONNECTOR_H
#define CCHANNELSLOTCONNECTOR_H

#include <QObject>

class cChannelSlotConnector : public QObject
{
    Q_OBJECT
public:
    explicit cChannelSlotConnector(int userCh, int devCh, int slot, QObject * parent = 0);
//    void addChannelSlot(int userCh, int devCh, int slot);
    int userChannel;
    int deviceChannel;
    int numSlot;

signals:

public slots:

private:

};

#endif // CCHANNELSLOTCONNECTOR_H
