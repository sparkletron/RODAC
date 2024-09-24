/*******************************************************************************
 * @file    tms99XXdatatypes.h
 * @brief   Datatypes for TI TMS9918/28/29 video display processor library.
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

#ifndef __LIB_TMS99XX_DATATYPE
#define __LIB_TMS99XX_DATATYPE

/** DATA STRUCTURES **/
/**
 * @struct s_tms99XX
 * @brief Struct for containing TMS99XX instances 
 */
struct s_tms99XX
{
  /**
   * @var s_tms99XX::spritePatternAddr
   * sprite pattern table address in VRAM
   */
  uint16_t spritePatternAddr;
  /**
   * @var s_tms99XX::spriteAttributeAddr
   * sprite attribute table address in VRAM
   */
  uint16_t spriteAttributeAddr;
  /**
   * @var s_tms99XX::patternTableAddr
   * pattern table address in VRAM
   */
  uint16_t patternTableAddr;
  /**
   * @var s_tms99XX::nameTableAddr
   * name table address in VRAM
   */
  uint16_t nameTableAddr;
  /**
   * @var s_tms99XX::colorTableAddr
   * color table address in VRAM
   */
  uint16_t colorTableAddr;
    /**
   * @var s_tms99XX::vdpMode
   * contains current mode of the VDP
   */
  uint8_t vdpMode;
  /**
   * @var s_tms99XX::register0
   * register 0 contents
   */
  uint8_t register0;
  /**
   * @var s_tms99XX::register1
   * register 1 contents
   */
  uint8_t register1;
  /**
   * @var s_tms99XX::colorReg
   * color sent to register 7, background/text color.
   */
  uint8_t colorReg;
};

/**
 * @union u_tms99XX_patternTable8x8
 * @brief Struct for containing a 8x8 pattern table
 */
union u_tms99XX_patternTable8x8
{
  /**
   * @var u_tms99XX_patternTable8x8::data
   * array of 8 bytes for a 8x8 matrix.
   */
  uint8_t data[8];
};

/**
 * @union u_tms99XX_BMPpixelBlock
 * @brief Struct for containing a single multicolor mode pixel block
 */
union u_tms99XX_BMPpixelBlock
{
  /**
  * @struct u_tms99XX_BMPpixelBlock::s_pDataNibbles
  * contains color fields for bitmap mode. see page 8-13 of TI-VDP-Programmers_guide.pdf
  * |--------|--------|
  * |color A |color B |
  * |--------|--------|
  * |color C |color D |
  * |--------|--------|
  *
  * The above is a mapping of the pixel pattern on screen. The resolution is
  * 64x48 pixels. Each row has rules for indexing from the name table into the
  * pattern table. Rows 0,4,8,12,16,20 index into the first pixel block at
  * offsets of 8 bytes. A name table value of 0, will be pattern pixel block 0.
  * A name table value of 1 will actually map to byte offset of 8, the next chunk
  * of pixels. Pixels are chunked in 8 byte sections. Since the rows name lookup
  * will loop after 4 rows. Meaning Row 1,5,9,13,17,21 name table will resolve to
  * the same value in the pattern table. Same for Row 0,4,8,12,16,20. To be clear
  * row 0, name table value 0 gets pattern 0. Row 4, name table value 0 gets
  * pattern 0 as well.
  */
  struct s_pDataNibbles
  {
    /**
    * @var  u_tms99XX_BMPpixelBlock::s_pDataNibbles::colorA
    * color lower nibble
    */
    uint8_t colorB:4;
    /**
    * @var  u_tms99XX_BMPpixelBlock::s_pDataNibbles::colorB
    * color upper nibble
    */
    uint8_t colorA:4;
    /**
    * @var  u_tms99XX_BMPpixelBlock::s_pDataNibbles::colorC
    * color lower nibble
    */
    uint8_t colorD:4;
    /**
    * @var  u_tms99XX_BMPpixelBlock::s_pDataNibbles::colorD
    * color upper nibble
    */
    uint8_t colorC:4;
  } dataNibbles;
  
