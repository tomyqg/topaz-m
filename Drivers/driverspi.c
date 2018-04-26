#ifndef WIN32

#include "driverspi.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>


char dev[100];     // name device
unsigned char spimode;    // spi mode
unsigned char spibits;    // bits per word
unsigned spispeed;  // max speed hz
struct spi_ioc_transfer transfer;

static void pabort(const char *s)
{
    perror(s);
    fprintf(stderr, s);
    assert(0);
}

void spi_init(csSPI_t cs, unsigned char mode, unsigned char bits, unsigned speed)
{
    if(cs == SPI_CS0)
    {
        strcpy(dev, "/dev/spidev2.0");
    }
    else if(cs == SPI_CS1)
    {
        strcpy(dev, "/dev/spidev2.1");
    }
    else
    {
        pabort("Error set CS for SPI");
    }
    spimode = mode;
    spibits = bits;
    spispeed = speed;
}

int spi_trans(unsigned char * tx, unsigned char * rx, unsigned short len)
{
    int ret = 0;
    int fd;

    fd = open(dev, O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    ret = ioctl(fd, SPI_IOC_WR_MODE, &spimode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spibits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spispeed);
    if (ret == -1)
        pabort("can't set max speed hz");

    /* full-duplex transfer */
    transfer.tx_buf = (unsigned long)tx;
    transfer.rx_buf = (unsigned long)rx;
    transfer.len = len;
    transfer.delay_usecs = 0;
    transfer.speed_hz = spispeed;
    transfer.bits_per_word = spibits;

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
    if (ret < 1)
        pabort("can't send spi message");

    // received data automatical writed to rx buffer

    close(fd);

    char s[len*3+4+10];
    char s1[len*3+4+10];


    sprintf(s1, "Tx: ");
    for (ret = 0; ret < len; ret++) {
        sprintf(s, "%.2X ", tx[ret]);
        strcat(s1, s);
    }
    strcat(s1, "\n");
    fprintf(stderr, s1);

    sprintf(s1, "Rx: ");
    for (ret = 0; ret < len; ret++) {
        sprintf(s, "%.2X ", rx[ret]);
        strcat(s1, s);
    }
    strcat(s1, "\n");
    fprintf(stderr, s1);



    return ret;

}

#endif

