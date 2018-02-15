#ifndef CARCHIVATOR_H
#define CARCHIVATOR_H

#include <QFile>
#include <QDataStream>
#include <Channels/channelOptions.h>
#include <QListIterator>

#define TOTAL_NUM_CHANELS 24
#define TIME_TICK 1000 //время записи в архив

// время в 32битном формате
typedef  uint32_t timeStamp2000;

#pragma pack(push, 1)
typedef struct
{
    timeStamp2000 time;
    float channel[TOTAL_NUM_CHANELS];
} sTickCh;
#pragma pack(pop)

class cArchivator : public QObject
{
    Q_OBJECT
public:
    explicit cArchivator(QString file, QListIterator<ChannelOptions*>& ch, QObject *parent = 0);
signals:

private slots:
    void addTick();

private:
    QString fileName;
    QList <ChannelOptions*> channels;
    QTimer * timerAddTick;
};

#endif // CARCHIVATOR_H
