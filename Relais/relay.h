#ifndef CRELAY_H
#define CRELAY_H

#include <QObject>

class cRelay : public QObject
{
    Q_OBJECT
public:
    explicit cRelay(uint8_t numDev, uint8_t slot, QObject *parent = 0);
    bool newState;
    bool oldState;
    bool confirmedState;
    uint8_t mySlot;
    uint8_t myPhysicalNum;

signals:

public slots:
};

#endif // CRELAY_H
