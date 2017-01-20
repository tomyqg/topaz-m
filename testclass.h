#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

class testclass : public QObject
{
public:

    testclass();

    void setprop1();

private:

    int qwe = 33;

};

#endif // TESTCLASS_H
