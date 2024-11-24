/**************************************************************************//**
 * @file    base.h
 * @author  Jay Convertino
 ******************************************************************************/

#ifndef __BASE
#define __BASE

#include "defines.h"

#include <stdint.h>

/***************************************************************************//**
 * @brief   delay in microseconds... at least close enough to it.
 *
 * @param   count number of microseconds to delay.
 ******************************************************************************/
void __delay_us(int count);

/***************************************************************************//**
 * @brief  tms99XX VDP interrupt is connected to NMI.
 *
 * @param  callback a function to use to parse vdp.
 ******************************************************************************/
void set_vdp_irq_callback(void (*callback)(void));

/***************************************************************************//**
 * @brief   coleco spinner irq callback
 *
 * @param   callback a function to use to parse spin.
 ******************************************************************************/
void set_spin_irq_callback(void (*callback)(void));

/***************************************************************************//**
 * @brief   read controller one port.
 *
 * @return  A unsigned 16 bit number contains all controller states.
 ******************************************************************************/
uint16_t getControllerOne();

/***************************************************************************//**
 * @brief   read controller two port.
 *
 * @return  A unsigned 16 bit number contains all controller states.
 ******************************************************************************/
uint16_t getControllerTwo();

#endif
