/*******************************************************************************
 * @file    tms99XX.c
 * @brief   Library for TI TMS9918/28/29 video display processor.
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2022.02.12
 * @details 16K is assumed for memory.
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

#include <base.h>
#include <stdint.h>

#include <tms99XX.h>

/** SEE MY PRIVATES **/
/*** read VDP status register ***/
inline uint8_t readVDPstatus(struct s_tms99XX * const p_tms99XX);
/*** read VDP vram ***/
inline int readVDPvram(struct s_tms99XX * const p_tms99XX, uint8_t *p_data, int size, int modLen);
/*** write VDP vram ***/
inline int writeVDPvram(struct s_tms99XX * const p_tms99XX, uint8_t const * const p_data, int size, int modLen);
/*** set write or read VDP vram address ***/
inline void writeVDPvramAddr(struct s_tms99XX * const p_tms99XX, uint16_t address, int rnw);
/*** write VDP registers ***/
inline void writeVDPregister(struct s_tms99XX * const p_tms99XX, uint8_t regNum, uint8_t data);
/*** graphics mode ***/
inline void initVDPmode(struct s_tms99XX * const p_tms99XX);

/** INITIALIZE AND FREE MY STRUCTS **/

/*** Initialize TMS99XX struct with ports to use for input output, must match direction registers above. ***/
void initTMS99XX(struct s_tms99XX *p_tms99XX, uint8_t vdpMode, uint8_t backColor)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;

  /**** setup vdp struct ****/
  p_tms99XX->vdpMode = vdpMode;

  /**** clear register 0 ****/
  p_tms99XX->register0 = 0;

  /**** clear register 1, and set to 16k ****/
  p_tms99XX->register1 = (unsigned char)(1 << VMEM_AMT_BIT);

  p_tms99XX->colorReg = (unsigned char)(backColor & 0x0F);

  /**** set vdp addresses ****/
  p_tms99XX->nameTableAddr = NAME_TABLE_ADDR;

  p_tms99XX->colorTableAddr = COLOR_TABLE_ADDR;

  p_tms99XX->patternTableAddr = PATTERN_TABLE_ADDR;

  p_tms99XX->spriteAttributeAddr = SPRITE_ATTRIBUTE_TABLE_ADDR;

  p_tms99XX->spritePatternAddr = SPRITE_PATTERN_TABLE_ADDR;

  /**** init vdp with defaults ****/
  initVDPmode(p_tms99XX);
}

/*** Set the TMS99XX mode to one of 4. Text, Graphics I, Graphics II, and bitmap. ***/
void setTMS99XXmode(struct s_tms99XX * const p_tms99XX, uint8_t vdpMode)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;

  p_tms99XX->vdpMode = vdpMode;

  initVDPmode(p_tms99XX);
}

/*** Set the TMS99XX to blank the current sprite and pattern planes. ***/
void setTMS99XXblank(struct s_tms99XX * const p_tms99XX, uint8_t mode)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;

  /**** blank is active low, so for mode on (one) set reg bit to 0 ****/
  if(mode)
  {
    p_tms99XX->register1 &= (uint8_t)~(1 << BLK_SCRN_BIT);
  }
  else
  {
    p_tms99XX->register1 |= (uint8_t)(1 << BLK_SCRN_BIT);
  }

  writeVDPregister(p_tms99XX, REGISTER_1, p_tms99XX->register1);
}

/*** Set the TMS99XX to irq to enabled or disabled. ***/
void setTMS99XXirq(struct s_tms99XX * const p_tms99XX, uint8_t mode)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;
  
  if(mode)
  {
    p_tms99XX->register1 |= (uint8_t)(1 << IRQ_BIT);
  }
  else
  {
    p_tms99XX->register1 &= (uint8_t)~(1 << IRQ_BIT);
  }
  
  writeVDPregister(p_tms99XX, REGISTER_1, p_tms99XX->register1);
}

/*** Set the TMS99XX to sprite size to 8x8 or 16x16. ***/
void setTMS99XXspriteSize(struct s_tms99XX * const p_tms99XX, uint8_t mode)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;
  
  if(mode)
  {
    p_tms99XX->register1 |= (uint8_t)(1 << SPRITE_SIZE_BIT);
  }
  else
  {
    p_tms99XX->register1 &= (uint8_t)~(1 << SPRITE_SIZE_BIT);
  }
  
  writeVDPregister(p_tms99XX, REGISTER_1, p_tms99XX->register1);
}

