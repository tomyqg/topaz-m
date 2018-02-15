#include "archivator.h"
#include <QDateTime>

cArchivator::cArchivator(QString file, QListIterator<ChannelOptions*>& ch, QObject *parent) : QObject(parent)
{
    fileName = file;
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
    QFile out(fileName);
    if(out.open(QIODevice::Append))
    {
        QDataStream stream(&out);
        stream << tick.time;
        stream.writeRawData((const char *) tick.channel, sizeof(tick.channel));
        out.close();
    }
}
