/**
  ******************************************************************************
  * @file    Diag_F340_Utility.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Utility module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of various utilities:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup UTILITY
  * @ UTILITY modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

char code * code AskStartAddress = "Start Address = ";
char code * code AskEndAddress = "End Address = ";
char code * code AskWordValue = "Enter word value = ";
char code * code AskByteValue = "Enter byte value = ";
char code * code AskValue = "Enter value = ";

/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
char code ascii[10] = "0123456789";
/* Private variables ---------------------------------------------------------*/
uint16 data HexValue;             /* buffer for 4 hex nibbles */
uchar idata Hex8Buffer[4];
uchar InputBuffer[8];
uint16 data MemStartAddress;      /* buffer for start address */
uint16 data MemEndAddress;        /* buffer for end address */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
/**
  *****************************************************************************
  * @brief
  * @param  None
  * @retval The ascii_to_integer function returns the integer value as an
  *         int variable. The return value is undefined if it is to large
  *         to fit an int variable.
  * @note   The ascii_to_integer converts a argument string into an int value.
  *         The string is expected to be of the form:
  *         [sign][5 digits(-32768 to + 32767)]
  *****************************************************************************
  */
uint16 AsciiToUint(char *str)
{
  uchar count = 5;                       /* character count */

  uint16 result = 0;
  uchar ptr;
  char cp;

  /* now convert the ascii string into 16 bit integer */
  ptr =0;                              /* point to first character entered */
  cp = str[ptr];                       /* read character */

  /* now convert the ascii string into 16 bit integer */
  while(count != 0)
  {
    if((cp >= '0') && (cp <= '9'))
    result *= 10;                      /* multiply buy 10 */
    result += cp-'0';                  /* data - 30 */
    cp = str[++ptr];                   /* increment pointer */
    count--;                           /* decrement count */
  }
  return(result);
}


/**
  * @brief  Convert interger to ascii character and put in result[] buffer.
  * @param  int u_val
  * @param  uchar *result
  * @retval None
  * @note
  */
void UintToAscii(uint16 u_val, uchar *result)
{
  uchar i;
  uint16 base;
  uint16 div_val;

  i = 0;
  base = 10;                    /* we are dealing with decimal number */
  div_val = 10000;              /* maximum value for unsigned integer is 65537 */

  do
  {
    result[i++] = ascii[u_val/div_val];  /* convert into ASCII */

    u_val %= div_val;           /* (65537 / 10000) remainder is 5537 */
    div_val /= base;            /* 10,000 / 10 = 1000  */

  } while (div_val);
}

/**
  * @brief  Convert number to bit and print on hyper-terminal.
  * @param  None
  * @retval None
  * @note
  */
void HexToBit(uchar number)
{
  uchar j;
  uint16 swbit;
  uint16 num;
  uint16 mask;

  num = number;

  mask = 0x8000;                      /* 1000 0000 binary */

  for(j=0; j<16; j++)
  {
    swbit = (mask & num) ? 1 : 0;   /* bit 1 or 0 */

    printf("%d01 ", swbit); /* print bit */

    mask >>= 1;                     /* shift mask to right */
  }
}		

/**
  * @brief  Converts the ASCII character to binary
  * @param  ASCII character between 0 to F
  * @retval Binary value
  * @note
  */
uchar HexToBin(uchar val)
{
  if(val >= '0' && val <= '9')
  {			
    return( val-'0');
  }				
  else 
  {			
    return((val-'0')-('A'-'0'-10));
  }				
} 

/**
  * @brief  Converts a packed BCD byte to uchar
  * @param  Packed BCD byte
  * @retval unsigned char valuve of parameter
  * @note
  */
uchar BCDToInt(uchar BCD)
{
  return ((BCD & 0x0F) + ((BCD >> 4) * 10));
}


