 /*******************************************************************************
 * @file    tms99XXdefines.h
 * @brief   Defines for TI TMS9918/28/29 video display processor library.
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2022.04.24
 * 
 * @version 0.0.1
 * 
 * @TODO
 *  - EVERYTHING
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

#ifndef __LIB_TMS99XX_DEFINES
#define __LIB_TMS99XX_DEFINES

/** DEFINES **/
/** VDP MODE DEFINES **/
/**
 * @def GFXI_MODE
 * mode bit for graphics I mode
 */
#define GFXI_MODE 0
/**
 * @def GFXII_MODE
 * mode bit for graphics II mode
 */
#define GFXII_MODE 1
/**
 * @def BMP_MODE
 * mode bit for bitmap(multicolor) mode
 */
#define BMP_MODE 2
/**
 * @def TXT_MODE
 * mode bit for text mode
 */
#define TXT_MODE 4

/** register 1 bit defines **/
/**
 * @def VMEM_AMT_BIT
 * amount of vram bit
 */
#define VMEM_AMT_BIT 7
/**
 * @def BLK_SCRN_BIT
 * blank screen bit
 */
#define BLK_SCRN_BIT 6
/**
 * @def IRQ_BIT
 * IRQ bit number
 */
#define IRQ_BIT 5
/**
 * @def SPRITE_SIZE_BIT
 * sprite size bit
 */
#define SPRITE_SIZE_BIT 1
/**
 * @def SPRITE_MAG_BIT
 * sprite magnification bit
 */
#define SPRITE_MAG_BIT 0

/** REGISTER DEFINES **/
/**
 * @def REGISTER_0
 * mode and external video bits
 */
#define REGISTER_0 0
/**
 * @def REGISTER_1
 * mode, sprite, interrupt, blank, and memory bits.
 */
#define REGISTER_1 1
/**
 * @def REGISTER_2
 * name table address
 */
#define REGISTER_2 2
/**
 * @def REGISTER_3
 * color table address
 */
#define REGISTER_3 3
/**
 * @def REGISTER_4
 * pattern table address
 */
#define REGISTER_4 4
/**
 * @def REGISTER_5
 * sprite attribute address
 */
#define REGISTER_5 5
/**
 * @def REGISTER_6
 * sprite pattern address
 */
#define REGISTER_6 6
/**
 * @def REGISTER_7
 * background, text color
 */
#define REGISTER_7 7

/** VRAM ADDRESS DEFINES **/
/**
 * @def NAME_TABLE_ADDR
 * default address for name table (R2 * 0x400[2^10])
 * 0x3800 address is set when R2 is 0x0E
 */
#define NAME_TABLE_ADDR 0x3800
/**
 * @def NAME_TABLE_ADDR_SCALE
 * value to scale the name table address
 */
#define NAME_TABLE_ADDR_SCALE 10
/**
 * @def COLOR_TABLE_ADDR
 * default address for color table (R3 * 0x40[2^6])
 * 0x2000 address is set when R3 is 0x80.
 * 
 * GFXII can only be 0x0000(0x7F) or 0x2000(0xFF) Init takes care of this, but
 * this can be easily broken if a bad address is used for the color table.
 */
#define COLOR_TABLE_ADDR 0x2000
/**
 * @def COLOR_TABLE_ADDR_SCALE
 * value to scale the color table address
 */
#define COLOR_TABLE_ADDR_SCALE 6
/**
 * @def PATTERN_TABLE_ADDR
 * default address for pattern table (R4 * 0x800[2^11])
 * 0x0000 address is set when R4 is 0x00
 * 
 * GFXII can only be 0x0000(0x03) or 0x2000(0x07) Init takes care of this, but 
 * this can be easily broken if a bad address is used for the pattern table.
 */
#define PATTERN_TABLE_ADDR 0x0000
/**
 * @def PATTERN_TABLE_ADDR_SCALE
 * value to scale the pattern table address
 */
#define PATTERN_TABLE_ADDR_SCALE 11
/**
 * @def SPRITE_ATTRIBUTE_TABLE_ADDR
 * default address for sprite attribute table (R5 * 0x80[2^7])
 * 0x3B80 address is set when R5 is 0x77
 */
#define SPRITE_ATTRIBUTE_TABLE_ADDR 0x3B80
/**
 * @def SPRITE_ATTRIBUTE_TABLE_ADDR_SCALE
 * value to scale the sprite attribute address
 */
#define SPRITE_ATTRIBUTE_TABLE_ADDR_SCALE 7
/**
 * @def SPRITE_PATTERN_TABLE_ADDR
 * default address for sprite pattern table (R6 * 0x800[2^11])
 * 0x1800 address is set when R6 is 0x03
 */
#define SPRITE_PATTERN_TABLE_ADDR 0x1800
/**
 * @def SPRITE_PATTERN_TABLE_ADDR_SCALE
 * value to scale the sprite attribute address
 */
#define SPRITE_PATTERN_TABLE_ADDR_SCALE 11

/** COLOR DEFINES **/
/**
 * @def TMS_TRANSPARENT
 * transparent for all plans/sprites
 */
#define TMS_TRANSPARENT 0x00
/**
 * @def TMS_BLACK
 * color black
 */
#define TMS_BLACK 0x01
/**
 * @def TMS_MEDIUM_GREEN
 * color medium green
 */
#define TMS_MEDIUM_GREEN 0x02
/**
 * @def TMS_LIGHT_GREEN
 * color light green
 */
#define TMS_LIGHT_GREEN 0x03
/**
 * @def TMS_DARK_BLUE
 * color dark blue
 */
#define TMS_DARK_BLUE 0x04
/**
 * @def TMS_LIGHT_BLUE
 * color light blue
 */
#define TMS_LIGHT_BLUE 0x05
/**
 * @def TMS_DARK_RED
 * color dark red
 */
#define TMS_DARK_RED 0x06
/**
 * @def TMS_CYAN
 * color cyan
 */
#define TMS_CYAN 7
/**
 * @def TMS_MEDIUM_RED
 * color medium red
 */
#define TMS_MEDIUM_RED 0x08
/**
 * @def TMS_LIGHT_RED
 * register
 */
#define TMS_LIGHT_RED 0x09
/**
 * @def TMS_DARK_YELLOW 
 * register
 */
#define TMS_DARK_YELLOW 0x0A
/**
 * @def TMS_LIGHT_YELLOW
 * register
 */
#define TMS_LIGHT_YELLOW 0x0B
/**
 * @def TMS_DARK_GREEN
 * register
 */
#define TMS_DARK_GREEN 0x0C
/**
 * @def TMS_MAGENTA
 * register
 */
#define TMS_MAGENTA 0x0D
/**
 * @def TMS_GREY
 * register
 */
#define TMS_GREY 0x0E
/**
 * @def TMS_WHITE
 * register
 */
#define TMS_WHITE 0x0F

/** MISC DEFINES **/
/**
 * @def MEM_SIZE
 * 16K of memory: 0 to 0x3FFF (0x4000)
 */
#define MEM_SIZE (1 << 14)

/**
 * @def SPRITE_TERM
 * Vertical field can contain a terminator value of 0xD0 to stop sprite processing.
 */
#define SPRITE_TERM 0xD0

#endif