  /**
   * @var u_tms99XX_BMPpixelBlock::data
   * array of 2 bytes for a 2x2 4 pixel map.
   */
  uint8_t data[2];
};

/**
 * @struct s_tms99XX_nameTable
 * @brief Struct for containing TMS99XX instances 
 */
struct s_tms99XX_nameTable
{
  /**
   * @var s_tms99XX_nameTable::data
   * byte for storing name table data.
   */
  uint8_t data;
};

/**
 * @union u_tms99XX_colorTable
 * @brief Union for containing TMS99XX color table data. 
 */
union u_tms99XX_colorTable
{
  /**
  * @struct u_tms99XX_colorTable::s_cDataNibbles
  * contains color fields
  */
  struct s_cDataNibbles
  {
    /**
    * @var u_tms99XX_colorTable::s_cDataNibbles::color2
    * bottom nibble, color for 0
    */
    uint8_t color2:4;
    /**
    * @var u_tms99XX_colorTable::s_cDataNibbles::color1
    * top nibble, color for 1
    */
    uint8_t color1:4;
  } dataNibbles;
  
  /**
  * @var u_tms99XX_colorTable::data
  * contains color data byte
  */
  uint8_t data;
};

/**
 * @struct s_tms99XX_spritePatternTable8x8
 * @brief Struct for containing a 8x8 sprite pattern
 */
struct s_tms99XX_spritePatternTable8x8
{
  /**
  * @var s_tms99XX_spritePatternTable8x8::data
  * contains 8x8 matrix of sprite pattern data
  */
  uint8_t data[8];
};

/**
 * @struct s_tms99XX_spritePatternTable16x16
 * @brief Struct for containing a 16x16 sprite pattern
 */
struct s_tms99XX_spritePatternTable16x16
{
  /**
  * @var s_tms99XX_spritePatternTable16x16::data
  * contains 16x16 matrix of sprite pattern data.
  * First 8, upper left.
  * Second 8, lower left.
  * Third 8, upper right.
  * Last 8, lower right.
  */
  uint8_t data[32];
};

/**
 * @union u_tms99XX_spriteAttributeTable
 * @brief union for containing sprite attribute data.
 */
union u_tms99XX_spriteAttributeTable
{
  /**
  * @struct u_tms99XX_spriteAttributeTable::s_dataNibbles
  * contains sprite attribute fields
  */
  struct s_dataNibbles
  {
    /**
    * @var u_tms99XX_spriteAttributeTable::s_dataNibbles::verticalPos
    * vertical position of sprite on screen.
    */
    uint8_t verticalPos;
    /**
    * @var u_tms99XX_spriteAttributeTable::s_dataNibbles::horizontalPos
    * horizontal position of sprite on screen.
    */
    uint8_t horizontalPos;
    /**
    * @var u_tms99XX_spriteAttributeTable::s_dataNibbles::name
    * name of sprite on screen (pointer to pattern table object).
    */
    uint8_t name;
    /**
    * @var u_tms99XX_spriteAttributeTable::s_dataNibbles::colorCode
    * color to set sprite pixels. this is the lower 4 bits (3 to 0)
    */
    uint8_t colorCode:4;
    /**
    * @var u_tms99XX_spriteAttributeTable::s_dataNibbles::na
    * nothing, always set to 0. bits 6 to 4.
    */
    uint8_t na:3;
      /**
    * @var u_tms99XX_spriteAttributeTable::s_dataNibbles::earlyClockBit
    * feature for slow movement of sprites off screen. bit 7
    */
    uint8_t earlyClockBit:1;
  } dataNibbles;
  
  /**
  * @var u_tms99XX_spriteAttributeTable::data
  * array of sprite attribute data
  */
  uint8_t data[4];
};

#endif
