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
** COPYRIGHT NOTIFICATION (c) 2015 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** Platform dependent macros and functions required by the ABCC driver and
** Anybus objects implementation to be platform independent.
********************************************************************************
********************************************************************************
** Services:
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_SW_PORT_H_
#define ABCC_SW_PORT_H_

#include "string.h"
#include "stdio.h"
#include "abcc_td.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

#define ABCC_PORT_DebugPrint( args ) printf args//debug_printf args //
//EXTFUNC void debug_printf( __const char *__restrict __format, ... );

#define ABCC_PORT_UseCritical() ABCC_PORT_UseCriticalImpl()
EXTFUNC void ABCC_PORT_UseCriticalImpl( void );

#define ABCC_PORT_EnterCritical() ABCC_PORT_EnterCriticalImpl()
EXTFUNC void ABCC_PORT_EnterCriticalImpl( void );

#define ABCC_PORT_ExitCritical() ABCC_PORT_ExitCriticalImpl()
EXTFUNC void ABCC_PORT_ExitCriticalImpl( void );

#define ABCC_PORT_MemCpy( pbDest, pbSource, iNbrOfOctets )                     \
        memcpy( pbDest, pbSource, iNbrOfOctets );

#define ABCC_PORT_CopyOctets( pxDest, iDestOctetOffset, pxSrc, iSrcOctetOffset,\
                              iNumOctets )                                     \
        ABCC_PORT_MemCpy( (void*)&( (UINT8*)(pxDest) )[ iDestOctetOffset ],    \
                          (void*)&( (UINT8*)(pxSrc) )[ iSrcOctetOffset ],      \
                          iNumOctets )

#define ABCC_PORT_StrCpyToNative( pxDest, pxSrc, iSrcOctetOffset, iNbrOfChars )\
        ABCC_PORT_MemCpy( (void*)(pxDest),                                     \
                          (void*)&( (UINT8*)(pxSrc) )[ iSrcOctetOffset ],      \
                          iNbrOfChars )

#define ABCC_PORT_StrCpyToPacked( pxDest, iDestOctetOffset, pxSrc,             \
                                  iNbrOfChars )                                \
        ABCC_PORT_MemCpy( (void*)&( (UINT8*)(pxDest) )[ iDestOctetOffset ],    \
                           pxSrc, iNbrOfChars )

#define ABCC_PORT_Copy8( pxDest, iDestOctetOffset, pxSrc, iSrcOctetOffset )    \
        ( (UINT8*)(pxDest) )[ iDestOctetOffset ] =                             \
        ( (UINT8*)(pxSrc) )[ iSrcOctetOffset ]

#define ABCC_PORT_Copy16( pxDest, iDestOctetOffset, pxSrc, iSrcOctetOffset )   \
        ABCC_PORT_CopyOctets( pxDest, iDestOctetOffset, pxSrc, iSrcOctetOffset, 2 )

#define ABCC_PORT_Copy32( pxDest, iDestOctetOffset, pxSrc, iSrcOctetOffset )   \
        ABCC_PORT_CopyOctets( pxDest, iDestOctetOffset, pxSrc, iSrcOctetOffset, 4 )

#if( ABCC_CFG_64BIT_ADI_SUPPORT )
#define ABCC_PORT_Copy64( pxDest, iDestOctetOffset, pxSrc, iSrcOctetOffset )   \
        ABCC_PORT_CopyOctets( pxDest, iDestOctetOffset, pxSrc, iSrcOctetOffset, 8 )
#endif

#endif  /* inclusion lock */
