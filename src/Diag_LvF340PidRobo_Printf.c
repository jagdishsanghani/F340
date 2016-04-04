/**
  ******************************************************************************
  * @file    Diag_F340_Printf.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Printf() module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of Printf():
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup PRINTF
  * @ PRINTF modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static char low_level_get(void); /* Read one char from I/O */

/**
  * @brief
  * @param  None
  * @retval None
  * @note   Use the putchar macro to write a single character to the display.
  *         The putchar macro returns the character written to stdout.
  */
schar putchar(uchar c)
{
	do{
	} while (!TxBuff0_Avail);   /* wait for Tx Buffer empty */

    TxBuff0_Avail = 0;          /* Flag that Tx Buffer is not empty */
    SBUF0 = c;                  /* send Tx data */
    return (c);
}


/**
  * @brief
  * @param  None
  * @retval received character
  * @note
  */
static char low_level_get(void)   /* Read one char from I/O */
{
	do {
	} while (!RxData0_Avail);
	RxData0_Avail = 0;             /* clear RxData0_Avail flag */
	EscFlag = 0;                   /* clear EscapeFlag */
	return (ReceiveData);          /* return data */
}

/**
  * @brief
  * @param  None
  * @retval return with valid character or nothing
  * @note
  *  The getche() function returns the next character read from the
  *  console and echoes that character to the screen.Characters from
  *	 space(20hex) to (7E) are echo to the screen.The getche() function
  *	 is not define by the ANSI C standard.
  */
schar getche(void) {
	char key;

	while (1) {
		key = low_level_get();
		if (key == 0x0D) {
			return (key);
		} else if (key == In_DELETE || key == In_BACKSPACE) {
			return (key);
		} else if (key == In_ESC) {
			EscFlag = 1;
			return (In_ESC);
		} else if (key >= ' ') {
			return (key);
		} else {
			putchar('\7');
		}
	}
	return (0);
}

/**
  *****************************************************************************
  * Function: int printf(const char *format, ...)
  * @param
  * @param
  * @retval  none
  * @note    Description:  Supports standard formats %c %s %d %x
  *
  *          %d and %x requires non-standard qualifiers,%dfn, %xfn:-
  *          f supplies a fill character
  *          n supplies a field width
  *
  *          Supports custom formats  %tf  %y  %a  %e  %b  %u
  *          %b	prints a 2 digit BCD value with leading zero
  *
  *          %u	prints the 16 bit unsigned integer in hex format
  *
  *****************************************************************************
  */
int printf(const char *format, ...)
{
  static const char code hex[] = "0123456789ABCDEF";
  uint16 code width_dec[5] = { 1, 10, 100, 1000, 10000 };
  uint16 code width_hex[4] = { 0x1, 0x10, 0x100, 0x1000 };

  char format_flag, fill_char;
  uint16 u_val, div_val, base;

  char *ptr;
  va_list ap;
  va_start(ap, format);

  for (;;)
  {
    while ((format_flag = *format++) != '%') /* until full format string read */
    {
      if (!format_flag)
      { /* until '%' or '\0' */
        return (0);
      }
      putchar(format_flag);
    }

    switch (format_flag = *format++)
    {
      case 'c':
        format_flag = va_arg(ap, int);

        default:
          putchar(format_flag);

          continue;

      case 't':
        format_flag = RtcBcdHours;                /* Read hours register in BCD */
        putchar( hex[ format_flag >> 4 ] );
        putchar( hex[ format_flag & 0x0F ] );
        putchar( ':' );
        format_flag = RtcBcdMins;                 /* Read minutes register in BCD */
        putchar( hex[ format_flag >> 4 ] );
        putchar( hex[ format_flag & 0x0F ] );
        putchar( ':' );
        format_flag = RtcBcdSecs;                 /* Read seconds register in BCD */
        putchar( hex[ format_flag >> 4 ] );
        putchar( hex[ format_flag & 0x0F ] );

          continue;

      case 'y':
        format_flag = RtcBcdDOM;                  /* Read date in BCD */
        putchar( hex[ format_flag >> 4 ] );
        putchar( hex[ format_flag & 0x0F ] );
        putchar( '/' );

        format_flag = RtcBcdMonth;                /* Read month in BCD */
        putchar( hex[ format_flag >> 4 ] );
        putchar( hex[ format_flag & 0x0F ] );
        putchar( '/' );

        format_flag = RtcBcdCentury;              /* Read Century in BCD */
        putchar( hex[ format_flag >> 4 ] );
        putchar( hex[ format_flag & 0x0F ] );

        format_flag = RtcBcdYear;                 /* Read Year in BCD */
        putchar( hex[ format_flag >> 4 ] );
        putchar( hex[ format_flag & 0x0F ] );

        continue;

      case 'b':
        format_flag = va_arg( ap, int );
        putchar( hex[ format_flag >> 4 ] );
        putchar( hex[ format_flag & 0x0F ] );

        continue;

      case 's':
        ptr = va_arg(ap, char *);
        format_flag = *ptr++;
        while(format_flag)
        {
          putchar(format_flag);
          format_flag = *ptr++;
        }
        continue;

      case 'u':
        base = 16;
        div_val = 0x1000;
        u_val = va_arg(ap, uint16);
        do
        {
          putchar(hex[u_val / div_val]);
          u_val %= div_val;
          div_val /= base;
        }while(div_val);

        continue;

      case 'd':
        base = 10;
        fill_char = *format++;
        format_flag = ( *format++ ) - '1';
        div_val = width_dec[format_flag];
        u_val = va_arg(ap,int);

        if(((int)u_val) < 0)
        {
          u_val = - u_val;        /* applied to unsigned type, result still unsigned */
          putchar('-');
        }

        goto CONVERSION_LOOP;

        case 'x':
          base = 16;
          fill_char = *format++;
          format_flag = (*format++) - '1';
          div_val = width_hex[format_flag];
          u_val = va_arg(ap, uint16);

          CONVERSION_LOOP:
          while(div_val > 1 && div_val > u_val)
          {
            div_val /= base;
            putchar(fill_char);
          }

          do
          {
            putchar(hex[u_val / div_val]);
            u_val %= div_val;
            div_val /= base;
          }while(div_val);
    }/* end of switch statement */
  }
  return (0);
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
