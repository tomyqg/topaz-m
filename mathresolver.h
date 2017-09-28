#ifndef MATHRESOLVER_H
#define MATHRESOLVER_H
#include <QTimer>
#include <QTime>
#include <QtMath>
#include <QFile>
#include <QtScript/QScriptEngine>

class mathresolver
{
public:
    mathresolver();
    double SolveEquation(QString eqstring, double x);
    double SolveEquation(QString eqstring);

    template< typename T >
    static T GetAverageValue();
//    static T GetAverageValue(QVector<T> & qvect);

};



#endif // MATHRESOLVER_H
