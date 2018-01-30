/*
 * Класс хранит конфигурацию слотов
 */


#ifndef CSLOTSCONFIG_H
#define CSLOTSCONFIG_H

#include <QObject>
#include <QList>
#include <QTimer>
#include "slot.h"
#include "transaction.h"

#define UpdateConfigTimer 10000    // период мсек обновления конфигурации с плат
#define DEBAG_SLOT_CONFIG
#define TOTAL_NUM_SLOTS 6

class cSlotsConfig : public QObject
{
    Q_OBJECT
public:
    explicit cSlotsConfig(QObject *parent = 0);
    ~cSlotsConfig();
    int getCountSlots();
    int getTypeDevice(int slot);
    bool isConfFinished();      //нужно возвращать какой-то признак, что все слоты сконфигурированы
    bool isSlotSet(int slot);
    int getStatusDev(int sl);

signals:
    void sendRequest(Transaction tr);

public slots:
    void receiveConf(Transaction tr);

private slots:
    void updConfig();

private:
    int countSlots;
    int stateUpdConf;
    QList<Slot*> listSlots;
    QTimer * timerUpdConfig;
    void addSlot(int num, int type);
    void setStatusDev(int sl, int st);
};

#endif // CSLOTSCONFIG_H
