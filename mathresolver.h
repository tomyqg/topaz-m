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
    double Solve(QString eqstring, double x);
    double Solve(QString eqstring);
};

#endif // MATHRESOLVER_H
