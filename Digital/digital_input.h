#ifndef CDIGITALINPUT_H
#define CDIGITALINPUT_H

#include <QObject>

class cDigitalInput : public QObject
{
    Q_OBJECT
public:
    explicit cDigitalInput(QObject *parent = 0);

signals:

public slots:

public:
    int function;   //
};

#endif // CDIGITALINPUT_H
