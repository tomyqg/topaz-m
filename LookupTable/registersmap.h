#ifndef CREGISTERSMAP_H
#define CREGISTERSMAP_H

#include <QObject>
//#include "lookupTables.h"

class cRegistersMap : public QObject
{
    Q_OBJECT
public:
    explicit cRegistersMap(QObject *parent = 0);
    static uint16_t getOffsetByName(QString name);
    static QString getNameByOffset(uint16_t offset);
signals:

public slots:
};

#endif // CREGISTERSMAP_H
