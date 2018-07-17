#ifndef CSTEELCONTROLLER_H
#define CSTEELCONTROLLER_H

#include <QObject>
#include <steel_connect.h>

class cSteelController : public QObject
{
    Q_OBJECT
public:
    explicit cSteelController(QObject *parent = 0);
    ~cSteelController();
    int addSteelSlot(int userS, int devS, int slot);
    int getDevSteel(int userS);
    int getSlotBySteel(int userS);
    bool isConnect();

signals:

public slots:

private:
    QList<cSteelConnect *> listSt;
};

#endif // CSTEELCONTROLLER_H
