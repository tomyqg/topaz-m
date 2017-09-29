#ifndef MATHRESOLVER_H
#define MATHRESOLVER_H
#include <QTimer>
#include <QTime>
#include <QtMath>
#include <QFile>
#include <QtScript/QScriptEngine>

class mathresolver : public QObject
{
public:
    mathresolver();
    double SolveEquation(QString eqstring, double x);
    double SolveEquation(QString eqstring);

    template< typename T >
    T GetAverageValue(QVector<T>& qvect2);
    double dGetAverageValue(QVector<double >& qvect2);

    //static T GetAverageValue();

};



#endif // MATHRESOLVER_H
