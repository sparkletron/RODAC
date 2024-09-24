/*******************************************************************************
 * @file    gisnd.h
 * @brief   Library for gisnd sound chip.
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2024.05.27
 * @details OH SO MANY
 * @version 0.0.1
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

#ifndef __LIB_GISND
#define __LIB_GISND

#include <xc.h>
#include <stdint.h>

/**
 * @struct s_gisnd
 * @brief Struct for containing gisnd instances
 */
struct s_gisnd
{
  /**
   * @var s_gisnd::p_dataPortW
   * data port to use for writing output data
   */
  volatile unsigned char *p_dataPortW;
  /**
   * @var s_gisnd::p_ctrlPortW
   * port to use for control line nCS + nWR, and A0. Will not change other port pins.
   */
  volatile unsigned char *p_ctrlPortW;
  /**
   * @var s_gisnd::nCSWR
   * active low write enable pin number
   */
  uint8_t nCSWR;
  /**
   * @var s_gisnd::A0
   * select data (H) or address (L) mode.
   */
  uint8_t A0;
};

/***************************************************************************//**
 * @brief   Initialize gisnd ports for correct I/O and set pin numbers
 * 
 * @param   p_gisnd pointer to struct to contain port data.
 * @param   p_dataTRIS pointer to data direction register.
 * @param   p_ctrlTRIS pointer to control direction register.
 * @param   nCSWR is the pin number(bit) nCS + WR is connected to on control port.
 * @param   A0 Select Address (0) or Data register (1).
 ******************************************************************************/
void initGISNDport(struct s_gisnd *p_gisnd, volatile unsigned char *p_dataTRIS, volatile unsigned char *p_ctrlTRIS, uint8_t nCSWR, uint8_t A0);

/***************************************************************************//**
 * @brief   Initialize gisnd struct with ports to use for input output, must match
 * direction registers above.
 * 
 * @param   p_gisnd pointer to struct to contain port data.
 * @param   p_dataPortW pointer to data output port.
 * @param   p_ctrlPortW pointer to control output port.
 ******************************************************************************/
void initGISND(struct s_gisnd *p_gisnd, volatile unsigned char *p_dataPortW, volatile unsigned char *p_ctrlPortW);

/***************************************************************************//**
 * @brief   Calculate frequency from hertz to binary value.
 *
 * @param   refClk is the reference clock in hertz for the sound chip.
 * @param   channelFreq is the target frequency in hertz.
 *
 * @return  A unsigned 16 bit number that will result in the freqency wanted. (* 32)
 ******************************************************************************/
uint16_t getGISND_FreqDiv(uint32_t refClk, uint32_t channelFreq);

/***************************************************************************//**
 * @brief   Calculate envelope frequency from hertz to binary value.
 *
 * @param   refClk is the reference clock in hertz for the sound chip.
 * @param   channelFreq is the target frequency in hertz.
 *
 * @return  A unsigned 16 bit number that will result in the freqency wanted. (* 512)
 *  ******************************************************************************/
uint16_t getGISND_EnvFreqDiv(uint32_t refClk, uint32_t channelFreq);

/***************************************************************************//**
 * @brief   Set gisnd channel frequency
 * 
 * @param   p_gisnd pointer to a struct that contains chip data.
 * @param   channel Select channel A, B, or C (character input, upper case).
 * @param   freqDiv is binary number to set the frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setGISNDchannel_freq(struct s_gisnd *p_gisnd, char channel, uint16_t freqDiv);

/***************************************************************************//**
 * @brief   Set gisnd channel attenuation
 * 
 * @param   p_gisnd pointer to a struct that contains chip data.
 * @param   channel Select channel A, B, or C (character input, upper case).
 * @param   attenuate A 4 bit value (0 to 15)
 * @param   select When select is 1, volume control is set by envelope generator, 0 by attenuate.
 ******************************************************************************/
void setGISNDchannel_attn(struct s_gisnd *p_gisnd, char channel, uint8_t attenuate, uint8_t select);

/***************************************************************************//**
 * @brief   Set gisnd mixer setting
 *
 * @param   p_gisnd pointer to a struct that contains chip data.
 * @param   noise 0 is enable, 1 is off. bit order C = 2, B = 1, A = 0.
 * @param   tone  0 is enable, 1 is off. bit order C = 2, B = 1, A = 0.
 ******************************************************************************/
void setGISNDmixer(struct s_gisnd *p_gisnd, uint8_t noise, uint8_t tone);

/***************************************************************************//**
 * @brief   Set gisnd noise frequency
 * 
 * @param   p_gisnd pointer to a struct that contains chip data.
 * @param   freqDiv is binary number to set the frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setGISNDnoise_freq(struct s_gisnd *p_gisnd, uint8_t freqDiv);

/***************************************************************************//**
 * @brief   Set gisnd envelope frequency
 * 
 * @param   p_gisnd pointer to a struct that contains chip data.
 * @param   freqDiv is binary number to set the frequency (f = refClk/(512*freqDiv))
 ******************************************************************************/
void setGISNDenv_freq(struct s_gisnd *p_gisnd, uint16_t freqDiv);

/***************************************************************************//**
 * @brief   Set gisnd envelope shape
 *
 * @param   p_gisnd pointer to a struct that contains chip data.
 * @param   shape A 4 bit value that can change the envelope shape, see datasheet.
 ******************************************************************************/
void setGISNDenv_shape(struct s_gisnd *p_gisnd, uint8_t shape);

#endif
