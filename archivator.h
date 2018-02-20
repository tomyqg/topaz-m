#ifndef CARCHIVATOR_H
#define CARCHIVATOR_H

#include <QFile>
#include <QDataStream>
#include <Channels/channelOptions.h>
#include <QListIterator>

#define TOTAL_NUM_CHANELS 24
#define TIME_TICK 1000 //время записи в архив
#define MAX_SIZE_BANK 131072

// время в 32битном формате
typedef  uint32_t timeStamp2000;

#pragma pack(push, 1)
typedef struct
{
    timeStamp2000 time;
    float channel[TOTAL_NUM_CHANELS];
} sTickCh;
#pragma pack(pop)

enum
{
    BANK_L,
    BANK_R
};

class cArchivator : public QObject
{
    Q_OBJECT
public:
    explicit cArchivator(QString file, QListIterator<ChannelOptions*>& ch, QObject *parent = 0);
signals:

private slots:
    void addTick();

private:
    QString fileName_l;
    QString fileName_r;
    QList <ChannelOptions*> channels;
    QTimer * timerAddTick;
    uint8_t currBank;
};

#endif // CARCHIVATOR_H
