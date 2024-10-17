//**************************************************************************//**
// @file    hello_world.c
// @author  Jay Convertino
//******************************************************************************

#include <base.h>
#include <tms99XX.h>
#include <tms99XXascii.h>

#if defined(_COLECO_SGM) || defined(_MSX)
#include <gisnd.h>
#endif

#ifndef _MSX
#include <sn76489.h>
#endif

#if defined(_COLECO) || defined(_COLECO_SGM)
  __at 0x8024 const char game_info[] = "HELLO WORLD!\x1E\x1F/JAY CONVERTINO/2024";
#endif

void main(void)
{
  int index = 0;

  /* contains ti chip object */
  struct s_tms99XX tms99XX;

  uint8_t scrollArray[40] = {0};

  const char helloWorld[] = "Hello World!!!";

  const char tag[] = "2024 Jay Convertino";

  const char txtmode[] = "TXT";

  /* create struct to store ascii name table in order, removing first 32 null patterns */
  uint8_t nameTable[1 + (sizeof(c_tms99XX_ascii)/8) - 32] = {0};

  /* create nametable to display all ascii characters */
  for(index = 0; index < sizeof(nameTable); index++)
  {
    /** offset to skip first 32 nulls **/
    nameTable[index] = (unsigned char)index + 32;
  }

  /* setup tms9928 chip and finish setting up struct */
  initTMS99XX(&tms99XX, TXT_MODE, TMS_BLACK);

  /* clear all ti vdp memory */
  clearTMS99XXvramData(&tms99XX);

  setTMS99XXtxtColor(&tms99XX, TMS_WHITE);

  // setTMS99XXblank(&tms99XX, 1);

  /* ascii chars */
  setTMS99XXvramWriteAddr(&tms99XX, PATTERN_TABLE_ADDR);

  setTMS99XXvramData(&tms99XX, c_tms99XX_ascii, sizeof(c_tms99XX_ascii));

  /* first ascii letter is space in this table, no image */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR);

  setTMS99XXvramConstData(&tms99XX, 0, 0x7FF);

  /* write all ascii text */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR);

  setTMS99XXvramData(&tms99XX, nameTable, sizeof(nameTable));

  /* write hello world on line 12 */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * 11));

  setTMS99XXvramData(&tms99XX, helloWorld, sizeof(helloWorld));

  /* write 2022 Jay Convertino on last line (24 (23, offset 0)) */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * 23));

  setTMS99XXvramData(&tms99XX, tag, sizeof(tag));

  setTMS99XXvramData(&tms99XX, txtmode, sizeof(txtmode));

  /* enable screen */
  setTMS99XXblank(&tms99XX, 0);

#if defined(_COLECO_SGM) || defined(_MSX)
  setGISNDchannel_freq('A', 256);

  setGISNDmixer(~0, 0xFE);

  setGISNDchannel_attn('A', 15, 0);
#else
  setSN76489voice_attn(1, 2);
  /* set frequency to 440 hz */
  setSN76489voice_freq(1, 254);
#endif

  for(;;)
  {
     /* read name table */
    setTMS99XXvramReadAddr(&tms99XX, NAME_TABLE_ADDR + (40 * 11));

    getTMS99XXvramData(&tms99XX, scrollArray, sizeof(scrollArray));

    /* shift all data from name table */
    /* write to name tabble */
    setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * 11));

    setTMS99XXvramData(&tms99XX, &scrollArray[1], sizeof(scrollArray)-1);

    setTMS99XXvramData(&tms99XX, &scrollArray[0], 1);

    __delay_us(32000);
  }
}

void vdp_irq_callback(void)
{
  // application depended method of dealing with data.
  // read and write data from global buffer of 1000 bytes total (need to check this, write priority).
  // Struct { p_inputBuffer, p_outputBuffer, inputIndex, outputIndex}, with set/get functions
  // int setTMS99XXvramData(struct s_tms99XX * const p_tms99XX, void const * const p_data, int size);
  // int getTMS99XXvramData(struct s_tms99XX * const p_tms99XX, void *p_data, int size);
}
