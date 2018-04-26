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
** This code is the property of HMS Industrial Networks AB.                   **
** The source code may not be reproduced, distributed, or used without        **
** permission. When used together with a product from HMS, permission is      **
** granted to modify, reproduce and distribute the code in binary form        **
** without any restrictions.                                                  **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
** Platform specific product configuration file.
**
** A symbol defined in this file will override the default product
** configurations in abcc_identification.h and abcc_obj_cfg.h.
** It should be used whenever the target platform needs to override an
** default configuration to realize its implementation.
********************************************************************************
********************************************************************************
** Services:
********************************************************************************
********************************************************************************
*/
#ifndef ABCC_PLATFORM_CFG_H_
#define ABCC_PLATFORM_CFG_H_

#include "abcc_sys_adapt_spi.h"


/*******************************************************************************
** Constants
********************************************************************************
*/

//#define ETN_OBJ_ENABLE                          FALSE//TRUE
//#define ETN_IA_IP_CONFIGURATION_ENABLE          FALSE//TRUE
//#define ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK   FALSE//TRUE

//#define APP_IA_SER_NUM_ENABLE                   TRUE
//#define APP_IA_SER_NUM_VALUE                    0x11223344

//#define SAFE_OBJ_ENABLE                         TRUE
//#define SAFE_IA_SAFETY_ENABLED_ENABLE           TRUE
//#define SAFE_IA_SAFETY_ENABLED_VALUE            FALSE
//#define SAFE_IA_BAUD_RATE_ENABLE                TRUE
//#define SAFE_IA_BAUD_RATE_VALUE                 1020000L

//#define SAFE_IA_CYCLE_TIME_ENABLE               TRUE
//#define SAFE_IA_CYCLE_TIME_VALUE                2

#define DPV1_OBJ_ENABLE                         TRUE
#define MOD_OBJ_ENABLE                          TRUE
#define ABCC_CFG_POLL_ABCC_IRQ_PIN              FALSE

/* Anybus CompactCom Setup */
#define ABCC_CFG_MAX_NUM_APPL_CMDS                  ( 2 )
#define ABCC_CFG_MAX_NUM_ABCC_CMDS                  ( 2 )
#define ABCC_CFG_MAX_MSG_SIZE                       ( 1524 )
#define ABCC_CFG_MAX_PROCESS_DATA_SIZE              ( 512 )
#define ABCC_CFG_CMD_SEQ_MAX_NUM_RETRIES            ( 1 )


//cDriverSPI spi;
ABCC_SYS_SpiDataReceivedCbfType pndr;


#endif /* #ifndef ABCC_PLATFORM_CFG_H_ */
