#include "archivator.h"
#include <QDateTime>
#include <QDebug>
#include <assert.h>

cArchivator::cArchivator(QString file, QListIterator<ChannelOptions*>& ch, QObject *parent) : QObject(parent)
{
    QStringList sl = file.split(".");
    int n = sl.size();
    assert(n>1);
    if(n <= 1) return;
    fileName_l = sl.at(n-2) + "_l." + sl.at(n-1);
    fileName_r = sl.at(n-2) + "_r." + sl.at(n-1);
    currBank = BANK_R;
    while(ch.hasNext())
        channels.append(ch.next());
    timerAddTick = new QTimer();
    connect(timerAddTick, SIGNAL(timeout()), this, SLOT(addTick()));
    timerAddTick->start(TIME_TICK);
}

void cArchivator::addTick()
{
    QDateTime time2000 = QDateTime::fromString("2000/01/01 00:00:00", "yyyy/MM/dd hh:mm:ss");
    sTickCh tick;
    tick.time = time2000.secsTo(QDateTime::currentDateTime());   //секунд с начала 2000 года по текущий момент
    for(int i = 0; i < TOTAL_NUM_CHANELS; i++)
    {
        if(i < channels.length())
        {
            tick.channel[i] = channels.at(i)->GetCurrentChannelValue();
        }
        else
        {
            tick.channel[i] = 0.0;
        }
    }

    QString file;
    QFile file_l(fileName_l);
    QFile file_r(fileName_r);

    // --- Смена банка, если переполнение --------------------------------
    if((currBank==BANK_L) && ((file_l.size() + sizeof(tick)) >= MAX_SIZE_BANK))
    {
        currBank = BANK_R;
        file = fileName_r;
        file_r.resize(0);
    }
    else if((currBank==BANK_R) && ((file_r.size() + sizeof(tick)) >= MAX_SIZE_BANK))
    {
        currBank = BANK_L;
        file = fileName_l;
        file_l.resize(0);
    }
    if(currBank==BANK_L) file = fileName_l;
    else file = fileName_r;
    // ---------------------------------------------------------------------

    //
    QFile out(file);
    if(out.open(QIODevice::Append))
    {
        QDataStream stream(&out);
        stream << tick.time;
        stream.writeRawData((const char *) tick.channel, sizeof(tick.channel));
        out.close();
    }
}