/*** Set the TMS99XX to sprite magnify to on or off (double set size). ***/
void setTMS99XXspriteMagnify(struct s_tms99XX * const p_tms99XX, uint8_t mode)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;
  
  if(mode)
  {
    p_tms99XX->register1 |= (uint8_t)(1 << SPRITE_MAG_BIT);
  }
  else
  {
    p_tms99XX->register1 &= (uint8_t)~(1 << SPRITE_MAG_BIT);
  }
  
  writeVDPregister(p_tms99XX, REGISTER_1, p_tms99XX->register1);
}

/*** Set the TMS99XX text color in text mode. ***/
void setTMS99XXtxtColor(struct s_tms99XX * const p_tms99XX, uint8_t color)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;

  p_tms99XX->colorReg = (uint8_t)((p_tms99XX->colorReg & 0x0F) | ((color & 0x0F) << 4));

  writeVDPregister(p_tms99XX, REGISTER_7, p_tms99XX->colorReg);
}

/*** Set the TMS99XX background color. ***/
void setTMS99XXbackgroundColor(struct s_tms99XX * const p_tms99XX, uint8_t color)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;
  
  p_tms99XX->colorReg = (uint8_t)((p_tms99XX->colorReg & 0xF0) | (color & 0x0F));
  
  writeVDPregister(p_tms99XX, REGISTER_7, p_tms99XX->colorReg);
}

/*** Set a register with a 8 bit value. ***/
void setTMS99XXreg(struct s_tms99XX * const p_tms99XX, uint8_t regNum, uint8_t regData)
{
  writeVDPregister(p_tms99XX, regNum, regData);
}

/*** Write a struct/union table to vram using address. Alighned to data size. ***/
int setTMS99XXvramTableData(struct s_tms99XX * const p_tms99XX, uint16_t tableAddr, void const * const p_data, int startNum, int number, int size)
{
  int index = 0;
  
  /**** set starting vram address to write too ****/
  writeVDPvramAddr(p_tms99XX, tableAddr + (uint16_t)(size * startNum), 0); 
  
  return writeVDPvram(p_tms99XX, (uint8_t *)p_data, size * number, size * number);
}

/*** Set the start of the write VRAM address. After this is set writes will auto increment the address. ***/
void setTMS99XXvramWriteAddr(struct s_tms99XX * const p_tms99XX, uint16_t vramAddr)
{
  writeVDPvramAddr(p_tms99XX, vramAddr, 0);
}

/*** Set the start of the read VRAM. After this is set read will auto increment the address. ***/
void setTMS99XXvramReadAddr(struct s_tms99XX * const p_tms99XX, uint16_t vramAddr)
{
  writeVDPvramAddr(p_tms99XX, vramAddr, 1);
}

/*** Write array of byte data to VRAM. ***/
int setTMS99XXvramData(struct s_tms99XX * const p_tms99XX, void const * const p_data, int size)
{
  return writeVDPvram(p_tms99XX, (uint8_t *)p_data, size, size);
}

/*** constant value to VRAM. ***/
int setTMS99XXvramConstData(struct s_tms99XX * const p_tms99XX, uint8_t const data, int size)
{
  return writeVDPvram(p_tms99XX, &data, size, 1);
}

/*** set sprite to a terminator value ***/
void setTMS99XXvramSpriteTerm(struct s_tms99XX * const p_tms99XX, uint8_t const num)
{
  if(!p_tms99XX) return;

  /**** terminator sprite ****/
  union u_tms99XX_spriteAttributeTable spriteTerm = { .data = {0} };

  spriteTerm.dataNibbles.verticalPos = SPRITE_TERM;

  spriteTerm.dataNibbles.horizontalPos = 0;

  spriteTerm.dataNibbles.name = 0;

  spriteTerm.dataNibbles.earlyClockBit = 0;

  spriteTerm.dataNibbles.na = 0;

  spriteTerm.dataNibbles.colorCode = TMS_TRANSPARENT;

  writeVDPvramAddr(p_tms99XX, p_tms99XX->spriteAttributeAddr + (num * sizeof(spriteTerm)), 1);

  /**** no need to check return, plenty of time to write 4 bytes ****/
  writeVDPvram(p_tms99XX, (uint8_t const * const)&spriteTerm, sizeof(spriteTerm), sizeof(spriteTerm));
}

