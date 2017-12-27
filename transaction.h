#ifndef TRANSACTION_H
#define TRANSACTION_H


class Transaction
{
public:

    typedef enum
    {
        R = 0,
        W = 1
    } dir_t;

    explicit Transaction(dir_t d, uint8_t sl, uint16_t addr, uint32_t vol = 0)
    {
        dir = d;
        slave = sl;
        offset = addr;
        volInt = vol;
    }

    dir_t dir;
    uint8_t slave;
    uint32_t offset;

    union
    {
        uint32_t volInt;
        float volFlo;
    };

signals:

public slots:

};

#endif // TRANSACTION_H
