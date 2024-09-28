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

#include <base.h>
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
inline void sendAddr(uint8_t addr);
/*** send data to port and control chip ***/
inline void sendData(uint8_t data);

/** INITIALIZE AND FREE MY STRUCTS **/

/*** Initialize gisnd struct ports ***/
void initGISND()
{
  /**** mute all ****/
  
  setGISNDchannel_attn('A', 0, 0);
  
  setGISNDchannel_attn('B', 0, 0);
  
  setGISNDchannel_attn('C', 0, 0);
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
void setGISNDchannel_freq(char channel, uint16_t freqDiv)
{
  uint8_t addr_l;
  uint8_t addr_h;
  
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
  
  sendAddr(addr_l);
  sendData(freqDiv & 0x00FF);

  sendAddr(addr_h);
  sendData((freqDiv >> 8) & 0x000F);
}

/*** set gisnd channel attenuation ***/
void setGISNDchannel_attn(char channel, uint8_t attenuate, uint8_t select)
{
  uint8_t addr;

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
  
  sendAddr(addr);
  sendData((attenuate & (unsigned)0x0F) | (unsigned)(select << 4));
}

void setGISNDmixer(uint8_t noise, uint8_t tone)
{
  sendAddr(MIXER_SETTING);
  sendData((unsigned)(noise << 3) | tone);
}

/*** set gisnd noise frequency ***/
void setGISNDnoise_freq(uint8_t freqDiv)
{
  sendAddr(NOISE_FREQ);
  sendData(freqDiv & (unsigned)0x1F);
}

/*** set gisnd envelope frequency***/
void setGISNDenv_freq(uint16_t freqDiv)
{
  sendAddr(ENVELOPE_FREQ_L);
  sendData(freqDiv & 0x00FF);

  sendAddr(ENVELOPE_FREQ_H);
  sendData((freqDiv >> 8) & 0x00FF);
}

void setGISNDenv_shape(uint8_t shape)
{
  sendAddr(ENVELOPE_SHAPE);
  sendData(shape & (unsigned)0x0F);
}

/*** send address to chip ***/
inline void sendAddr(uint8_t addr)
{
  di();

  GI_SND_CP_PORT = addr;

  ei();
}

/*** send data to chip ***/
inline void sendData(uint8_t data)
{
  di();
  
  GI_SND_WDATA_PORT = data;
  
  ei();
}
