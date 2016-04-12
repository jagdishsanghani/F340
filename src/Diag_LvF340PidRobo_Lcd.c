/**
  ******************************************************************************
  * @file    Diag_F340_Lcd.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   LCD module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of LCD module:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup LCD
  * @ LCD modules
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Control bytes */
                                  /* CO RS 0 0 - 0 0 0 0     */
#define  SEL_INST_REG      0x00   /* 0  0  0 0 - 0 0 0 0     */
                                  /* CO = 0, RS = 0  next byte is control byte */

#define  SEL_DATA_REG      0x40   /* 0  1  0 0 - 0 0 0 0     */
                                  /* CO = 0, RS = 1, next byte is data byte */

/*BASIC INSTRUCTIONS(bit H=0) */

                                  /* RS R/nW D7 D6 D5 D4 - D3  D2  D1  D0 */
#define  NOP               0x00   /*  0  0   0  0  0  0    0   0   0   0  */

#define  FUNC_SET_BASIC    0x24   /*  0  0   0  0  1  DL   0   M   SL  H      */
                                  /*  0  0   0  0  1  0    0   1   0   0      */

#define  BF_AC             0x00   /*  0  0   BF AC 0  0    0   0   0   0      */

#define  READ_DATA         0xC0   /*  1  1           READ DATA                */

#define  WRITE_DATA        0x80   /*  1  0          WRITE DATA                */


/* STANDARD INSTRUCTIONS(bit H=0) */

                                  /* RS R/nW D7 D6 D5 D4 - D3  D2  D1  D0  */
#define  CLEAR_DISPLAY     0x01   /*  0  0   0  0  0  0    0   0   0   1   */
                                  /* clear entire display, set DDRAM address counter = 0 */

#define  RETURN_HOME       0x02   /*  0  0   0  0   0  0   0   0   1   0     */
                                  /* DDRAM address counter = 0, Cursor back  */
                                  /* to the left of the first display line   */

#define  ENTRY_MODE_SET    0x06   /*  0  0   0  0   0  0   0   1  I_D  S     */
                                  /*  0  0   0  0   0  0   0   1   1   0     */
                                  /* DDRAM or CGRAM address increment by 1. S: no shift  */

#define  DISPLAY_CTL       0x0C   /*  0  0   0  0   0  0   1   D   C   B     */
                                  /*  0  0   0  0   0  0   1   1   0   0     */
                                  /* D: Display ON, C: Cursor OFF, B: Blink OFF */

#define  DISPLAY_OFF       0x08   /*  0  0   0  0   0  0   1   D   C   B     */
                                  /*  0  0   0  0   0  0   1   0   0   0     */

#define  DISPLAY_ON        0x0C   /*  0  0   0  0   0  0   1   D   C   B     */
                                  /*  0  0   0  0   0  0   1   1   0   0     */

#define  CURS_DISP_SHIFT   0x10   /*  0  0   0  0   0  1   SC  RL  0   0     */
                                  /*  0  0   0  0   0  1   0   0   0   0     */
                                  /* display on, cursor on, blink on         */

#define  SET_CGRAM         0x8C   /*  0  0   0  1   A  A   A   A   A   A     */
                                  /*  0  0   0  1   0  0   0   1   1   0     */
                                  /* Set DDRAM to 12, most right position.       */
                                  /* Display shift is enabled right to left and  */
                                  /* the content will enter the display here and */
                                  /* shift to the left */

#define  SET_DDRAM_L1      0x80   /*  0  0   1  A   A  A   A   A   A   A     */
                                  /*  0  0   1  0   0  0   0   0   0   0     */
                                  /* 1000-0000. Set DDRAM to 0x40, most right position. */
                                  /* Display shift is enabled right to left and the content */
                                  /* will enter the display here and shift to the left */

#define  SET_DDRAM_L2      0xC0   /*  0  0   1  A   A  A   A   A   A   A     */
                                  /*  0  0   1  1   0  0   0   0   0   0     */
                                  /* 1100-0000. Set DDRAM to 0x40, most right position. */
                                  /* Display shift is enabled right to left and the content */
                                  /* will enter the display here and shift to the left */

/* EXTENDED INSTRUCTIONS(bit H=1) RS=0, R/nW=0  */

                                  /* RS R/nW D7 D6 D5 D4 - D3  D2  D1  D0     */
