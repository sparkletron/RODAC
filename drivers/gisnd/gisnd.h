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

#include <stdint.h>

/***************************************************************************//**
 * @brief   Initialize gisnd and mute
 ******************************************************************************/
void initGISND();

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
 * @param   channel Select channel A, B, or C (character input, upper case).
 * @param   freqDiv is binary number to set the frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setGISNDchannel_freq(char channel, uint16_t freqDiv);

/***************************************************************************//**
 * @brief   Set gisnd channel attenuation
 * 
 * @param   channel Select channel A, B, or C (character input, upper case).
 * @param   attenuate A 4 bit value (0 to 15)
 * @param   select When select is 1, volume control is set by envelope generator, 0 by attenuate.
 ******************************************************************************/
void setGISNDchannel_attn(char channel, uint8_t attenuate, uint8_t select);

/***************************************************************************//**
 * @brief   Set gisnd mixer setting
 *
 * @param   noise 0 is enable, 1 is off. bit order C = 2, B = 1, A = 0.
 * @param   tone  0 is enable, 1 is off. bit order C = 2, B = 1, A = 0.
 ******************************************************************************/
void setGISNDmixer(uint8_t noise, uint8_t tone);

/***************************************************************************//**
 * @brief   Set gisnd noise frequency
 * 
 * @param   freqDiv is binary number to set the frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setGISNDnoise_freq(uint8_t freqDiv);

/***************************************************************************//**
 * @brief   Set gisnd envelope frequency
 * 
 * @param   freqDiv is binary number to set the frequency (f = refClk/(512*freqDiv))
 ******************************************************************************/
void setGISNDenv_freq(uint16_t freqDiv);

/***************************************************************************//**
 * @brief   Set gisnd envelope shape
 *
 * @param   shape A 4 bit value that can change the envelope shape, see datasheet.
 ******************************************************************************/
void setGISNDenv_shape(uint8_t shape);

#endif
