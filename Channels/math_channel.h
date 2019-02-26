#ifndef CMATHCHANNEL_H
#define CMATHCHANNEL_H

#include <QObject>
#include "../mathresolver.h"

class cMathChannel : public QObject
{
    Q_OBJECT
public:
    explicit cMathChannel(QObject *parent = 0);
    void setNum(int n) { num = n; }
    int getNum() { return num; }
    QString getName() { return name; }
    void setName(QString str) { name = str; }
    QString GetMathString();
    void SetMathEquation(QString newmathstring);
    double GetCurrentMathValue();
    int numChannel[4];      //номера подключенных каналов в качестве аргументов формулы
    double GetValuePercent();
    double GetHigherMeasureLimit();
    double GetLowerMeasureLimit();
signals:

public slots:

private:
    int num;                // номер матканала
    QString name;  // идентификатор уставки
    QString mathstring; //формула
    double higherLimit;
    double lowerLimit;
};

#endif // CMATHCHANNEL_H