/*** Read array of byte data to VRAM. ***/
int getTMS99XXvramData(struct s_tms99XX * const p_tms99XX, void *p_data, int size)
{
  return readVDPvram(p_tms99XX, (uint8_t *)p_data, size, size);
}

/*** Read status register of VDP. ***/
uint8_t getTMS99XXstatus(struct s_tms99XX * const p_tms99XX)
{
  return readVDPstatus(p_tms99XX);
}

/*** clear data from VRAM. ***/
void clearTMS99XXvramData(struct s_tms99XX * const p_tms99XX)
{
  int      index = 0;
  uint8_t  data = 0x00;
  uint16_t amtWrote = 0;

  /**** set start address to write 0x00 to all of the VRAM ****/
  writeVDPvramAddr(p_tms99XX, 0x0000, 0);

  for(index = 0; index < MEM_SIZE; index += amtWrote)
  {
    amtWrote = (uint16_t)writeVDPvram(p_tms99XX, &data, MEM_SIZE - index, 1);
  }
}

/*** check vram with read write check ***/
uint8_t checkTMS99XXvram(struct s_tms99XX * const p_tms99XX)
{
  /**** future improvemtn, crc check of some sort ****/
  int      index = 0;
  uint16_t bufIndex = 0;
  uint8_t  buffer[256] = {0};
  uint8_t  data = 0x55;
  uint16_t amtRead = 0;
  uint16_t amtWrote = 0;

  /**** set start address to write 0x55 to all of the VRAM ****/
  writeVDPvramAddr(p_tms99XX, 0x0000, 0);

  for(index = 0; index < MEM_SIZE; index += amtWrote)
  {
    amtWrote = (uint16_t)writeVDPvram(p_tms99XX, &data, MEM_SIZE - index, 1);
  }

  /**** reset address to 0 for read ****/
  writeVDPvramAddr(p_tms99XX, 0x0000, 1);

  /**** loop in chunks of 256 to read vram ****/
  for(index = 0; index < MEM_SIZE; index += amtRead)
  {

    /**** read 256 chunk ****/
    amtRead = (uint16_t)readVDPvram(p_tms99XX, buffer, sizeof(buffer), sizeof(buffer));

    /**** check all chunks against original, return 0 if it fails ****/
    for(bufIndex = 0; bufIndex < amtRead; bufIndex++)
    {
      if(buffer[bufIndex] != data) return 0;
    }
  }

  /**** return 1 on success ****/
  return 1;
}

/** SEE MY PRIVATES **/
/*** read VDP status register ***/
inline uint8_t readVDPstatus(struct s_tms99XX * const p_tms99XX)
{
  uint8_t tempData;

  /**** NULL Check ****/
  if(!p_tms99XX) return 0;

  /**** read data ****/
  tempData = VDP_REG_PORT;

  return tempData;
  
}

/*** read VDP vram ***/
inline int readVDPvram(struct s_tms99XX * const p_tms99XX, uint8_t *p_data, int size, int modLen)
{
  int index = 0;

  /**** NULL Check ****/
  if(!p_tms99XX) return 0;

  if(!p_data) return 0;

  di();

  for(index = 0; index < size; index++)
  {
    /**** read data from port into array ****/
    p_data[index % modLen] = VDP_DATA_PORT;
  }

  /**** status read clears the interrupt, also screws up access if done before data transfer  ****/
  readVDPstatus(p_tms99XX);

  ei();

  return index;
}

/*** write VDP vram ***/
inline int writeVDPvram(struct s_tms99XX * const p_tms99XX, uint8_t const * const p_data, int size, int modLen)
{
  int index = 0;

  /**** NULL Check ****/
  if(!p_tms99XX) return 0;

  if(!p_data) return 0;

  di();

  for(index = 0; index < size; index++)
  {
    /**** write data to port from array of data at index ****/
    VDP_DATA_PORT = p_data[index % modLen];
  }

  /**** status read clears the interrupt, also screws up access if done before data transfer ****/
  readVDPstatus(p_tms99XX);

  ei();

  return index;
}