#define  FUNC_SET_EXT      0x25   /*  0  0 # 0  0  1  DL   0   M   SL  H      */
                                  /*  0  0 # 0  0  1  0    0   1   0   1      */
                                  /* 2 x 16, 1/18 duty, extended inst. set.   */
                                  /* Next byte will be another command.       */
                                  /* DL= 8 bits data length; M=1->2x16; SL=0->multiplex 1:18 */
                                  /* H=1->basic + extended inst set           */

/*  1  2  3            14  15  16 |<-non displayed ddram address->|         */
/*  00 01 02 --------- 0D  0E  0F | 10 11 12 ------------25 26 27 | LINE 1  */

/*  40 41 42 --------- 4D  4E  4F | 50 51 52 ------------65 66 67 | LINE 2  */


#define  SCREEN_CONF       0x02   /*  0  0 # 0  0  0  0  - 0   0   1   L      */
                                  /*  0  0 # 0  0  0  0    0   0   1   0      */
                                  /* L=0 split screen standard connection     */

#define  DISP_CONF         0x04   /*  0  0 # 0  0  0  0  - 0   1   P   Q      */
                                  /*  0  0 # 0  0  0  0    0   1   0   0      */
                                  /* Set display config.P=0; Column data->left to right, */
	                              /* Q=0; Row data-> top to bottom,1 to 16.   */

#define  ICON_CTL          0x08   /*  0  0 # 0  0  0  0  - 1   IM  IB  DM     */
                                  /*  0  0 # 0  0  0  0    1   0   0   0      */
                                  /* Set IconMode=0 character mode, full display*/
                                  /* IconBlink=0  icon blink disabled, DirectMode=0 Off */

#define  TEMP_CTL          0x10   /*  0  0 # 0  0  0  1  - 0   0   TC1  TC2   */
                                  /*  0  0 # 0  0  0  1    0   0   0    0     */
                                  /* Vlco Temp. Coef. 0,->0.16%/K             */

#define  HV_GEN            0x42   /*  0  0 # 0  1  0  0  - 0   0   S1   S0    */
                                  /*  0  0 # 0  1  0  0    0   0   1    0     */
                                  /* Set voltage multiplier to 2              */

#define  VLCD_SET          0xAA   /*  0  0 # 1  V  ------VA or VB---------    */
                                  /*  0  0 # 1  0  1  0    0   0   0    0     */
                                  /* Set Vlcd and store in register VA        */

#define R_SPACE            0xA0   /* R CGROM space */

#define LCD_BUF_SIZE       18     /* Command : CharPerLine : NullCharForString */
                                  /*    1    +      16     +       1   = 18    */

#define MINUS_SIGN  0xAD;  /* put minus sign in location result[0] */

#define ERASE              0xA0   /* R CGROM space */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* INITIALIZE PARAMETERS FOR LCD */ 
char code Pcf2119Reg[] = {
  /* command bytes to configure pcf2119 reg */
  SEL_INST_REG,   /* 0x00 -> MSB (Continuation bit Co) = 0, more than one byte */
                    /* may follow. Bit6, RS=0, next byte is command byte */

    FUNC_SET_BASIC, /* 0x24 -> Until here settings which are really related to the display  */
                    /* being used, also HW-aspect Change from extended instruction */
                    /* set to basic instruction set */

    DISPLAY_OFF,    /* 0x08 -> Display control: set display off, cursor off, no blink */

    ENTRY_MODE_SET, /* 0x06 -> Entry mode set, increase DDRAM after access, no shift mode */
 
    FUNC_SET_EXT,   /* 0x25 -> 2 lines x 16, 1/18 duty, extended instruction set. */

    DISP_CONF,      /* 0x04 -> 0 0 0 0-0 1 P Q; Set display configuration to  right to left,*/
	                /*  right to left, column 80 to 1. Row data displ. top to bottom,1 to 16. */

    TEMP_CTL,       /* Vlco Temp. Coef. 0, 0x10 -> 0.16%/K      */

    HV_GEN,         /* 0x42 -> Set voltage multiplier to 2 */

    VLCD_SET,       /* 0xAA -> Set Vlcd and store in register VA */

    FUNC_SET_BASIC, /* 0x24 -> Until here settings which are really related to the display  */
                    /* being used, also HW-aspect Change from extended instruction */
                    /* set to basic instruction set */

    SET_DDRAM_L1,   /* 0x80 -> 1 A A A - A A A A;  1000-1100. Set DDRAM to 0x00, most right position. */
                    /* Display shift is enabled right to left and the content will enter the display */
                    /* here and shift to the left */  

    RETURN_HOME,    /* DDRAM address counter = 0, Cursor back to the left of the first display line */

    CLEAR_DISPLAY   /* clear entire display, set DDRAM address counter = 0 */

  }; 

