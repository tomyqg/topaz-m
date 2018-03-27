#include <QFile>
#include <QDebug>
#include "driveri2c.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
//#include <stdio.h>
//#include <stdlib.h>

cDriverI2C::cDriverI2C()
{

}

int cDriverI2C::init(int port)
{
    if(port == 1)
        fd = open("/dev/i2c-1", O_RDWR);
    else if (port == 2)
        fd = open("/dev/i2c-2", O_RDWR);
    else return 0;

    if(fd < 0) {
#ifdef DEBUG_I2C
        qDebug() << "Error opening I2C:" << strerror(errno);
#endif
        return 0;
    }

    return 1;

}

int cDriverI2C::send(unsigned char addr, unsigned char * data)
{
    if(ioctl(fd, I2C_SLAVE, addr) < 0) {
#ifdef DEBUG_I2C
        qDebug() << "ioctl error:" << strerror(errno);
#endif
        return 0;
    }
    if(write(fd, data, 1) != 1)
    {
#ifdef DEBUG_I2C
        qDebug() << "Error writing I2C:" << strerror(errno);
#endif
        return 0;
    }
    return 1;
}

int cDriverI2C::receive(unsigned char addr, unsigned char * data)
{
    if(ioctl(fd, I2C_SLAVE, addr) < 0) {
#ifdef DEBUG_I2C
        qDebug() << "ioctl error:" << strerror(errno);
#endif
        return 0;
    }
    data[0] = 0xFF;
    write(fd, data, 1);

    if(read(fd, data, 1) != 1)
    {
#ifdef DEBUG_I2C
        qDebug() << "Error reading I2C:" << strerror(errno);
#endif
        return 0;
    }
    return 1;
}

void cDriverI2C::detect()
{
    unsigned char buff[255];
    memset(buff, 0, sizeof(buff));
    char str[1000];
    for(int addr = 0; addr <= 127; addr++)
    {
        if(!receive(addr, &buff[addr]))
        {
            buff[addr+128] = buff[addr];
        }

    }
    qDebug() << "     0  1  3  4  5  6  7  8  9  a  b  c  d  e  f";
    for(unsigned char y = 0; y < 8; y++)
    {
        sprintf(str, "%02X:", y<<4);
        for(unsigned char x = 0; x < 0xF; x++)
        {
            if(buff[y*16+x+128] == 0xFF)
                sprintf(str, "%s --", str);
            else
                sprintf(str, "%s %02X", str, buff[y*16+x]);
        }
        qDebug() << str;
    }
}

