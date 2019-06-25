#ifndef CRELAY_H
#define CRELAY_H

#include <QObject>
#include <QTimer>

class cRelay : public QObject
{
    Q_OBJECT
public:
    explicit cRelay(uint8_t numDev, uint8_t slot = 0, QObject *parent = 0);

    bool confirmedState;
    bool enable;
    uint8_t mySlot;
    uint8_t myPhysicalNum;
    bool type;           // false - нормальноразомкнутое, true - нормальнозамкнутое

    void setState(bool f);
    bool getState() { return newState; }
    bool getCurState() { return curState; }
    void setCurState(bool state);

signals:
    void signalSwitch(uint8_t sl, uint8_t num, bool state);
    void signalGetState(uint8_t sl, uint8_t num);

public slots:

private:
    bool newState;
    bool curState;      //текщее состояние из Modbus
    QTimer timerUpdate;

private slots:
    void update();
};

#endif // CRELAY_H
