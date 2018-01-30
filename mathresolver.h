#ifndef MATHRESOLVER_H
#define MATHRESOLVER_H
#include <QTimer>
#include <QTime>
#include <QtMath>
#include <QFile>
#ifndef Q_OS_WIN32
#include <QtScript/QScriptEngine>
#endif

class mathresolver : public QObject
{
public:
    mathresolver();
    static double SolveEquation(QString eqstring, double x);
    static double SolveEquation(QString eqstring);

    template< typename T >
    T GetAverageValue(QVector<T>& qvect2);

    static double dGetAverageValue(QVector<double >& qvect2);
    static double dGetMinimumValue(QVector<double >& qvect);
    static double dGetMaximumValue(QVector<double >& qvect);
    static double dGetDempheredValue(QVector<double >& qvect, int count);
};



#endif // MATHRESOLVER_H
