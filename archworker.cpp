#include "archworker.h"
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QDataStream>
#include <sleep.h>
#include <QThread>
#include <QVector>

#define STR_DATE_2000           "2000/01/01 00:00:00"
#define FORMAT_STR_DATE_2000    "yyyy/MM/dd hh:mm:ss"

cArchWorker::cArchWorker(QString file, QObject *parent) : QObject(parent)
{
    fileArch = file;
    shift = 0;
}

/*
 * Функция загрузки данных из файлов архива в буффер для дальнейшего отображения
 * */
void cArchWorker::run()
{
    int periodSec = period; //период в секундах
    QDateTime curTime = QDateTime::currentDateTime().addSecs(-shift);
    QDateTime firstTime = curTime.addSecs(-periodSec);
    int countAvg[TOTAL_NUM_CHANNELS];
    memset(countAvg, 0, sizeof(countAvg));
    QDateTime time2000 = QDateTime::fromString(STR_DATE_2000, FORMAT_STR_DATE_2000);

    int daysPeriod = firstTime.daysTo(curTime);
    vecTicks.resize(0);


    // "тупой" перебор файлов по именам, содержащим даты
    for(int d = 0; d <= daysPeriod; d++)
    {
        QString strDay = firstTime.addDays(d).toString("yyMMdd.");
        QString fileName;

        if(d == daysPeriod)
            fileName = fileArch;
        else
        {
            QStringList sl = fileArch.split(".");
            int n = sl.size();
            fileName = sl.at(n-2) + "_" + strDay + sl.at(n-1);
        }

        QFile arch_sek(fileName);
        if(arch_sek.exists())   //проверка наличия такого файла
        {
            if(arch_sek.open(QIODevice::ReadOnly))
            {
                QDataStream stream(&arch_sek);
                sTickCh tick;
                int count = 0;
                while(!stream.atEnd())
                {   //чтение файла до конца
                    stream >> tick.time;
                    stream.readRawData((char *) tick.channel, sizeof(tick.channel));
                    if(period < 604800)
                    {
                        vecTicks.append(tick);
                    }
                    else
                    {
//                    emit newTick(tick); //отправка тика в родительский класс
                        int vecSize = vecTicks.size();
                        if((vecSize == 0))
                        {
                            vecTicks.append(tick);
                            memset(countAvg, 0, sizeof(countAvg));
                        }
                        else
                        {
                            sTickCh lastTick = vecTicks.at(vecSize - 1);
                            QDateTime timeNew(time2000.addSecs(tick.time));
//                            QString strTimeNew = timeNew.toString("yyMMddhhmm");
                            QDateTime timeLast(time2000.addSecs(lastTick.time));
//                            QString strTimeLast = timeLast.toString("yyMMddhhmm");

                            if(timeLast.secsTo(timeNew) < 600)
                            {
                                //если отметка времени в диапазоне 10 минут
                                for(int i = 0 ; i < TOTAL_NUM_CHANNELS; i++)
                                {
                                    // проверка значения на действительность
                                    if((tick.channel[i] != 0) && (tick.channel[i] == tick.channel[i]))
                                    {
                                        if(countAvg[i] != 0)
                                        {
                                            lastTick.channel[i] += tick.channel[i];
                                        }
                                        else
                                        {
                                            lastTick.channel[i] = tick.channel[i];
                                        }
                                        countAvg[i]++;
                                    }
                                }
                            }
                            else
                            {
                                //тики из разных 10-минутных диапазонов
                                // усреднение накопленных значений
                                for(int i = 0 ; i < TOTAL_NUM_CHANNELS; i++)
                                {
                                    //проверка на НЕ NAN
                                    if((lastTick.channel[i] == lastTick.channel[i])\
                                            && (countAvg[i] != 0))
                                    {
                                        lastTick.channel[i] = lastTick.channel[i]/countAvg[i];
                                    }
                                    countAvg[i] = 1;

                                }

                                //перезапись последнего элемента
                                vecTicks.replace(vecSize - 1, lastTick);

                                //добавление нового тика
                                vecTicks.append(tick);
                            }
                        }
                    }
                    count++;
                    if(count >= 1000)
                    {   // каждые 10 тыс. тиков освобождать ресурсы процессора на мсек для других задач
                        count = 0;
                        this->thread()->usleep(1000);
                    }
                }
                arch_sek.close();
                // освобождение ресурсов CPU для других задач между сканированием файлов
                this->thread()->usleep(10000);
            }
        }
    }
    emit sendVectorsTicks(vecTicks);
    emit finished();
}
