#ifndef CSTEELCONNECT_H
#define CSTEELCONNECT_H

#include <QObject>

class cSteelConnect : public QObject
{
    Q_OBJECT
public:
    explicit cSteelConnect(int userS, int devS, int slot, QObject *parent = 0);
    int userSteel;
    int deviceSteel;
    int numSlot;

signals:

public slots:
};

#endif // CSTEELCONNECT_H
