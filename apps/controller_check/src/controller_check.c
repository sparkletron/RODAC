//**************************************************************************//**
// @file    hello_world.c
// @author  Jay Convertino
//******************************************************************************

#include <base.h>
#include <tms99XX.h>
#include <tms99XXascii.h>

#define LAST_LINE 23
#define CHAR_PER_LINE 40
#define BUTTON_LINE 12

#if defined(_COLECO) || defined(_COLECO_SGM)
  __at 0x8024 const char game_info[] = "CONTROLLER CHECK BY JAY/ /2024";
#endif

void main(void)
{
  int index = 0;

  /* contains ti chip object */
  struct s_tms99XX tms99XX;

  const char title[] = "Controller Check";

  const char tag[] = "2024 Jay Convertino";

  /* setup tms9928 chip and finish setting up struct */
  initTMS99XX(&tms99XX, TXT_MODE, TMS_BLACK);

  setTMS99XXtxtColor(&tms99XX, TMS_WHITE);

  /* ascii chars */
  setTMS99XXvramWriteAddr(&tms99XX, PATTERN_TABLE_ADDR);

  setTMS99XXvramData(&tms99XX, c_tms99XX_ascii, sizeof(c_tms99XX_ascii));

  /* write title on line 0 */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR);

  setTMS99XXvramData(&tms99XX, title, sizeof(title));

  /* write 2024 Jay Convertino on last line (24 (23, offset 0)) */
  setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (CHAR_PER_LINE * LAST_LINE));

  setTMS99XXvramData(&tms99XX, tag, sizeof(tag));

  /* enable screen */
  setTMS99XXblank(&tms99XX, 0);

  for(;;)
  {
    volatile uint16_t controller = getControllerOne();

    setTMS99XXvramWriteAddr(&tms99XX, NAME_TABLE_ADDR + (CHAR_PER_LINE * BUTTON_LINE));

    //keypad mask
    /* when fire is pressed, start selected game */
    if(!((controller >> FIRE_BIT) & 0x0001))
    {
      setTMS99XXvramData(&tms99XX, "FIRE  ", 6);
    }
    else if(!((controller >> ARM_BIT) & 0x0001))
    {
      setTMS99XXvramData(&tms99XX, "ARM   ", 6);
    }
    else if(!((controller >> UP_BIT) & 0x0001))
    {
      setTMS99XXvramData(&tms99XX, "UP    ", 6);
    }
    else if(!((controller >> LEFT_BIT) & 0x0001))
    {
      setTMS99XXvramData(&tms99XX, "LEFT  ", 6);
    }
    else if(!((controller >> RIGHT_BIT) & 0x0001))
    {
      setTMS99XXvramData(&tms99XX, "RIGHT ", 6);
    }
    else if(!((controller >> DOWN_BIT) & 0x0001))
    {
      setTMS99XXvramData(&tms99XX, "DOWN  ", 6);
    }
    else if(((controller >> KEYPAD_BIT) & 0x000F) != 0x000F)
    {
      switch((controller >> KEYPAD_BIT) & 0x000F)
      {
        case 10:
          setTMS99XXvramData(&tms99XX, "0    ", 6);
          break;
        case 13:
          setTMS99XXvramData(&tms99XX, "1    ", 6);
          break;
        case 7:
          setTMS99XXvramData(&tms99XX, "2    ", 6);
          break;
        case 12:
          setTMS99XXvramData(&tms99XX, "3    ", 6);
          break;
        case 2:
          setTMS99XXvramData(&tms99XX, "4    ", 6);
          break;
        case 3:
          setTMS99XXvramData(&tms99XX, "5    ", 6);
          break;
        case 14:
          setTMS99XXvramData(&tms99XX, "6    ", 6);
          break;
        case 5:
          setTMS99XXvramData(&tms99XX, "7    ", 6);
          break;
        case 1:
          setTMS99XXvramData(&tms99XX, "8    ", 6);
          break;
        case 11:
          setTMS99XXvramData(&tms99XX, "9    ", 6);
          break;
        case 9:
          setTMS99XXvramData(&tms99XX, "*    ", 6);
          break;
        case 6:
          setTMS99XXvramData(&tms99XX, "#    ", 6);
          break;
      }
    }
    else
    {
      setTMS99XXvramConstData(&tms99XX, 0, 40);
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

