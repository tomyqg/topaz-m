#ifndef CLOG_H
#define CLOG_H

#include <QObject>
#include <QQueue>
#include <QJsonArray>

#define MAX_SIZE_FILE_LOG (100*1024) //максимальная размер лог файла (100 КБайт)

class cLogger : public QObject
{
    Q_OBJECT
public:
    explicit cLogger(QString file, QObject *parent = 0);
    void addMess(QString mess);
    QJsonArray MessRead();

signals:

public slots:

private:
    QString logFile;
    QQueue <QString> qLog;
};

#endif // CLOG_H
