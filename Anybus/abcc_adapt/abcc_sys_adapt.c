/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.04.01 (2018-01-23)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.55.01 (2017-12-20)                                     **
**    ABCC Driver    5.04.01 (2017-12-18)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2014 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** file_description
********************************************************************************
********************************************************************************
*/

#include "abcc_drv_cfg.h"
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_spi.h"
#include "abcc.h"
#include "Drivers/driverspi.h"
#include "stdio.h"
#include "fcntl.h"
#include "pin.h"
#include "pthread.h"
#include "unistd.h"

#define PIN_RESET   96
#define PIN_IRQ     29

int fdPinIRQ;           //дескриптор прерывания на пине gpio
pthread_t thread;     //указатель на поток
bool fInterruptActive;   //признак активации прерывания


/*******************************************************************************
** Private Services
********************************************************************************
*/


void threadRun(void * arg)
{
    while(1)
    {
        int fd = * (int *) arg;
        char c = getPoll(fd);
        if(c == -1) fprintf(stderr, "error getPoll");
//        if(c == "0") fprintf(stderr, "getPoll = %c\n", c);
        fInterruptActive = true;
        fprintf(stderr, "threadRun()\n");
        usleep(10000);
    }
}

/*******************************************************************************
** Public Services
********************************************************************************
*/

//функция установки RESET для AnybusCC
void ABCC_SYS_HWReset( void )
{
    setPin(PIN_RESET, 0);
}

//Функция сброса RESET - включение anybus
void ABCC_SYS_HWReleaseReset( void )
{
    setPin(PIN_RESET, 1);
}

BOOL ABCC_SYS_HwInit( void )
{
    //init gpio RESET
    setPin(PIN_RESET, 0);

    //init gpio IRQ to interrupt
    fdPinIRQ = setPoll(PIN_IRQ, "falling"/*"rising"*//*"both"*/);
    fprintf(stderr, "fdPinIRQ=%d\n", fdPinIRQ);
    fInterruptActive = false;

    //выделяем отдельный поток для слежения за прерываниями IRQ
    int result = pthread_create(&thread, NULL, threadRun, &fdPinIRQ);
    if(result != 0) fprintf(stderr, "thread_create error = %d\n", result);

    //init of spi interface
    fprintf(stderr, "spi_init\n");
    spi_init(SPI_CS0, SPI_MODE_3, 0, 10000000); //100МГц
    ABCC_SYS_HWReset();
    return TRUE;
}

BOOL ABCC_SYS_Init( void )
{
    return TRUE;
}

void ABCC_SYS_Close( void )
{
    printf("ABCC_SYS_Close");
}

//- Check if interrupt is active.
BOOL ABCC_SYS_IsAbccInterruptActive( void )
{
    return fInterruptActive;
}

//- Start transaction
void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
    // apply SPI driver to transfer data
    spi_trans(pxSendDataBuffer, pxReceiveDataBuffer, iLength);
    pndr();
}

//- MISO received
void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived )
{
    pndr = pnDataReceived;
    pndr();
}
