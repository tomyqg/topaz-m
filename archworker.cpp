#include "archworker.h"
#include <QFile>
#include <QDateTime>
#include <QDataStream>
#include <sleep.h>
#include <QThread>

cArchWorker::cArchWorker(QString file, QObject *parent) : QObject(parent)
{
    fileArch = file;
}

void cArchWorker::run()
{
    int periodSec = period; //период в секундах

    QDateTime curTime = QDateTime::currentDateTime();
    QDateTime firstTime = curTime.addSecs(-periodSec);

    int daysPeriod = firstTime.daysTo(curTime);
    for(int i = 0; i <= daysPeriod; i++)
    {
        QString strDay = firstTime.addDays(i).toString("yyMMdd.");
        QString fileName;

        if(i == daysPeriod)
            fileName = fileArch;
        else
        {
            QStringList sl = fileArch.split(".");
            int n = sl.size();
            fileName = sl.at(n-2) + "_" + strDay + sl.at(n-1);
        }

        QFile arch_sek(fileName);
        if(arch_sek.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&arch_sek);
            sTickCh tick;
            int count = 0;
            while(!stream.atEnd())
            {
                stream >> tick.time;
                stream.readRawData((char *) tick.channel, sizeof(tick.channel));
                emit newTick(tick);
                count++;
                if(count >= 10000)
                {
                    count = 0;
//                    Sleep::msleep(1);
                    this->thread()->usleep(1000);
                }
            }
            arch_sek.close();
        }
//        Sleep::msleep(1);
        this->thread()->usleep(10000);
    }
    emit finished();
}
