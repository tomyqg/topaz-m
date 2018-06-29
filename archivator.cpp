#include "archivator.h"
#include <QDateTime>
#include <QDebug>
#include <assert.h>
#include <QProcess>
#include <QDir>
#include <defines.h>

cArchivator::cArchivator(QString file, QListIterator<ChannelOptions*>& ch, QObject *parent) : QObject(parent)
{
    time2000 = QDateTime::fromString(STR_DATE_2000, FORMAT_STR_DATE_2000);
    QStringList sl = file.split(".");
    int n = sl.size();
    assert(n>1);
    if(n <= 1) return;
    fileName_l = sl.at(n-2  ) + "_l." + sl.at(n-1);
    fileName_r = sl.at(n-2) + "_r." + sl.at(n-1);
    fileName_sek = sl.at(n-2) + "_sek." + sl.at(n-1);
    fileName_10min = sl.at(n-2) + "_10min." + sl.at(n-1);
    currBank = BANK_R;
    while(ch.hasNext())
        channels.append(ch.next());
    timerAddTick = new QTimer();
    connect(timerAddTick, SIGNAL(timeout()), this, SLOT(addTick()));
    timerAddTick->start(TIME_TICK);
    timerSek = new QTimer();
    connect(timerSek, SIGNAL(timeout()), this, SLOT(addTickSek()));
    timerSek->start(TIME_SEK);
    timer10Min = new QTimer();
    connect(timer10Min, SIGNAL(timeout()), this, SLOT(addTick10Min()));
    timer10Min->start(TIME_10MIN);
}

void cArchivator::addTick()
{
//    QDateTime time2000 = QDateTime::fromString(STR_DATE_2000, FORMAT_STR_DATE_2000);
    sTickCh tick;
    tick.time = time2000.secsTo(QDateTime::currentDateTime());   //секунд с начала 2000 года по текущий момент
    for(int i = 0; i < TOTAL_NUM_CHANNELS; i++)
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

}

void cArchivator::addTickSek()
{

    // --- Проверка смены суток -----------------------------------------
    QFile out(fileName_sek);
    if(out.open(QIODevice::ReadWrite))
    {
        QDataStream stream(&out);
        timeStamp2000 time;
        stream >> time;
        QDateTime archiveTime = time2000.addSecs(time);
        if(archiveTime.date() != QDateTime::currentDateTime().date())
        {
            //если сегодня другой день
            QStringList sl = fileName_sek.split(".");
            int n = sl.size();
            QString fileName_sek_day = sl.at(n-2) \
                    + "_" \
                    + archiveTime.date().toString("yyMMdd.") \
                    + sl.at(n-1);
            out.copy( fileName_sek_day);
            out.resize(0);
        }
        out.close();
    }
    // ---------------------------------------------

    sTickCh tick;
    tick.time = time2000.secsTo(QDateTime::currentDateTime());
    for(int i = 0; i < TOTAL_NUM_CHANNELS; i++)
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

    // --- Посекундный накопительный архив в файле -----------------------------------------
    if(out.open(QIODevice::Append))
    {
        QDataStream stream(&out);
        stream << tick.time;
        stream.writeRawData((const char *) tick.channel, sizeof(tick.channel));
        out.close();
    }
    // ---------------------------------------------
}

