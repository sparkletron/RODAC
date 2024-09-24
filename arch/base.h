/**************************************************************************//**
 * @file    base.h
 * @author  Jay Convertino
 ******************************************************************************/

#ifndef __BASE
#define __BASE

#include "defines.h"

void __delay_us(int count);

void set_vdp_irq_callback(void (*callback)(void));

void set_spin_irq_callback(void (*callback)(void));

#endif
