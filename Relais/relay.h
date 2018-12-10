#ifndef CRELAY_H
#define CRELAY_H

#include <QObject>

class cRelay : public QObject
{
    Q_OBJECT
public:
    explicit cRelay(uint8_t numDev, uint8_t slot, QObject *parent = 0);
    void setState(bool f);
    bool getState();
    bool confirmedState;
    bool getCurState() {
        return curState;
    }

    uint8_t mySlot;
    uint8_t myPhysicalNum;
    uint8_t type;           // 0 - нормальноразомкнутое, 1 - нормальнозамкнутое

signals:
    void signalSwitch(uint8_t sl, uint8_t num, bool state);

public slots:
    void setCurState(bool state);

private:
    bool oldState;
    bool newState;
    bool curState;      //текщее состояние из Modbus
};

#endif // CRELAY_H
