/*
 * usart.c
 *
 *  Created on: Apr 9, 2011
 *      Author: Martin
 */
//------------------------------------------------------------------------------
#include "pms_hw.h"
#include "pms.h"

//--------------------------------------------
static uint32_t pms1_tx_bytes;
//--------------------------------------------
static uint32_t pms1_rx_bytes;
static uint32_t pms1_rx_overrun;
//--------------------------------------------
static uint32_t pms1_err_fe;
static uint32_t pms1_err_ore;
static uint32_t pms1_err_pe;
static uint32_t pms1_err_ne;
//------------------------------------------------------------------------------
void pms1_init_hw()
{
  pms1_err_fe  = 0;
  pms1_err_ore = 0;
  pms1_err_pe  = 0;
  pms1_err_ne  = 0;

  pms1_tx_bytes   = 0;
  pms1_rx_bytes   = 0;
  pms1_rx_overrun = 0;

  LL_USART_EnableIT_RXNE(USART6);
}
//------------------------------------------------------------------------------
void pms1_prw_on_hw()
{

}
//------------------------------------------------------------------------------
void pms1_prw_off_hw()
{

}
//------------------------------------------------------------------------------
void pms1_reset()
{
  LL_GPIO_ResetOutputPin(GPIOD, GPIO_PIN_15);  //RST  -> L
  HAL_Delay(100);
  LL_GPIO_SetOutputPin(GPIOD, GPIO_PIN_15);    //RST  -> H
}
//------------------------------------------------------------------------------
void pms1_it_proc()
{
  uint8_t uart_data;

  //-------------------------------------------------------------------
  if(LL_USART_IsActiveFlag_PE(USART6))
  {
    LL_USART_ClearFlag_PE(USART6);
    LL_USART_ReceiveData8(USART6);

    pms1_err_pe++;
  }

  if(LL_USART_IsActiveFlag_ORE(USART6))
  {
    LL_USART_ClearFlag_ORE(USART6);
    LL_USART_ReceiveData8(USART6);

    pms1_err_ore++;
  }

  //if(LL_USART_IsEnabledIT_ERROR(USART6) && LL_USART_IsActiveFlag_FE(USART6))
  if(LL_USART_IsActiveFlag_FE(USART6))
  {
    LL_USART_ClearFlag_FE(USART6);
    LL_USART_ReceiveData8(USART6);

    pms1_err_fe++;
  }

  if(LL_USART_IsActiveFlag_NE(USART6))
  {
    LL_USART_ClearFlag_NE(USART6);
    LL_USART_ReceiveData8(USART6);

    pms1_err_ne++;
  }
  //-------------------------------------------------------------------
  if(LL_USART_IsActiveFlag_RXNE(USART6) && LL_USART_IsEnabledIT_RXNE(USART6))
  {
    uart_data = LL_USART_ReceiveData8(USART6);
    pms1_rx_bytes++;

    //opravdu? nepocitat je jen do rx_overrrun?
    if(ring_buff_put(&pms1_rx_ring_buff, &uart_data, 1) == 0) pms1_rx_overrun ++;
  }
  //-------------------------------------------------------------------
  if(LL_USART_IsEnabledIT_TC(USART6) && LL_USART_IsActiveFlag_TC(USART6))
  {
    LL_USART_ClearFlag_TC(USART6);
    LL_USART_DisableIT_TC(USART6);
  }
  //----------------------------------------------------------------
  if(LL_USART_IsEnabledIT_TXE(USART6) && LL_USART_IsActiveFlag_TXE(USART6))
  {
    if(ring_buff_get(&pms1_tx_ring_buff, &uart_data, 1) == 1)
    {
      LL_USART_TransmitData8(USART6, uart_data);
      pms1_tx_bytes++;
    }
    else
    {
      LL_USART_DisableIT_TXE(USART6);
    }
  }
  //----------------------------------------------------------------
  /*if(LL_USART_IsActiveFlag_IDLE(USART6))
  {
    LL_USART_ClearFlag_IDLE(USART6);
  }*/

  /*if(LL_USART_IsActiveFlag_LBD(USART6))
  {
    LL_USART_ClearFlag_LBD(USART6);
  }*/

  /*if(LL_USART_IsActiveFlag_nCTS(USART6))
  {
    LL_USART_ClearFlag_nCTS(USART6);
  }*/
}
//------------------------------------------------------------------------------
void pms1_rx_proc()
{
  uint8_t buff[70];
  uint32_t n, i;

  n = ring_buff_get(&pms1_rx_ring_buff, buff, 70);
  if(n != 0)
  {
    for(i = 0; i < n; i++)
    {
      pms1_rx_data(buff[i]);
    }

    //CDC_Transmit_FS((uint8_t*)buff, n);
  }
}
//------------------------------------------------------------------------------
void pms1_tx_data(uint8_t *data, uint16_t len)
{
  log_tx_msg(data, len);

  if(ring_buff_put(&pms1_tx_ring_buff, data, len) == 0) pms1_tx_overrun ++;
  LL_USART_EnableIT_TXE(USART6);
}
//------------------------------------------------------------------------------
