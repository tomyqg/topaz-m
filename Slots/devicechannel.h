#ifndef CDEVICECHANNEL_H
#define CDEVICECHANNEL_H

#include <QtCore>

typedef struct {
    uint16_t measureType;   // 14
    union                   // 15
    {
        uint16_t TCType;
        uint16_t TRType;
    };
    uint16_t CJType;        // 16
    uint16_t CJChannel;     // 17
    union                   // 18
    {
        float CJValue;
        float RWire;
    };
} typeAdditionalParam1;

typedef struct {
    float RNominal;         // 20
    uint16_t reserv1;       // 22
    uint16_t reserv2;       // 23
    uint16_t reserv3;       // 24
    uint16_t reserv4;       // 25
} typeAdditionalParam2;

class cDeviceChannel : public QObject
{
    Q_OBJECT
public:
    explicit cDeviceChannel(QObject *parent = 0);
    typeAdditionalParam1 additionalParameter1;
    typeAdditionalParam2 additionalParameter2;
signals:

public slots:
};

#endif // CDEVICECHANNEL_H
