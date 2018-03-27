#ifndef CDRIVERI2C_H
#define CDRIVERI2C_H

#ifndef Q_OS_WIN32
#include <linux/i2c-dev.h>
#endif

//#define DEBUG_I2C

class cDriverI2C
{
public:
    cDriverI2C();
    int init(int port);
    int send(unsigned char addr, unsigned char * data);
    int receive(unsigned char addr, unsigned char * data);
    void detect();

private:
    int fd;
};

#endif // CDRIVERI2C_H
