/**************************************************************************//**
 * @file    defines.h
 * @author  Jay Convertino
 ******************************************************************************/

#ifndef __DEFINES
#define __DEFINES

#define VDP_DATA_ADDR     0xBE
#define VDP_REG_ADDR      0xBF
#define SN_SND_ADDR       0xFF
#define CTRL_STR_SET_ADDR 0x80
#define CTRL_STR_RST_ADDR 0xC0
#define CTRL_ONE_ADDR     0xFC
#define CTRL_TWO_ADDR     0xFF

__sfr __at(VDP_DATA_ADDR)     VDP_DATA_PORT;
__sfr __at(VDP_REG_ADDR)      VDP_REG_PORT;
__sfr __at(SN_SND_ADDR)       SN_SND_PORT;
__sfr __at(CTRL_STR_SET_ADDR) CTRL_STR_SET_PORT;
__sfr __at(CTRL_STR_RST_ADDR) CTRL_STR_RST_PORT;
__sfr __at(CTRL_ONE_ADDR)     CTRL_ONE_PORT;
__sfr __at(CTRL_TWO_ADDR)     CTRL_TWO_PORT;

#define ei() __asm ei __endasm;
#define di() __asm di __endasm;

#endif
