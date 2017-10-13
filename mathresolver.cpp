#include "mathresolver.h"

#include <QtMath>
#include <QtScript/QScriptEngine>
#include "defines.h"

mathresolver::mathresolver()
{
}

double mathresolver::SolveEquation(QString eqstring, double x)
{
    QScriptEngine myEngine;
    QString replaced=eqstring;
    replaced.replace(QString("sin"), QString("Math.sin"));
    replaced.replace(QString("cos"), QString("Math.cos"));
    replaced.replace(QString("sqrt"), QString("Math.sqrt"));
    replaced.replace(QString("pow"), QString("Math.pow"));
    replaced.replace(QString("abs"), QString("Math.abs"));
    replaced.replace(QString("x"), QString::number(x));
    double Result = myEngine.evaluate(replaced).toNumber();



    return Result;
}

double mathresolver::SolveEquation(QString eqstring)
{
    /*QScriptEngine myEngine;
     * доступные функции:
     * сложение: x+1
     * вычитание: x-1
     * синус: cos(x)
     * косинус: sin(x)
     * взятие по модулю: abs(x)
     * число в степени: pow (x, 2)
     * Сдвиньте слайдер вправо
     * чтобы разблокировать
     **/


    QString replaced=eqstring;
    replaced.replace(QString("x"), QString(""));
    return SolveEquation(replaced,0);
}

template< typename T >
T mathresolver::GetAverageValue(QVector<T> & qvect)
//T mathresolver::GetAverageValue()
{

    //    QVector<double> qvect;
    //    qvect.append(3);
    //    qvect.append(10);
    //    qvect.append(15);
    //    qvect.append(30);

    T sum   = 0;
    T averagevalue = 0 ;
    for(T a : qvect)
        sum += a;

    if (qvect.size()>0)
        averagevalue = sum/qvect.size();

    return averagevalue ;
}

double mathresolver::dGetAverageValue(QVector<double>& qvect)
{
    double sum   = 0;
    double averagevalue = 0 ;
    for(double a : qvect)
        sum += a;

    if (qvect.size()>0)
        averagevalue = sum/qvect.size();

    return averagevalue ;
}

double mathresolver::dGetMinimumValue(QVector<double> &qvect)
{
    double minimum = *std::min_element(&qvect[0], &qvect[0] + qvect.length());
    return minimum;
}

double mathresolver::dGetMaximumValue(QVector<double> &qvect)
{
    double maximum = *std::max_element(&qvect[0], &qvect[0] + qvect.length());
    return maximum;
}

double mathresolver::dGetDempheredValue(QVector<double> &qvect, int count)
{
    QVector<double> qvecttemp = qvect;
    double dempheredvalue = 0;

    Q_ASSERT(count <= 0);

    if (count <= 0)
        return 0 ;

    for (int var = 0; var < count; ++var) {
        if (qvecttemp.isEmpty())
            break;
        dempheredvalue += qvecttemp.last();
        qvecttemp.removeLast();
    }

    dempheredvalue /= count;
    return dempheredvalue;
}
