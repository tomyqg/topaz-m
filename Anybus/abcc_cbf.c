#include "abcc.h"
#include "abcc_td.h"
#include "abp.h"
#include "abcc_ad_if.h"


// Delivery of the received message.
void ABCC_CbfReceiveMsg( ABP_MsgType* psReceivedMsg)
{

}

// Request of the ADI mapping information.

UINT16 ABCC_CbfAdiMappingReq( const AD_AdiEntryType** const ppsAdiEntry,
                                      const AD_DefaultMapType**  const ppsDefaultMap )
{

}

//              - User specific setup made by the application.
void ABCC_CbfUserInitReq( void )
{

}

//    - Request of the latest write process data.
BOOL ABCC_CbfUpdateWriteProcessData( void* pxWritePd )
{

}

// - Delivery of the latest read process data.
void ABCC_CbfNewReadPd( void* pxReadPd )
{

}

//- Communication lost.
void ABCC_CbfWdTimeout( void )
{

}

//- Communication restored.
void ABCC_CbfWdTimeoutRecovered( void )
{

}

//- Events received. Called from ISR.
void ABCC_CbfEvent( UINT16 iEvents)
{

}

//    - Acknowledge of remap has been sent.
void ABCC_CbfRemapDone( void )
{

}

//         - The anybus state has changed.
void ABCC_CbfAnbStateChanged( ABP_AnbStateType bNewAnbState )
{

}

//- Callback for sync event.
void ABCC_CbfSyncIsr( void )
{

}

//
void ABCC_CbfDriverError(  ABCC_SeverityType eSeverity,
                                   ABCC_ErrorCodeType  iErrorCode,
                                   UINT32  lAddInfo )
{

}
