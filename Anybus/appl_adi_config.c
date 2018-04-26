#include "appl_adi_config.h"

UINT32   appl_lUint32;
INT32    appl_lInt32;

#define ADI_DESCR_ALL_ACCESS  ( ABP_APPD_DESCR_GET_ACCESS | ABP_APPD_DESCR_SET_ACCESS | ABP_APPD_DESCR_MAPPABLE_WRITE_PD | ABP_APPD_DESCR_MAPPABLE_READ_PD )

/*------------------------------------------------------------------------------
** Access descriptor for the ADIs
**------------------------------------------------------------------------------
*/
#define APPL_READ_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_READ_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_SET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_WRITE_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_WRITE_PD )

#define APPL_NOT_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS )

#define APPL_NOT_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_SET_ACCESS )

/*------------------------------------------------------------------------------
** ADI instance allocation without 'struct' or 'callback'.
**------------------------------------------------------------------------------
*/
const AD_AdiEntryType APPL_asAdiEntryList[] =
{
   /*
   ** --------------------------------------------------------------------------------------------------------------
   ** | 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 6.pxValuePtr | 7. pxValuePropPtr |
   ** --------------------------------------------------------------------------------------------------------------
   */
    { 20, "ABP_UINT32_READ",  ABP_UINT32, 1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_lUint32, NULL } }, NULL },
    { 21, "ABP_UINT32_WRITE", ABP_UINT32, 1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_lUint32, NULL } }, NULL },
    { 22, "ABP_SINT32_READ",  ABP_SINT32, 1,  APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_lInt32,  NULL } }, NULL },
    { 23, "ABP_SINT32_WRITE", ABP_SINT32, 1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_lInt32,  NULL } }, NULL },
};

void APPL_CyclicalProcessing( void )
{

}

unsigned short APPL_GetNumAdi( void )
{
    return( sizeof( APPL_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}

