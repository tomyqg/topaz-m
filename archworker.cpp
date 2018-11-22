#include "archworker.h"
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QDataStream>
#include <sleep.h>
#include <QThread>

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

    int daysPeriod = firstTime.daysTo(curTime);

    // "тупой" перебор файлов по именам, содержащим даты
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
                    emit newTick(tick); //отправка тика в родительский класс
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
    emit finished();
}
