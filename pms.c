/*
 * usart.c
 *
 *  Created on: Apr 9, 2011
 *      Author: Martin
 */
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "pms.h"

//------------------------------------------------------------------------------
// Globalni promenne modulu
//------------------------------------------------------------------------------
uint8_t pms_buf[32];                   //buffer na prijimana data
uint8_t pms_buf_pos;                   //pozice v bufferu na prijimana data
uint8_t pms_frame_len;                 //delka prijimaneho ramce
uint16_t pms_frame_cc;                 //kontrolni soucet dat ramce

uint16_t pms_data[13];                 //pole nactenych hodnot
 uint8_t pms_data_valid;               //platnost nactenych dat
 
uint32_t pms_msg_cnt;                  //pocet platnych prijatych zprav
uint32_t pms_msg_fr_err;               //pocet zprav s vadnym ramcem
uint32_t pms_msg_cc_err;               //pocet zprav s vadnym kontrolnim souctem
 
//------------------------------------------------------------------------------
void pms_init()
{
  uint8_t i;

  pms_buf_pos = 0;
  pms_frame_len = 0;
  pms_frame_cc = 0;
  
  pms_data_valid = 0;
  
  pms_msg_cnt = 0;
  pms_msg_fr_err = 0;
  pms_msg_cc_err = 0;

  for(i = 0; i < 32; i++) pms_data[i] = 0;

  pms_init_hw();
}
//------------------------------------------------------------------------------
/*void pms_wakeup()
{
    
}
//------------------------------------------------------------------------------
void pms_sleep()
{
    
}
//------------------------------------------------------------------------------
void pms_reset()
{
    
}*/
//------------------------------------------------------------------------------
void pms_wakeup_sw()
{
    
}
//------------------------------------------------------------------------------
void pms_sleep_sw()
{
    
}
//------------------------------------------------------------------------------
void pms_rx_data(uint8_t *data, uint16_t len)
{
  uint16_t i;
  for(i = 0; i < len; i++) pms_rx_b(data[i]);
}
//------------------------------------------------------------------------------
void pms_rx_b(uint8_t b)
{
  uint8_t i;  
  uint16_t cc;  
    
  pms_buf[pms_buf_pos++] = b;
    
  switch(pms_buf_pos)
  {
  case 1:                     //start character 1
    if(b != 0x42)      
    {
      pms_buf_pos = 0;
      pms_msg_fr_err++;
    }

    pms_frame_cc = b;
    return;
    break;
    
  case 2:                     //start character 2
    if(b != 0x4D)
    {
      pms_buf_pos = 0;
      pms_msg_fr_err++;
    }

    pms_frame_cc += b;
    return;
    break;

  case 3:                    //frame length, hight 8 bits, 2x13+2 or 2x9+2 (data+check bytes)
    if(b != 0x00)
    {
      pms_buf_pos = 0;  
      pms_msg_fr_err++;
    }    

    pms_frame_cc += b;
    return;
    break;
    
  case 4:                    //frame length, low 8 bits, 2x13+2 or 2x9+2 (data+check bytes)
    pms_frame_len = b + 4;  
      
    if(pms_frame_len != 32 || pms_frame_len != 24)
    {
      pms_buf_pos = 0;  
      pms_msg_fr_err++;
    }

    pms_frame_cc += b;
    return;
    break;  
    
  default:
    break;
  }
  
  if(pms_buf_pos >= pms_frame_len - 2) pms_frame_cc += b;

  if(pms_buf_pos >= pms_frame_len)    //end of frame
  {
    //cc = pms_buf[0];
    //for(i = 1; i < pms_frame_len - 2; i++) cc += pms_buf[i];
    
    if(pms_frame_cc != ((pms_buf[pms_frame_len - 2] << 8) + pms_buf[pms_frame_len - 1]))  //check sum test
    {
      pms_buf_pos = 0;  
      pms_msg_cc_err++;
      return;
    }
    
    for(i = 0; i < (pms_frame_len - 6) / 2; i++) pms_data[i] = (pms_buf[i*2 + 4] << 8) + pms_buf[i*2 + 4 + 1];
    
    pms_buf_pos = 0;  
    pms_msg_cnt++;
    pms_data_valid = 1;
  }
}
//------------------------------------------------------------------------------
