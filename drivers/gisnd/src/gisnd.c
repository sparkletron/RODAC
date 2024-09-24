/*******************************************************************************
 * @file    gisnd.c
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2024.05.27
 * @brief   Library for GISND sound chip.
 * 
 * @license mit
 * 
 * Copyright 2024 Johnathan Convertino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>

#include <gisnd.h>

/** DEFINES **/
#define CHAN_A_FREQ_L   0
#define CHAN_A_FREQ_H   1
#define CHAN_B_FREQ_L   2
#define CHAN_B_FREQ_H   3
#define CHAN_C_FREQ_L   4
#define CHAN_C_FREQ_H   5
#define NOISE_FREQ      6
#define MIXER_SETTING   7
#define CHAN_A_LEVEL    8
#define CHAN_B_LEVEL    9
#define CHAN_C_LEVEL    10
#define ENVELOPE_FREQ_L 11
#define ENVELOPE_FREQ_H 12
#define ENVELOPE_SHAPE  13
#define NON_VALID_ADDR  14
#define CONTROL_POWER   15

/** SEE MY PRIVATES **/
/*** send address to port and control chip ***/
void sendAddr(struct s_gisnd *p_gisnd, uint8_t addr);
/*** send data to port and control chip ***/
void sendData(struct s_gisnd *p_gisnd, uint8_t data);

/** INITIALIZE AND FREE MY STRUCTS **/

/*** Initialize gisnd tris port direction and struct pins ***/
void initGISNDport(struct s_gisnd *p_gisnd, volatile unsigned char *p_dataTRIS, volatile unsigned char *p_ctrlTRIS, uint8_t nCSWR, uint8_t A0)
{
  if(!p_dataTRIS) return;
  
  if(!p_ctrlTRIS) return;
  
  if(!p_gisnd) return;
  
  /**** All outputs for data port ****/
  *p_dataTRIS = 0;
  
  /**** nCSWR, and A0 are output ****/
  *p_ctrlTRIS &= ~(((unsigned)1 << nCSWR) | ((unsigned)1 << A0));
  
  /**** setup struct with pin locations ****/
  p_gisnd->nCSWR = nCSWR;
  
  p_gisnd->A0 = A0;
}

/*** Initialize gisnd struct ports ***/
void initGISND(struct s_gisnd *p_gisnd, volatile unsigned char *p_dataPortW, volatile unsigned char *p_ctrlPortW)
{
  if(!p_dataPortW) return;
  
  if(!p_ctrlPortW) return;
  
  if(!p_gisnd)  return;
  
  /**** setup ports for library usage ****/
  p_gisnd->p_dataPortW = p_dataPortW;
  
  p_gisnd->p_ctrlPortW = p_ctrlPortW;
  
  *(p_gisnd->p_ctrlPortW) = ~0;
  
  *(p_gisnd->p_dataPortW) = 0;
  
  /**** mute all ****/
  
  setGISNDchannel_attn(p_gisnd, 'A', 0, 0);
  
  setGISNDchannel_attn(p_gisnd, 'B', 0, 0);
  
  setGISNDchannel_attn(p_gisnd, 'C', 0, 0);
}

/*** calculate freqDiv ***/
uint16_t getGISND_FreqDiv(uint32_t refClk, uint32_t channelFreq)
{
  return (refClk/(channelFreq << 5)) & 0x0FFF;
}

/*** calculate envelope freqDiv ***/
uint16_t getGISND_EnvFreqDiv(uint32_t refClk, uint32_t channelFreq)
{
  return (refClk/(channelFreq << 9));
}

/** SET YOUR DATA **/

