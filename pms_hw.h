/*-----------------------------------------------------------------------------*/
/*
 * usart.h
 *
 *  Created on: Apr 9, 2011
 *      Author: Martin Polasek
 */
/*-----------------------------------------------------------------------------*/
#ifndef PMS1003_HW_H_INCLUDED
#define PMS1003_HW_H_INCLUDED

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/*#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_usart.h"

#include "ring_buffer.h"
#include "log.h"*/
/* Exported functions ------------------------------------------------------- */

/* Defines -------------------------------------------------------------------*/
#define PMS1_RB_RX_BUFF_LEN 500
#define PMS1_RB_TX_BUFF_LEN 500
// Functions -------------------------------------------------------------------
void pms1_init_hw();

void pms1_sleep(uint8_t state);
void pms1_reset();

void pms1_it_proc();
void pms1_rx_proc();
void pms1_tx_data(uint8_t *data, uint16_t len);
//------------------------------------------------------------------------------
#endif /* PMS1003_HW_H_INCLUDED */
