#include "archivator.h"
#include <QDateTime>
#include <QDebug>
#include <assert.h>
#include <QProcess>
#include <QDir>
#include <defines.h>

cArchivator::cArchivator(QString file, QListIterator<ChannelOptions*>& ch, QObject *parent) : QObject(parent)
{
    QStringList sl = file.split(".");
    int n = sl.size();
    assert(n>1);
    if(n <= 1) return;
    fileName_l = sl.at(n-2) + "_l." + sl.at(n-1);
    fileName_r = sl.at(n-2) + "_r." + sl.at(n-1);
    fileName_sek = sl.at(n-2) + "_sek." + sl.at(n-1);
    fileName_10min = sl.at(n-2) + "_10min." + sl.at(n-1);
    currBank = BANK_R;
    while(ch.hasNext())
        channels.append(ch.next());
    timerAddTick = new QTimer();
    connect(timerAddTick, SIGNAL(timeout()), this, SLOT(addTick()));
    timerAddTick->start(TIME_TICK);
}

void cArchivator::addTick()
{
    QDateTime time2000 = QDateTime::fromString(STR_DATE_2000, FORMAT_STR_DATE_2000);
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

    // --- Запись в текущий архив ----------------------
    QFile out(file);

    if(out.open(QIODevice::Append))
    {
        QDataStream stream(&out);
        stream << tick.time;
        stream.writeRawData((const char *) tick.channel, sizeof(tick.channel));
        out.close();
    }
    // ---------------------------------------------

    QDateTime curDateTime = QDateTime::currentDateTime();
    static QDateTime lastSek = curDateTime;
    if((lastSek.secsTo(curDateTime) > 0) || curDateTime.secsTo(lastSek) > 0)
    {
        //если секунда уже не та ))

        // --- Проверка смены суток -----------------------------------------
        out.setFileName(fileName_sek);
        if(out.open(QIODevice::ReadWrite))
        {
            QDataStream stream(&out);
            timeStamp2000 time;
            stream >> time;
            QDateTime archiveTime = time2000.addSecs(time);
            if(archiveTime.date() != curDateTime.date())
            {
                //если сегодня другой день
                QStringList sl = fileName_sek.split(".");
                int n = sl.size();
                QString fileName_sek_day = sl.at(n-2) \
                        + archiveTime.date().toString("yyMMdd.") \
                        + sl.at(n-1);
                out.copy( fileName_sek_day);
                out.resize(0);
            }
            out.close();
        }
        // ---------------------------------------------

        // --- Посекундный накопительный архив в файле -----------------------------------------
        out.setFileName(fileName_sek);
        if(out.open(QIODevice::Append))
        {
            QDataStream stream(&out);
            stream << tick.time;
            stream.writeRawData((const char *) tick.channel, sizeof(tick.channel));
            out.close();
        }
        // ---------------------------------------------

        lastSek = curDateTime;
    }
}

QVector<double> cArchivator::getVector(int ch, int period)
{
    assert(ch<TOTAL_NUM_CHANELS);//запрашиваемый канал не превышает максимальный по счёту
    assert(period<40000000);//период не больше года с хвостиком

    QVector<double> ret(period, 0);
    QDateTime curTime = QDateTime::currentDateTime();
    QDateTime firstTime = curTime.addSecs(-period);
    QVector<sTickCh> arrTicks;

    if(firstTime.date() == curTime.date())
    {   //период выбран в пределах текущих суток - данные все в одном файле
        QFile arch_sek(fileName_sek);
        if(arch_sek.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&arch_sek);
            sTickCh tick;
            for(int i = 0; (i < period) && (!stream.atEnd()); i++)
            {   //чтение всего файла в свой буффер
                stream >> tick.time;
                stream.readRawData((char *) tick.channel, sizeof(tick.channel));
                arrTicks.append(tick);
            }
            arch_sek.close();
        }
    }
    else
    {   //период охватывает предыдущие сутки

    }

    QDateTime time2000 = QDateTime::fromString(STR_DATE_2000, FORMAT_STR_DATE_2000);
    foreach(sTickCh tick, arrTicks)
    {
        QDateTime timeArch(time2000.addSecs(tick.time));
        int vectorIndex = firstTime.secsTo(timeArch);
        ret.replace(vectorIndex, tick.channel[ch]);
    }
    return ret;
}
