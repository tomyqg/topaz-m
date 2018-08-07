#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QMetaType>

class Transaction
{
public:

    typedef enum
    {
        R = 0,
        W = 1
    } dir_t;

    explicit Transaction(dir_t d = R, uint8_t sl = 1, uint16_t addr = 0, uint32_t vol = 0)
    {
        dir = d;
        slave = sl;
        offset = addr;
        volInt = vol;
//        memset(paramInt16, vol, sizeof(paramInt16));
    }

    dir_t dir;
    uint8_t slave;
    uint32_t offset;

    union
    {
        uint32_t volInt;
        float volFlo;
        uint16_t paramA12[6];
        int16_t paramInt16[32];  //для работы со знаковыми данными / буффером
    };

signals:

public slots:

};

//Q_DECLARE_METATYPE(Transaction)


#endif // TRANSACTION_H