/*** set gisnd channel frequency ***/
void setGISNDchannel_freq(struct s_gisnd *p_gisnd, char channel, uint16_t freqDiv)
{
  uint8_t addr_l;
  uint8_t addr_h;
  
  /**** NULL check ****/
  if(!p_gisnd) return;
  
  /**** Select the correct register for the selected channel ****/
  switch(channel)
  {
    case 'A':
      addr_l = CHAN_A_FREQ_L;
      addr_h = CHAN_A_FREQ_H;
      break;
    case 'B':
      addr_l = CHAN_B_FREQ_L;
      addr_h = CHAN_B_FREQ_H;
      break;
    case 'C':
      addr_l = CHAN_C_FREQ_L;
      addr_h = CHAN_C_FREQ_H;
      break;
    default:
      return;
  }
  
  sendAddr(p_gisnd, addr_l);
  sendData(p_gisnd, freqDiv & 0x00FF);

  sendAddr(p_gisnd, addr_h);
  sendData(p_gisnd, (freqDiv >> 8) & 0x000F);
}

/*** set gisnd channel attenuation ***/
void setGISNDchannel_attn(struct s_gisnd *p_gisnd, char channel, uint8_t attenuate, uint8_t select)
{
  uint8_t addr;


  /**** NULL check ****/
  if(!p_gisnd) return;

  /**** Select the correct register for the selected channel ****/
  switch(channel)
  {
    case 'A':
      addr = CHAN_A_LEVEL;
      break;
    case 'B':
      addr = CHAN_B_LEVEL;
      break;
    case 'C':
      addr = CHAN_C_LEVEL;
      break;
    default:
      return;
  }
  
  sendAddr(p_gisnd, addr);
  sendData(p_gisnd, (attenuate & (unsigned)0x0F) | (unsigned)(select << 4));
}

void setGISNDmixer(struct s_gisnd *p_gisnd, uint8_t noise, uint8_t tone)
{
  /**** NULL check ****/
  if(!p_gisnd) return;

  sendAddr(p_gisnd, MIXER_SETTING);
  sendData(p_gisnd, (unsigned)(noise << 3) | tone);
}

/*** set gisnd noise frequency ***/
void setGISNDnoise_freq(struct s_gisnd *p_gisnd, uint8_t freqDiv)
{
  /**** NULL check ****/
  if(!p_gisnd) return;
  
  sendAddr(p_gisnd, NOISE_FREQ);
  sendData(p_gisnd, freqDiv & (unsigned)0x1F);
}

/*** set gisnd envelope frequency***/
void setGISNDenv_freq(struct s_gisnd *p_gisnd, uint16_t freqDiv)
{
  /**** NULL check ****/
  if(!p_gisnd) return;
  
  sendAddr(p_gisnd, ENVELOPE_FREQ_L);
  sendData(p_gisnd, freqDiv & 0x00FF);

  sendAddr(p_gisnd, ENVELOPE_FREQ_H);
  sendData(p_gisnd, (freqDiv >> 8) & 0x00FF);
}

void setGISNDenv_shape(struct s_gisnd *p_gisnd, uint8_t shape)
{
  /**** NULL check ****/
  if(!p_gisnd) return;

  sendAddr(p_gisnd, ENVELOPE_SHAPE);
  sendData(p_gisnd, shape & (unsigned)0x0F);
}

/*** send address to chip ***/
void sendAddr(struct s_gisnd *p_gisnd, uint8_t addr)
{
  /**** NULL check ****/
  if(!p_gisnd) return;

  *(p_gisnd->p_ctrlPortW) &= ~((unsigned)1 << p_gisnd->A0);

  sendData(p_gisnd, addr);

  *(p_gisnd->p_ctrlPortW) |= ((unsigned)1 << p_gisnd->A0);

}

/*** send data to chip ***/
void sendData(struct s_gisnd *p_gisnd, uint8_t data)
{
  /**** NULL check ****/
  if(!p_gisnd) return;
  
  di();
  
  *(p_gisnd->p_dataPortW) = data;

  *(p_gisnd->p_ctrlPortW) &= ~((unsigned)1 << p_gisnd->nCSWR);

  *(p_gisnd->p_ctrlPortW) |= ((unsigned)1 << p_gisnd->nCSWR);
  
  ei();
}
