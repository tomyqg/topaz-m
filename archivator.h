#ifndef CARCHIVATOR_H
#define CARCHIVATOR_H

#include <QFile>
#include <QDataStream>
#include <Channels/channelOptions.h>
#include <QListIterator>

#define TOTAL_NUM_CHANELS 24
#define TIME_TICK 1000 //время записи в архив
#define MAX_SIZE_BANK 131072
#define STR_DATE_2000           "2000/01/01 00:00:00"
#define FORMAT_STR_DATE_2000    "yyyy/MM/dd hh:mm:ss"

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
    QVector<double> getVector(int ch, int period);
signals:

private slots:
    void addTick();

private:
    QString fileName_l;
    QString fileName_r;
    QString fileName_sek;
    QString fileName_10min;

    QList <ChannelOptions*> channels;
    QTimer * timerAddTick;
    uint8_t currBank;

};

#endif // CARCHIVATOR_H
