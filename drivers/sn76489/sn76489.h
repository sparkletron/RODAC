/*******************************************************************************
 * @file    sn76489.h
 * @brief   Library for sn76489 sound chip.
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2022.01.23
 * @details OH SO MANY
 * @version 0.0.1
 * 
 * @TODO
 *  - Add in a better way to enable/disable delay setting
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

#ifndef __LIB_SN76489
#define __LIB_SN76489

#include <stdint.h>

/***************************************************************************//**
 * @brief   Initialize sn76489 to mute all channels
 ******************************************************************************/
void initSN76489();

/***************************************************************************//**
 * @brief   Calculate frequency from hertz to binary value.
 * 
 * @param   refClk is the reference clock in hertz for the sound chip.
 * @param   voiceFreq is the target frequency in hertz.
 * 
 * @return  A unsigned 16 bit number that will result in the freqency wanted.
 ******************************************************************************/
uint16_t getSN76489_FreqDiv(uint32_t refClk, uint32_t voiceFreq);

/***************************************************************************//**
 * @brief   Set sn76489 voice frequency
 * 
 * @param   voice the voice (1,2, or 3) to set the frequency to.
 * @param   freqDiv is binary number to set the frequency (f = refClk/(32*freqDiv))
 ******************************************************************************/
void setSN76489voice_freq(uint8_t voice, uint16_t freqDiv);

/***************************************************************************//**
 * @brief   Set sn76489 voice attenuation
 * 
 * @param   voice the voice (1,2, or 3) to set the attenuation to.
 * @param   attenuate : 8 = 16db, 4 = 8db, 2 = 4db, 1 = 2 db, 0 = 0 db?, 15 = Mute 
 ******************************************************************************/
void setSN76489voice_attn(uint8_t voice, uint8_t attenuate);

/***************************************************************************//**
 * @brief   Set sn76489 noise attenuation
 * 
 * @param   attenuate : 8 = 16db, 4 = 8db, 2 = 4db, 1 = 2 db, 15 = Mute 
 ******************************************************************************/
void setSN76489noise_attn(uint8_t attenuate);

/***************************************************************************//**
 * @brief   Set sn76489 noise type
 * 
 * @param   type : 0 = periodic, 1 = white.
 * @param   rate : 0 = N/512, 1 = N/2048, 2 = N/1024, 3 = voice 3 
 ******************************************************************************/
void setSN76489noiseCtrl(uint8_t type, uint8_t rate);

#endif
