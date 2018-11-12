#ifndef CMATHCHANNEL_H
#define CMATHCHANNEL_H

#include <QObject>

class cMathChannel : public QObject
{
    Q_OBJECT
public:
    explicit cMathChannel(QObject *parent = 0);

signals:

public slots:
};

#endif // CMATHCHANNEL_H