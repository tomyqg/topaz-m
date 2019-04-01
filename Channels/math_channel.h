#ifndef CMATHCHANNEL_H
#define CMATHCHANNEL_H

#include <QObject>
#include <QMutex>
#include <QVector>
#include <QTimer>
#include "../mathresolver.h"

class cMathChannel : public QObject
{
    Q_OBJECT
public:
    explicit cMathChannel(QObject *parent = 0);
    ~cMathChannel();
    void setNum(int n) { num = n; }
    int getNum() { return num; }
    QString getName() { return name; }
    void setName(QString str) { name = str; }
    QString GetMathString();
    void SetMathEquation(QString newmathstring);
    double GetCurrentMathValue();
    int numChannel[4];      //номера подключенных каналов в качестве аргументов формулы
    double GetValuePercent();
    double GetHigherMeasureLimit() { return higherLimit; }
    double GetLowerMeasureLimit() { return lowerLimit; }
    void SetHigherMeasureLimit(double limit) { higherLimit = limit; }
    void SetLowerMeasureLimit(double limit) { lowerLimit = limit; }
    QVector<double> GetMathValuesBuffer();
    QVector<double> GetMathXBuffer();

signals:

public slots:

private slots:
    void slotUpdate();

private:
    int num;                // номер матканала
    QString name;  // идентификатор уставки
    QString mathstring; //формула
    double higherLimit;
    double lowerLimit;
    double currentvalue;
    QTimer timerUpdate;     //таймер обновления результа

    //Буфферы данных
    QMutex * buffermutex;
    QVector<double> mathvaluesbuffer;
    QVector<double> dempheredvaluesbuffer;
    QVector<double> mathxbuffer;
    QVector<double> mathpercentbuffer;
};

#endif // CMATHCHANNEL_H