/*
*****************************************************************************************
 *	Function:	  uint16 GetHex(uchar digit)
 *
 *	Description:  Gets a 16bit hex value of up to 4 digits.
 *
 *	Parameters:   none
 *
 *	Returns:	  HexValue, 16bit
 *
*****************************************************************************************
*/
/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
uint16 GetHex(uchar digit)
{
  uint16 key;			/* input character */
  char c;

  uint16 pointer=0;		/* Pointer in buffer */
  uint16 count=0;		/* character count */

  HexValue = 0;			/* ASCII 4 bytes into integer */

  for(;;)
  {
    while(1)
    {
      c = getche();
      key = ToUpper(c);    /* get char from serial and if lower then convert to upper */

      if ( key == In_ESC )  /* if ESCAPE pressed, then exit */
      {
        return ( key );
      }

      else if ( key == In_CR )	/* CARRIAGE RETURN ? */
      {
        if(count==0)		/* any characters at all ? */
        {
          continue;		/* no, so get another character */
        }
						/* check for any character */
        break;			/* yes, exit from this loop */
      }

      else if ( key == In_DELETE || key == In_BACKSPACE ) /* delete or back space */
      {
        if(pointer==0)	/* any characters entered */
        {
          continue;	/* no, so get another character */
        }

        EraseChar();

        pointer--;			/* decrement pointer */
        count--;		/* decrement character count */
        continue;
      }

	/* character 0 to 9 and A to F? */

      else if (('0'<= key <='9')||('A'<= key <='F'))
      {
        if(pointer < digit)
        {
          InputBuffer[pointer] = key;	/* save character and increment pointer */
          pointer++;
          count++;					/* increment count */
          putchar(key);			/* echo character */
        }
        continue;						/* and get some more */
      }
      else
      {
        putchar ( '\7' );	/* ring the bell */
      }
    }/* end of while(1) loop */

    /* now convert 4 bytes into 16 bit integer address */

    pointer--;	/* point to last character entered */
    count--;	/* decrement character count */
    key = InputBuffer[pointer];	/* read character */

    HexValue = HexToBin( key);	/* convert to binary and save it */

    if(count==0)
    {
      break;		/* only character ? */
    }

    pointer--;		/* next character */
    count--;		/* decrement character count */

    key = InputBuffer[pointer];			/* read character */

    HexValue = (HexValue |( (HexToBin(key))<<4 ));	/* convert to binary and save it */

    if(count==0)
    {
      break;		/* any more character ?, no then exit  */
    }

    pointer--;			/* back to next character */
    count--;			/* decrement character count */

    key = InputBuffer[pointer];			/* read character */

    HexValue = (HexValue |( (HexToBin(key))<<8 ));	/* convert to binary and save it */

    if(count==0){ break; }	/* any more character ?, no then exit */

    pointer--;			/* one last character */

    key = InputBuffer[pointer];	/* read character */

    HexValue = (HexValue |( (HexToBin(key))<<12 ));	/* convert to binary and save it */

    return( HexValue);	/* return with 16bit value */
  }/* end of for(;;) loop */
  return(0);
}

/*
*****************************************************************************************
 *	Function:      uchar GetValue( uchar MaxDigit)
 *
 *
 *	Description:
 *
 *  Parameters:    MaxDigit
 *
 *  Returns:
 *
*****************************************************************************************
*/
/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
uchar GetValue(uchar MaxDigit)
{
  uchar val;

  printf("%s",AskByteValue);  /* Ask Byte Value */

  if(EscFlag)
  {
    return(0);      /* escape ? then return */
  }
  val = MaxDigit;

  GetHex(val);      /* get hex value */

  return(0);
}

/**
  * @brief  print out Uint16 into binary format
  * @param  number
  * @param
  * @retval
  * @note
  */
void WordToBit(uint16 number)
{
  uchar j, swbit;
  uint16 mask, num;

  num = number;

  mask = 0x0001;	/* 0000 0000 0000 0001 binary */

  for(j=0; j<6; j++)
  {
    swbit = (mask & num) ? 1 : 0; /* bit 1 or 0 */
    printf("%d01      ", swbit);	/* print bit */

    mask <<= 1;		 /*  shift mask to left */
  }
}

/**
  * @brief  The GetAtoI converts a argument string into an int value.
  *	        The string is expected to be of the form:
  *         [whitespace][sign][digits]
  * @param
  * @param
  * @retval  The atoi function returns the integer value as an
  *		     int variable. The return value is undefined if it
  *		     is to large to fit an int variable.
  * @note
  */
