//**************************************************************************//**
// @file    multicart.c
// @author  Jay Convertino
//******************************************************************************

#include <base.h>
#include <tms99XX.h>
#include <tms99XXascii.h>
#include <stdio.h>
#include <roms.h>

#if defined(_COLECO) || defined(_COLECO_SGM)
  __at 0x8024 const char game_info[] = "MULTICART BY JAY CONVERTINO/ /2024";
#endif

void main(void)
{
  uint8_t index = 0;
  uint8_t prev_index = 0;

  volatile uint8_t buffer = (uint8_t)~0;

  uint8_t counter = 0;

  const uint8_t num_of_roms = sizeof(roms) / sizeof(roms[0]);

  /* contains ti chip object */
  struct s_tms99XX tms99XX;

  const char title[] = "       PRESS FIRE TO SELECT A ROM";

  const char tag[] = "2024 Jay Convertino            MULTICART";

  const char line[] = "========================================";

  const char loading[] = "  R E S E T  T O  S T A R T  G A M E  ";

  volatile uint8_t *bank_switch = (uint8_t *)0xE001;

  /* setup tms9928 chip and finish setting up struct */
  initTMS99XX(&tms99XX, TXT_MODE, TMS_BLACK);

  setTMS99XXtxtColor(&tms99XX, TMS_WHITE);

  /* ascii chars */
  setTMS99XXvramWriteAddr(&tms99XX, PATTERN_TABLE_ADDR);

  setTMS99XXvramData(&tms99XX, c_tms99XX_ascii, sizeof(c_tms99XX_ascii));

  /* invert ascii chars */
  for(int inv_index = 0; inv_index < sizeof(c_tms99XX_ascii); inv_index++)
  {
    /**** write data to port from array of data at index ****/
    VDP_DATA_PORT = ~(((uint8_t *)c_tms99XX_ascii)[inv_index]);
  }

  /* first ascii letter is space in this table, no image */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR);

  setTMS99XXvramConstData(&tms99XX, 0, 0x7FF);

  /* write title on line 0 */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * 0));

  setTMS99XXvramData(&tms99XX, title, sizeof(title));

  /* write a line on line 1 */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * 1));

  setTMS99XXvramData(&tms99XX, line, sizeof(line));

  /* write 20XX Jay Convertino on last line (24 (23, offset 0)) */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * 23));

  setTMS99XXvramData(&tms99XX, tag, sizeof(tag));

  /* populate game names as a list on screen, highlight first game */
  for(int rom_index = 0; rom_index < num_of_roms; rom_index++)
  {
    setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * (rom_index+2)));

    if(rom_index == 0)
    {
      for(int string_index = 0; string_index < sizeof(roms[rom_index]); string_index++)
      {
        /**** write data to port from array of data at index ****/
        VDP_DATA_PORT = roms[rom_index][string_index] + 0x80;
      }
    }
    else
    {
      setTMS99XXvramData(&tms99XX, roms[rom_index], sizeof(roms[rom_index]));
    }
  }

  /* enable screen */
  setTMS99XXblank(&tms99XX, 0);

  for(;;)
  {
    /* get controller one input */
    uint8_t controller = getControllerOne();

    /* when fire is pressed, start selected game */
    if(!((controller >> FIRE_BIT) & 0x01))
    {
      setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * 0));
      setTMS99XXvramData(&tms99XX, loading, sizeof(loading));
      //do read at E001 + index
      //I do this 4 times do to the pic i use having a 200 ns instruction time
      //problem is the select is only 620ns on the coleco, which for all the
      //instructions I needed takes 1000ns to complete and read again.
      //this means a pulse could be missed if a sample is taken just before.
      //Doing this 4 times solves this issue.
      buffer = *(bank_switch + index);
      buffer = *(bank_switch + index);
      buffer = *(bank_switch + index);
      buffer = *(bank_switch + index);
      //It seems that Atari games do not like the reset. So halt and let the user
      //do it. User is slow enough to sync the systems without issue.
      __asm__("halt");
    }
    /* when up is pressed, and its 0 shift a buffer till its 0, then decrement index to move the highlight up the screen */
    else if(!((controller >> UP_BIT) & 0x01))
    {
      buffer = buffer << 1;

      if(!buffer)
      {
        index = (index > 0 ? (index - 1) : num_of_roms-1);

        buffer = (uint8_t)~0;
      }
    }
    /* when down is pressed, and its 0 shift a buffer till its 0, then increment index to move the highlight down the screen */
    else if(!((controller >> DOWN_BIT) & 0x01))
    {
      buffer = buffer << 1;

      if(!buffer)
      {
        index = (index < num_of_roms-1 ? (index + 1) : 0);

        buffer = (uint8_t)~0;
      }
    }
    /* nothing? then do a count to allow quick presses to shift the buffer, and if its been to long, clear everything out */
    else
    {
      counter += 1;

      if(counter > 32)
      {
        counter = 0;
        buffer = (uint8_t)~0;
      }
    }

    /* a button has been pressed and passed indexs are no longer equal so we update the screen */
    if(index != prev_index)
    {
      setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * (prev_index+2)));

      /* write the previous string that was inverted, as a normal string */
      setTMS99XXvramData(&tms99XX, roms[prev_index], sizeof(roms[prev_index]));


      setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (40 * (index+2)));

      /* use the new index to write the inverted version of the string */
      for(int string_index = 0; string_index < sizeof(roms[index]); string_index++)
      {
        /**** write data to port from array of data at index ****/
        VDP_DATA_PORT = roms[index][string_index] + 0x80;
      }

      prev_index = index;
    }

    __delay_us(750);
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
