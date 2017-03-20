#include "mathresolver.h"
#include <QtMath>
#include <QtScript/QScriptEngine>

mathresolver::mathresolver()
{
//    float a = Solve("sin(2)",1);
}

double mathresolver::Solve(QString eqstring, double x)
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

double mathresolver::Solve(QString eqstring)
{
    /*QScriptEngine myEngine;
    QString replaced=eqstring;
    replaced.replace(QString("sin"), QString("Math.sin"));
    replaced.replace(QString("cos"), QString("Math.cos"));
    replaced.replace(QString("sqrt"), QString("Math.sqrt"));
    replaced.replace(QString("pow"), QString("Math.pow"));
    replaced.replace(QString("abs"), QString("Math.abs"));
    double Result = myEngine.evaluate(replaced).toNumber();
    return Result;*/
    mathresolver m;
    return m.Solve(eqstring,0);
}
