#include "mathresolver.h"
#include <QtMath>

mathresolver::mathresolver()
{
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
    return Solve(replaced,0);
}
