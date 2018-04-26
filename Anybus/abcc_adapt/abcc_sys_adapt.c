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



/*******************************************************************************
** Public Services
********************************************************************************
*/

//функция установки RESET для AnybusCC
void ABCC_SYS_HWReset( void )
{

    FILE *fgpio = fopen( "/sys/class/gpio/gpio96/value", "w" );
    if(fgpio == NULL)
        fprintf(stderr, "Error file /sys/class/gpio/gpio96/value open\n");
    else
        fprintf(fgpio, "0");

    fclose(fgpio);


}

//Функция сброса RESET - включение anybus
void ABCC_SYS_HWReleaseReset( void )
{
    FILE *fgpio = fopen( "/sys/class/gpio/gpio96/value", "w" );
    if(fgpio == NULL)
        fprintf(stderr, "Error file /sys/class/gpio/gpio96/value open\n");
    else
        fprintf(fgpio, "1");

    fclose(fgpio);
}

BOOL ABCC_SYS_HwInit( void )
{

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

}

//- Start transaction
void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
    // apply SPI driver to transfer data
//    fprintf(stderr, "spi_trans\n");
    spi_trans(pxSendDataBuffer, pxReceiveDataBuffer, iLength);
    pndr();
}

//- MISO received
void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived )
{
    pndr = pnDataReceived;
    pndr();
}


