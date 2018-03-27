#ifndef WIN32

#ifndef CDRIVERSPI_H
#define CDRIVERSPI_H

#include <linux/spi/spidev.h>
#include <linux/poll.h>
#include <sys/epoll.h>

typedef enum
{
    SPI_CS0 = 0,
    SPI_CS1 = 1,
} csSPI_t;

void spi_init(csSPI_t cs, unsigned char mode, unsigned char bits, unsigned speed);
int spi_trans(unsigned char * tx, unsigned char * rx, unsigned short len);

#endif // CDRIVERSPI_H

#endif
