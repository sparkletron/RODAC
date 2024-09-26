/*******************************************************************************
 * @file    sn76489.c
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2022.01.23
 * @brief   Library for SN76489 sound chip.
 * 
 * @license mit
 * 
 * Copyright 2022 Johnathan Convertino
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

#include <sn76489.h>

/** DEFINES **/
#define VOICE1_FREQ 0
#define VOICE2_FREQ 2
#define VOICE3_FREQ 4
#define VOICE1_ATTN 1
#define VOICE2_ATTN 3
#define VOICE3_ATTN 5
#define NOISE_CTRL  6
#define NOISE_ATTN  7
#define FIRST_BYTE  0x80
#define SECOND_BYTE 0x00
#define REG_SHIFT   4

/** SEE MY PRIVATES **/
/*** send data to port and control chip ***/
inline void sendData(uint8_t data);

/*** Initialize sn76489 ***/
void initSN76489()
{
  /**** mute all ****/
  
  setSN76489voice_attn(1, 15);
  
  setSN76489voice_attn(2, 15);
  
  setSN76489voice_attn(3, 15);
  
  setSN76489noise_attn(15);
}

/*** calculate freqDiv ***/
uint16_t getSN76489_FreqDiv(uint32_t refClk, uint32_t voiceFreq)
{
  return (refClk/(voiceFreq << 5)) & 0x03FF;
}

/** SET YOUR DATA **/

/*** set sn76489 voice frequency ***/
void setSN76489voice_freq(uint8_t voice, uint16_t freqDiv)
{
  uint8_t regVoice = 0;
  
  /**** Select the correct register for the selected voice ****/
  switch(voice)
  {
    case 1:
      regVoice = VOICE1_FREQ;
      break;
    case 2:
      regVoice = VOICE2_FREQ;
      break;
    case 3:
      regVoice = VOICE3_FREQ;
      break;
    default:
      regVoice = VOICE1_FREQ;
      break;
  }
  
  sendData((((unsigned)regVoice << REG_SHIFT) | (freqDiv & 0x000F) | FIRST_BYTE));
  sendData((((freqDiv & 0x03F0) >> 4) | SECOND_BYTE));
}

/*** set sn76489 voice attenuation ***/
void setSN76489voice_attn(uint8_t voice, uint8_t attenuate)
{
  uint8_t regVoice = 0;
  
  /**** Select the correct register for the selected voice ****/
  switch(voice)
  {
    case 1:
      regVoice = VOICE1_ATTN;
      break;
    case 2:
      regVoice = VOICE2_ATTN;
      break;
    case 3:
      regVoice = VOICE3_ATTN;
      break;
    default:
      regVoice = VOICE1_ATTN;
      break;
  }
  
  sendData(((unsigned)regVoice << REG_SHIFT) | attenuate | FIRST_BYTE);
}

/*** set sn76489 noise attenuation ***/
void setSN76489noise_attn(uint8_t attenuate)
{
  sendData(((unsigned)NOISE_ATTN << REG_SHIFT) | attenuate | FIRST_BYTE);
}

/*** set sn76489 noise controls***/
void setSN76489noiseCtrl(uint8_t type, uint8_t rate)
{
  sendData(((unsigned)NOISE_CTRL << REG_SHIFT) | (unsigned)rate | ((unsigned)type << 2) | FIRST_BYTE);
}

/*** send data to chip ***/
inline void sendData(uint8_t data)
{
  di();
  
  SN_SND_PORT = data;
  
  ei();
}
