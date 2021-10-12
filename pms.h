/*-----------------------------------------------------------------------------*/
/*
 * usart.h
 *
 *  Created on: Apr 9, 2011
 *      Author: Martin Polasek
 */
/*-----------------------------------------------------------------------------*/
#ifndef PMS_H_INCLUDED
#define PMS_H_INCLUDED

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>

#include "pms_hw.h"
/* Exported functions ------------------------------------------------------- */

/* Defines -------------------------------------------------------------------*/
#define PMS_PM1_0_SP_UGM3     0
#define PMS_PM2_5_SP_UGM3     1
#define PMS_PM10_SP_UGM3      2

#define PMS_PM1_0_AE_UGM3     3
#define PMS_PM2_5_AE_UGM3     4
#define PMS_PM10_AE_UGM3      5     //Opravdu? v dokumentaci chybi koncentrace ceho to je

#define PMS_0_3UM_100ML       6
#define PMS_0_5UM_100ML       7
#define PMS_1UM_100ML         8
#define PMS_2_5UM_100ML       9
#define PMS_5UM_100ML         10
#define PMS_10UM_100ML        11
#define PMS_RESERVED          12

//------------------------------------------------------------------------------

// Functions -------------------------------------------------------------------
void pms_init();

//void pms_wakeup();
//void pms_sleep();
//void pms_reset();

void pms_wakeup_sw();
void pms_sleep_sw();

void pms_rx_data(uint8_t *data, uint16_t len);   //received data processing
void pms_rx_b(uint8_t b);                        //received data processing
//------------------------------------------------------------------------------
#endif /* PMS_H_INCLUDED */