/*** write VDP registers ***/
inline void writeVDPregister(struct s_tms99XX * const p_tms99XX, uint8_t regNum, uint8_t data)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;

  di();

  /**** write msb as 1 and reg num to lower 3 bits ****/
  VDP_REG_PORT = data;

  VDP_REG_PORT = (unsigned char)(0x80 | regNum);

  ei();
}

/*** set write or read VDP vram address ***/
inline void writeVDPvramAddr(struct s_tms99XX * const p_tms99XX, uint16_t address, int rnw)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;

  di();

  /**** output bottom 8 bits of 14 bit address ****/
  VDP_REG_PORT = (unsigned char)(0xFF & address);

  /**** write bit 7 as 0, 6 as 1, and rest are top 6 bits of address ****/
  VDP_REG_PORT = (unsigned char)((rnw != 0 ? 0x00 : 0x40) | (unsigned char)(0x3F & (address >> 8)));

  ei();
}

/*** set modes by setting vdpMode ***/
/*** Default method per TI-VDP-Programmers_Guide.pdf ***/
inline void initVDPmode(struct s_tms99XX * const p_tms99XX)
{
  /**** NULL Check ****/
  if(!p_tms99XX) return;

  /**** only graphics mode II will set M3 (bit 0) to 1 ****/
  p_tms99XX->register0 = (uint8_t)(0x02 & (p_tms99XX->vdpMode << 1));

  /**** keep previous register 1 settings, only change VDP mode ****/
  p_tms99XX->register1 = (uint8_t)((p_tms99XX->register1 & 0xE3) | ((0x6 & p_tms99XX->vdpMode) << 2));

  /**** setup register 0 ****/
  writeVDPregister(p_tms99XX, REGISTER_0, p_tms99XX->register0);

  /**** setup regsiter 1 ****/
  writeVDPregister(p_tms99XX, REGISTER_1, p_tms99XX->register1);

  /**** setup register 2 for a name table address ****/
  writeVDPregister(p_tms99XX, REGISTER_2, (unsigned char)(p_tms99XX->nameTableAddr >> NAME_TABLE_ADDR_SCALE));

  if(p_tms99XX->vdpMode == GFXII_MODE)
  {
    /**** setup register 3 for a color table address GFX II has two fixed values for its only two addresses ****/
    writeVDPregister(p_tms99XX, REGISTER_3, (unsigned char)((p_tms99XX->colorTableAddr == 0x0000) ? 0x7F : 0xFF));

    /**** setup register 4 for pattern table address GFX II has two fixed values for its only two addresses  ****/
    writeVDPregister(p_tms99XX, REGISTER_4, (unsigned char)((p_tms99XX->patternTableAddr == 0x0000) ? 0x03 : 0x07));
  }
  else
  {
    if(p_tms99XX->vdpMode != TXT_MODE)
    {
      /**** setup register 3 for a color table address ****/
      writeVDPregister(p_tms99XX, REGISTER_3, (unsigned char)(p_tms99XX->colorTableAddr >> COLOR_TABLE_ADDR_SCALE));
    }

    /**** setup register 4 for pattern table address  ****/
    writeVDPregister(p_tms99XX, REGISTER_4, (unsigned char)(p_tms99XX->patternTableAddr >> PATTERN_TABLE_ADDR_SCALE));
  }


  if(p_tms99XX->vdpMode != TXT_MODE)
  {
    /**** setup register 5 for sprite attribute table address ****/
    writeVDPregister(p_tms99XX, REGISTER_5, (unsigned char)(p_tms99XX->spriteAttributeAddr >> SPRITE_ATTRIBUTE_TABLE_ADDR_SCALE));

    /**** setup register 6 for sprite pattern table address ****/
    writeVDPregister(p_tms99XX, REGISTER_6, (unsigned char)(p_tms99XX->spritePatternAddr >> SPRITE_PATTERN_TABLE_ADDR_SCALE));
  }

  /**** setup register 7 for backdrop color ****/
  writeVDPregister(p_tms99XX, REGISTER_7, p_tms99XX->colorReg);
}
