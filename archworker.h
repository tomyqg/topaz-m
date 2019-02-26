#ifndef CARCHWORKER_H
#define CARCHWORKER_H

#include <QObject>
#include <QVector>
#include <archticks.h>

class cArchWorker : public QObject
{
    Q_OBJECT
public:
    explicit cArchWorker(QString file, QObject *parent = 0);
    void setPeriod(int p) {period = p;}
    void setShift(int s) { shift = s; }
    QVector<sTickCh> vecTicks;

private:
    QString fileArch;
    int period;
    int shift;

signals:
    void finished();
    void newTick(sTickCh tick);
    void sendVectorsTicks(QVector<sTickCh> vec);

public slots:
    void run();



};

#endif // CARCHWORKER_H
