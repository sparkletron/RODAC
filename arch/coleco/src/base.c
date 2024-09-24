/**************************************************************************//**
 * @file    base.c
 * @author  Jay Convertino
 ******************************************************************************/

#include "base.h"

volatile void (*vdp_callback)(void) = 0;

volatile void (*spin_callback)(void) = 0;

void __delay_us(int count)
{
  for(; count > 0; count--)
  {
    __asm
      nop
    __endasm;
  }
}

void vdp_irq(void)
{
  if(vdp_callback) (*vdp_callback)();
}

void spin_irq(void)
{
  if(spin_callback) (*spin_callback)();
}

void set_vdp_irq_callback(void (*callback)(void))
{
  di();

  vdp_callback = callback;

  ei();
}

void set_spin_irq_callback(void (*callback)(void))
{
  di();

  spin_callback = callback;

  ei();
}