/* PARAMETERS FOR DISPLAY ON */ 
    char code SetLcdDisplayOn[] = { /* command bytes to configure pcf2119 reg */

    SEL_INST_REG,   /* 0x00 -> MSB (Continuation bit Co) = 0, more than one byte */
                    /* may follow. Bit6, RS=0, next byte is command byte */

    FUNC_SET_BASIC, /* 0x24 -> Until here settings which are really related to the display  */
                    /* being used, also HW-aspect Change from extended instruction */
                    /* set to basic instruction set */

    DISPLAY_ON      /* 0x0C -> Display control: set display on, cursor off, no blink */
};


/* PARAMETERS FOR LCD DATA CONFIGURATION, FOR TOP LINE */
    char code LcdConfigForTop[] = {

    SEL_INST_REG,   /* 0x00 -> MSB (Continuation bit Co) = 0, more than one byte */
                    /* may follow. Bit6, RS=0, next byte is command byte */

    SET_DDRAM_L1 
  }; 

/* PARAMETERS FOR LCD DATA CONFIGURATION, FOR BOTTOM LINE */
    char code LcdConfigForBottom[] = {

    SEL_INST_REG,  /* CO = 0, RS = 0  last control byte */

    SET_DDRAM_L2
  }; 

uchar position_val_ascii[6]; /* -99999 */

/* Fill Lcd Buffer with one control byte and 17 spaces */
   char code FillDdramWithSpaces[] = { /* CO = 0, RS = 1  next byte is DATA byte */
  /* First Byte is Control Byte 0x40 data register select */ 
    SEL_DATA_REG, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
                  0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0
  }; 

/* Local variables ----------------------------------------------------------*/
   /* LCD  buffers for TOP and BOTTOM lines */
   uchar  data lcd_display_image_top[17];
   uchar  data lcd_display_image_bottom[17];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Routine initialized LCD display
  * @param  None
  * @retval None
  */
void lcd_init(void)	
{
   /* configure pcf2119 as 2x16 display and display is off */	
	SMbusWrite(PCF2119_ID, NULL, Pcf2119Reg,sizeof(Pcf2119Reg));

  /* configure as to receive display data */
	SMbusWrite(PCF2119_ID, NULL, LcdConfigForTop, sizeof(LcdConfigForTop));

//	erase_lcd_buffer();

	/* clear display, fill top line with spaces */
	SMbusWrite(PCF2119_ID, NULL, FillDdramWithSpaces, sizeof(FillDdramWithSpaces));

	SMbusWrite(PCF2119_ID, NULL, LcdConfigForBottom, sizeof(LcdConfigForBottom));

  /* clear display, fill bottom line with spaces */ 
	SMbusWrite(PCF2119_ID, NULL, FillDdramWithSpaces, sizeof(FillDdramWithSpaces));

  /* set display on */ 
	SMbusWrite(PCF2119_ID, NULL, SetLcdDisplayOn, sizeof(SetLcdDisplayOn));

}

/**
  * @brief  Convert integer to ascii for 'R' ROM compatible character.
  *         Result is place in result[] buffer.
  * @param  None
  * @retval None
  */
void integer_to_ascii(int u_val, uchar *result)
{
  int i, base,sign;
  uint16 div_val;

  i = 0;
  base = 10;                    /* we are dealing with decimal number */
  div_val = 10000;              /* maximum value for unsigned integer is 65537 */

  if ((sign = u_val) < 0)       /* record sign */
  {
    u_val = -u_val;             /* make u_val positive */
    result[i++] = MINUS_SIGN;   /* put minus sign in location result[0] */
  }

  do
  {
    result[i++] = ascii[u_val/div_val] | 0x80;  /* convert into R_CGROM code */

    u_val %= div_val;           /* (65537 / 10000) remainder is 5537 */
    div_val /= base;            /* 10,000 / 10 = 1000  */

  } while (div_val);
}

