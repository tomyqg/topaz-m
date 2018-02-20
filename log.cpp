#include "log.h"
#include <QJsonDocument>
#include <QJsonObject>
//#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <assert.h>
#include <QDebug>

cLogger::cLogger(QString file, QObject *parent) : QObject(parent)
{
    logFile = file;

}

void cLogger::addMess(QString mess)
{
    // --------- Упаковка сообщения в Json ----------------------
    QJsonObject message;
    QDateTime local (QDateTime::currentDateTime());
    message["T"] = local.time().toString();
    message["D"] = local.date().toString("dd/MM/yy");
    message["M"] = mess;
    QString strMess = QJsonDocument(message).toJson(QJsonDocument::Compact);
    // --------------------------------------------------------

    qLog.enqueue(strMess);      //запись сообщения в очередь - использовать когда файл будет доступен

    QFile file(logFile);

    //а не открыт ли файл. Функцию addMess() могло вызвать и другое событие.
    if(file.isOpen()) return;  //выходим. Сообщение запишем в файл позже (из очереди)

    if(file.size() >= MAX_SIZE_FILE_LOG)   //огранчиваем размер файла в 100КБ
    {
        int countLine = 0;      //счётчик строчек (сообщений)
        QString line;           //строчка из файла
        QList <QString> ll;     //список строк

        if(file.open(QIODevice::ReadWrite))     //прочитаем и перезапишем уменьшенный файл
        {

            //----------- Чтение строк из файла в список ----------------------
            QTextStream stream(&file);
            do
            {
                line = stream.readLine();
                if(!line.isNull()) ll.append(line);
                //                    {
                //                        countLine++;
                //                        doc = QJsonDocument::fromJson(line.toUtf8());
                //                        jobj = doc.object();
                //                        jarr.append(jobj);
                //                    }
            } while(!line.isNull());
            // --------------------------------------------------------

            countLine = ll.size();      //число строк
            file.resize(0);             //очистка файла

            //----------- Перезапись укороченного списка строк в файл -----
            for(int i = countLine/2; i < countLine; i++)
            {
                //                    jobj = jarr.at(i).toObject();
                //                    line = QJsonDocument(jobj).toJson(QJsonDocument::Compact);
                line = ll.at(i);
                stream << line << endl;
            }
            // ------------------------------------------------------------

            file.close();   // не забыть за собой закрыть файл
        }
    }


    if(file.open(QFile::Append))
    {
        QTextStream out(&file);
        if(qLog.size() > 100)
        {
            qDebug() << "!!! qLog > 100 !!!";
            assert(0);
        }
        while(!qLog.isEmpty())
        {
            strMess = qLog.dequeue();
            out << strMess << endl;
        }
        file.close();
    }

}


QJsonArray cLogger::MessRead()
{
    QFile file(logFile);
    while(file.isOpen());
    QTextStream in(&file);
    QString line;
    QJsonDocument doc;
    QJsonObject jobj;
    QJsonArray jarr;

    if(!file.open(QFile::ReadOnly)) return jarr;

    do
    {
        line = in.readLine();
        if(!line.isNull())
        {
            doc = QJsonDocument::fromJson(line.toUtf8());
            jobj = doc.object();
            jarr.append(jobj);
        }
    } while(!line.isNull());

    return jarr;
}
