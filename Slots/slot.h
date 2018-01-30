/*
 * Класс описывает слот с подключенной платой
 */

#ifndef SLOT_H
#define SLOT_H

#include <QObject>

#define MAX_NUM_SLOTS 6

class Slot : public QObject
{
    Q_OBJECT
public:
    explicit Slot(QObject *parent = 0)
    {
        needConfig = true;
        fOnline = false;
        num = 0;
        state = 0;
        deviceType = 0;
        deviceStatus = 0;
    }
    ~Slot()
    {

    }
    int getNum() { return num; }
    bool getNeedConfig() { return needConfig; }
    int getState() { return state; }
    int getType() { return deviceType; }
    void setType(int type) { deviceType = type; }
    void initSlot(int n, int type);
    bool isOnline() { return fOnline; }
    void updOnline(bool online) { fOnline = online; }
    void setStatus(int st) { deviceStatus = st; }
    int getStatus() { return deviceStatus; }

signals:

public slots:

private:
    int num;
    bool fOnline;
    bool needConfig;
    int state;
    int deviceType;
    int deviceStatus;

};

#endif // SLOT_H