/******************************************************************************
  * @brief  Routine initialized LCD display
  *
  * @param1: const schar *pcStr is a pointer to the string to be display.
  *
  * @param1: uchar   ulX is the horizontal position to display the string,
  *                  specified in columns from the left edge of the display.
  *
  * @param1: uchar   ulY is the vertical position to display the string,
  *                  specified in eight scan line blocks from the top of the
  *                  display (that is, only 0 and 1  are valid).
  *
  * @param1: bit     EraseBuffer
  *
  * @retval: none.
  *
  * @Description : Displays a string on the LCD display.
  *
  *  Note(s):
  *  Ctrl|                     Data Bytes(16)                            |Null
  *  Byte|     Top          LCD Display buffer                           |Byte
  *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |11 |12 |13 |14 |15 |16 |17 |
  *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |11 |12 |13 |14 |15 |16 |17 |
  *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *          Bottom
  *****************************************************************************
  */

void  print_string (const schar *pcStr, uchar ucX, uchar ucY, bit EraseBuffer)
{
  uchar i;
  uchar ucXpos;  /* variable for char position */


  /* lcd display image top line buffer */
  if(ucY == 0)
  {
    if(EraseBuffer)
    {
      for( i = 0; i < LCD_BUF_SIZE; i++)
      {
        lcd_display_image_top[i] = 0xA0;  /* fill with space */
      }
    }
    // Move the display cursor to the requested position on the display.
    ucXpos = ucX + 1; /* get char position including plus 1 for Control Byte */

    // Loop while there are more characters in the string.
    while(*pcStr != 0)
    {
      // See if there is enough space on the display for this entire string.
      if(ucXpos <= LCD_BUF_SIZE)  /* 16 characters  plus One for Control Byte 0x40 */ 
      {
         // place the content of pointer into image buffer and increment pointer.
         lcd_display_image_top[ucXpos++] = *pcStr++ | 0x80;  
      }
    }
    /* send it to lcd display */
    lcd_display_image_top[0] = SEL_DATA_REG;  /* put control byte in 1st location */

    SMbusWrite(PCF2119_ID, NULL, LcdConfigForTop, sizeof(LcdConfigForTop));
    SMbusWrite(PCF2119_ID, NULL, lcd_display_image_top, sizeof(lcd_display_image_top));
  }

  else /* lcd display image bottom line buffer */
  {
    if(EraseBuffer)
    {
      for( i = 0; i < LCD_BUF_SIZE; i++)
      {
        lcd_display_image_bottom[i] = 0xA0;  /* fill with space */
      }
    }
    // Move the display cursor to the requested position on the display.
    ucXpos = ucX + 1; /* get char position including plus 1 for Control Byte */

    // Loop while there are more characters in the string.
    while(*pcStr != 0)
    {
      // See if there is enough space on the display for this entire string.
      if(ucXpos <= LCD_BUF_SIZE)  /* 16 characters  plus One for Control Byte 0x40 */ 
      {
         // place the content of pointer into image buffer and increment pointer.
         lcd_display_image_bottom[ucXpos++] = *pcStr++ | 0x80;  
      }
    }

    /* send it to lcd display */
    lcd_display_image_bottom[0] = SEL_DATA_REG;  /* put control byte in 1st location */

    SMbusWrite(PCF2119_ID, NULL, LcdConfigForBottom, sizeof(LcdConfigForBottom));
    SMbusWrite(PCF2119_ID, NULL, lcd_display_image_bottom, sizeof(lcd_display_image_bottom));
  }
}

/**
  *****************************************************************************
  * @brief  Routine initialized LCD display
  *
  * @param1: int val is a 16 bit signed value to be display on LCD.
  *
  * @param2: uchar   ulX is the horizontal position to display the string,
  *              specified in columns from the left edge of the display.
  *
  * @param3: uchar   ulY is the vertical position to display the string,
  *              specified top or bottom line (0 = top line; 1 bottom line).
  *
  * @param4: bit     EraseBuffer
  *
  * retval: none.
  *
  * @note: Displays integer on the LCD display.
  *   Ctrl|                     Data Bytes(16)                            |Null
  *   Byte|    Top           LCD Display buffer                           |Byte
  *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |11 |12 |13 |14 |15 |16 |17 |
  *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |11 |12 |13 |14 |15 |16 |17 |
  *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *           Bottom
  *****************************************************************************
  */