char GetAtoI(void)
{
  uchar StringBuffer[8];
  uint16 pointer=0;		/* Pointer in buffer */
  uint16 count=0;		/* character count */
  uint16 digit = 6;

  int result = 0;
  char cp;
  // int sign;

  for(;;)
  {
    for(;;)
    {
      cp = getche();	/* get input character */

      if(cp == In_ESC)	/* if ESCAPE pressed then exit */
      {
        return (cp);
      }
      else if (cp == In_CR)	/* CARRIAGE RETURN ? */
      {
        if(count==0)		/* any characters at all ? */
        {
          continue;		/* no, so get another character */
        }
        break;
      }
      else if(cp == In_DELETE || cp == In_BACKSPACE) /* delete or back space */
      {
        if(pointer==0)	/* any characters entered */
        {
          continue;	/* no, so get another character */
        }

        EraseChar();

        pointer--;			/* decrement pointer */
        count--;		/* decrement character count */
        continue;
      }

      else if (cp>= '0' && cp <='9')    /* is character between 0 to 9? */
      {
        if(pointer < digit)
        {
          StringBuffer[pointer] = cp;	/* save character and */
          pointer++;					/* increment pointer */
          count++;					/* increment count */
          putchar(cp);		/* echo character */

          continue;
        }
      }
      else
      {
        putchar(cp);			/* echo character */
        EraseChar();
        putchar('\7');	 		/* ring the bell */
        continue;
      }
    }
     /* now convert the ascii string into 16 bit integer */
    pointer =0;	/* point to first character entered */
    cp = StringBuffer[pointer];	/* read character */

    while(count != 0)
    {
      result *= 10;		/* multiply buy 10 */
      result += cp-'0';	/* data - 30 */
      cp = StringBuffer[++pointer];	/* increment pointer */
      count--;			/* decrement count */
    }
    return(result);
  }
}

/**
  * @brief  store addresses in StartAddress and End Address in Binary format.
  * @param  maximum digits 4
  * @param
  * @retval
  * @note
  */
char GetMemStartEndAddr(uchar digit)
{
  printf(AskStartAddress);        /* print 'Enter end address' */

  if(EscFlag)
  {
    return(0);                    /* escape ? then return */
  }

  MemStartAddress = GetHex(digit);

  CarrRtnLineFeed();

  printf(AskEndAddress);          /* print 'Enter end address' */
                                  /* ESC,'P',5,10,'Enter end address */
  if(EscFlag)
  {
    return(0);                    /* escape ? then return */
  }

  MemEndAddress = GetHex(digit);

  CarrRtnLineFeed();

  return(0);                      /* exit */
}

/**
  * @brief  Converts the ASCII character to BCD
  *			 Routine to convert the 2 ASCII digits to packed BCD
  * @param
  * @param  ASCII character between 0 to 9
  * @retval packed BCD
  * @note
  */
uchar Get2ASCIIToBCD(uchar MSB, uchar LSB)
{
  uchar  RTCData;

  RTCData = (( MSB & 0x0F) << 4);  /* ASCII 31->0x0011 0001 to 0x0001 0000   */
  RTCData = (( LSB & 0x0F) | RTCData);

  return(RTCData);
}

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note    Convert interger to ascii for 'R' ROM compatible character.
  *          Result is place in result[] buffer.
  */
void IntegerToAscii(int u_val, uchar *result)
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

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
void EraseChar (void)
{
  putchar(Out_BACKSPACE);
  putchar(Out_SPACE);
  putchar(Out_BACKSPACE);
}

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
void EraseCharWithUnderScore(void)
{
  putchar( Out_BACKSPACE );       /* erase character on the screen */
  putchar('_');                   /* and write '_' on the screen */
  putchar( Out_BACKSPACE );       /* erase character on the screen */
}

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
void EraseBackLash(void)
{
  putchar( Out_BACKSPACE );       /* erase character on the screen */
  putchar('/');                   /* and write ':' on the screen */
  putchar( Out_BACKSPACE );       /* erase character on the screen */
  putchar( Out_BACKSPACE );       /* erase character on the screen */
  putchar('_');                   /* and write '_' on the screen */
  putchar( Out_BACKSPACE );       /* erase character on the screen */
}

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
void EraseSemiColon(void)
{
  putchar( Out_BACKSPACE );       /* erase character on the screen */
  putchar(':');                   /* and write ':' on the screen */
  putchar( Out_BACKSPACE );       /* erase character on the screen */
  putchar( Out_BACKSPACE );       /* erase character on the screen */
  putchar('_');                   /* and write '_' on the screen */
  putchar( Out_BACKSPACE );       /* erase character on the screen */
}


/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
void EraseAndRingTheBell(void)
{
  putchar(Out_BACKSPACE);         /* back space */
  putchar('_');                   /* erase 1st char on the screen */
  putchar(Out_BACKSPACE);         /* back space */
  putchar('_');                   /* erase 2nd char on the screen */
  putchar(Out_BACKSPACE);         /* back space */
  putchar('\7');                  /* ring the bell */
}



/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
