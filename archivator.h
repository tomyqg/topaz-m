#ifndef CARCHIVATOR_H
#define CARCHIVATOR_H

#include <QFile>
#include <QDataStream>
#include <Channels/channelOptions.h>
#include <QListIterator>
#include <archticks.h>
#include <archworker.h>
//#include <QtSql/QSqlDatabase>
//#include <QDateTime>

#define TIME_TICK 200 //период записи в архив
#define TIME_SEK 1000 //период записи в посекундный архив
#define TIME_10MIN 600000 //период записи в 10минутный архив
#define MAX_SIZE_BANK 131072
#define STR_DATE_2000           "2000/01/01 00:00:00"
#define FORMAT_STR_DATE_2000    "yyyy/MM/dd hh:mm:ss"



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
    ~cArchivator();
    void load(int per, int shift = 0);
    QVector<double> getVector(int ch);

signals:
    void loadFinished();

private slots:
    void addTick();
    void addTickSek();
    void addTick10Min();
    void addLoadTickFromFile(sTickCh tick);
    void endLoad();
    void slotVectorTicks(QVector<sTickCh> ticks);

private:
    QString fileName_l;
    QString fileName_r;
    QString fileName_sek;
    QString fileName_10min;

    QList <ChannelOptions*> channels;
    QTimer * timerAddTick;
    QTimer * timerSek;
    QTimer * timer10Min;
    uint8_t currBank;
    QDateTime time2000;
    QDateTime askTime;
    cArchWorker * worker;
    QThread * threadReadFile;
    QVector<sTickCh> arrTicks;
    int period;
    int shiftPeriod;
};

#endif // CARCHIVATOR_H