void cArchivator::addTick10Min()
{
    QVector<sTickCh> vecTick;
    QDateTime curTime = QDateTime::currentDateTime();
    sTickCh tick10Min;

    // --- Проверка смены года -----------------------------------------
    QFile out(fileName_10min);
    if(out.open(QIODevice::ReadWrite))
    {
        QDataStream stream(&out);
        timeStamp2000 time;
        stream >> time;
        QDateTime archiveTime = time2000.addSecs(time);
        if(archiveTime.toString("yyyy").toInt() != QDateTime::currentDateTime().toString("yyyy").toInt())
        {
            //если сегодня другой год
            QStringList sl = fileName_10min.split(".");
            int n = sl.size();
            QString fileName_10min_day = sl.at(n-2) \
                    + "_" \
                    + archiveTime.date().toString("yyyy.") \
                    + sl.at(n-1);
            out.copy(fileName_10min_day);
            out.resize(0);
        }
        out.close();
    }
    // ---------------------------------------------

    // --- Чтение секундных отсчётов из архива и извлечение последних 10 минут
    QFile fileSek(fileName_sek);
    if(fileSek.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&fileSek);
        sTickCh tick;
        while(!stream.atEnd())
        {
            stream >> tick.time;
            stream.readRawData((char *) tick.channel, sizeof(tick.channel));
            if((tick.time + 600) > time2000.secsTo(curTime))
                vecTick.append(tick);
        }
        fileSek.close();
    }
    // ------------------------------------------------------------------------

    int sizeVecTick = vecTick.size();
    if(sizeVecTick > 0)
    {

        // --- Усреднение за 10 минут по всем каналам -----------------------------
        float channelsSum[TOTAL_NUM_CHANNELS]{0};
        tick10Min.time = time2000.secsTo(curTime);
        foreach (sTickCh tick, vecTick) {
            for(int i = 0; i < TOTAL_NUM_CHANNELS; i++)
                channelsSum[i] += tick.channel[i];
        }
        for(int i = 0; i < TOTAL_NUM_CHANNELS; i++)
            tick10Min.channel[i] = channelsSum[i]/sizeVecTick;
        // -------------------------------------------------------------------------

        // --- Запись в 10-минутный архив ------------------------------------------
        QFile fileMin(fileName_10min);
        if(fileMin.open(QIODevice::Append))
        {
            QDataStream stream(&fileMin);
            stream << tick10Min.time;
            stream.writeRawData((const char *) tick10Min.channel, sizeof(tick10Min.channel));
            fileMin.close();
        }
        // -------------------------------------------------------------------------

    }

}

QVector<double> cArchivator::getVector(int ch, int period, bool timing10min)
{
    assert(ch<TOTAL_NUM_CHANNELS);//запрашиваемый канал не превышает максимальный по счёту
    assert(period<40000000);//период не больше года с хвостиком

    int periodSec = period; //период в секундах
    if(timing10min) periodSec *= 600;   //пересчёт, если задан 10-минутный тайминг

    QVector<double> ret(period, NAN);
    QDateTime curTime = QDateTime::currentDateTime();
    QDateTime firstTime = curTime.addSecs(-periodSec);
    QVector<sTickCh> arrTicks;

    if(!timing10min)
    {
        int daysPeriod = firstTime.daysTo(curTime);
        for(int i = 0; i <= daysPeriod; i++)
        {
            QString strDay = firstTime.addDays(i).toString("yyMMdd.");
            QString fileName;

            if(i == daysPeriod)
                fileName = fileName_sek;
            else
            {
                QStringList sl = fileName_sek.split(".");
                int n = sl.size();
                fileName = sl.at(n-2) + "_" + strDay + sl.at(n-1);
            }

            QFile arch_sek(fileName);
            if(arch_sek.open(QIODevice::ReadOnly))
            {
                QDataStream stream(&arch_sek);
                sTickCh tick;
                while(!stream.atEnd())
                {
                    stream >> tick.time;
                    stream.readRawData((char *) tick.channel, sizeof(tick.channel));
                    arrTicks.append(tick);
                }
                arch_sek.close();
            }
        }
    }
    else
    {   //
        int firstYear = firstTime.toString("yy").toInt();
        int yearsPeriod = curTime.toString("yy").toInt() - firstYear;
        for(int i = 0; i <= yearsPeriod; i++)
        {
            QString strYear = firstTime.addYears(i).toString("yyyy.");
            QString fileName;

            if(i == yearsPeriod)
                fileName = fileName_10min;
            else
            {
                QStringList sl = fileName_10min.split(".");
                int n = sl.size();
                fileName = sl.at(n-2) + "_" + strYear + sl.at(n-1);
            }

            QFile arch_year(fileName);
            if(arch_year.open(QIODevice::ReadOnly))
            {
                QDataStream stream(&arch_year);
                sTickCh tick;
                while(!stream.atEnd())
                {
                    stream >> tick.time;
                    stream.readRawData((char *) tick.channel, sizeof(tick.channel));
                    arrTicks.append(tick);
                }
                arch_year.close();
            }
        }
    }

    foreach(sTickCh tick, arrTicks)
    {
        QDateTime timeArch(time2000.addSecs(tick.time));
        int vectorIndex = firstTime.secsTo(timeArch);
        if(timing10min) vectorIndex /= 600; //пересчёт индекса с учётом тайминга
        if((vectorIndex >= 0) && (vectorIndex < period))
        {
            ret.replace(vectorIndex, tick.channel[ch]);
        }
    }

    return ret;
}
