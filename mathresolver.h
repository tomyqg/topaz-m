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

    static double dGetAverageValue();


};



#endif // MATHRESOLVER_H