void  print_integer (int val, uchar ucX, uchar ucY, bit EraseBuffer)
{
  uchar ucXpos;  /* variable for char position */
  uchar display_value[6];
  uchar i;

  integer_to_ascii(val, display_value);

  /* lcd display image top line buffer */
  if(ucY == 0)
  {
    if( EraseBuffer)
	{
      for( i = 0; i < LCD_BUF_SIZE; i++)
      {
        lcd_display_image_top[i] = ERASE;  /* fill with space */
      }
    }
    // Move the display cursor to the requested position on the display.
    ucXpos = ucX + 1; /* get char position including plus 1 for Control Byte */

    // Loop while there are more characters in the string.
    for(i = 0; i < 5; i++)
    {
      // See if there is enough space on the display for this entire string.
      if(ucXpos <= LCD_BUF_SIZE)  /* 16 characters  plus One for Control Byte 0x40 */ 
      {
         // place the content of pointer into image buffer and increment pointer.
         lcd_display_image_top[ucXpos++] = display_value[i];  
      }
    }

    lcd_display_image_top[0] = SEL_DATA_REG;     /* put control byte in 1st location on top array */

    /* send it to lcd display */
    SMbusWrite(PCF2119_ID, NULL, LcdConfigForTop, sizeof(LcdConfigForTop));
    SMbusWrite(PCF2119_ID, NULL, lcd_display_image_top, sizeof(lcd_display_image_top));
  }

  else /* lcd display image bottom line buffer */
  {
    if(EraseBuffer)
	{
      for( i = 0; i < LCD_BUF_SIZE; i++)
      {
        lcd_display_image_bottom[i] = ERASE;  /* fill with space */
      }
    }
    // Move the display cursor to the requested position on the display.
    ucXpos = ucX + 1; /* get char position including plus 1 for Control Byte */

    // Loop while there are more characters in the string.
    for(i = 0; i < 5; i++)
    {
      // See if there is enough space on the display for this entire string.
      if(ucXpos <= LCD_BUF_SIZE)  /* 16 characters  plus One for Control Byte 0x40 */ 
      {
         // place the content of pointer into image buffer and increment pointer.
         lcd_display_image_bottom[ucXpos++] = display_value[i];  
      }
    }

    lcd_display_image_bottom[0] = SEL_DATA_REG;  /* put control byte in 1st location on bottom array */

  /* send it to lcd display */
    SMbusWrite(PCF2119_ID, NULL, LcdConfigForBottom, sizeof(LcdConfigForBottom));
    SMbusWrite(PCF2119_ID, NULL, lcd_display_image_bottom, sizeof(lcd_display_image_bottom));
  }
}


/******************************************************************************
  * @brief  Erase entire LCD
  * @param  None
  * @retval None
  * @note
  *
  *   Ctrl|                     Data Bytes(16)                            |Null
  *   Byte|    Top           LCD Display buffer                           |Byte
  *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |11 |12 |13 |14 |15 |16 |17 |
  *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |11 |12 |13 |14 |15 |16 |17 |
  *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *           Bottom
  *****************************************************************************
  */

void erase_lcd_buffer(void)
{
  uchar i;

  for( i = 0; i < 17; i++)
  {
    lcd_display_image_top[i] = ERASE;
    lcd_display_image_bottom[i] = ERASE;
  }

  /* send it to lcd display */
  lcd_display_image_top[0] = SEL_DATA_REG;     /* put control byte in 1st location */

  SMbusWrite(PCF2119_ID, NULL, LcdConfigForTop, sizeof(LcdConfigForTop));
  SMbusWrite(PCF2119_ID, NULL, lcd_display_image_top, sizeof(lcd_display_image_top));

  /* put control byte in 1st location on bottom array */
  lcd_display_image_bottom[0] = SEL_DATA_REG;

  /* send it to lcd display */
  SMbusWrite(PCF2119_ID, NULL, LcdConfigForBottom, sizeof(LcdConfigForBottom));
  SMbusWrite(PCF2119_ID, NULL, lcd_display_image_bottom, sizeof(lcd_display_image_bottom));
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
