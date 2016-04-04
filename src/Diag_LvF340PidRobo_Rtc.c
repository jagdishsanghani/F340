/**
  ******************************************************************************
  * @file    Diag_F340_Rtc.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Real Time Clock (RTC) module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of RTC peripheral:
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup RTC
  * @ RTC modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
   /* ###### I2C DS1338  REAL TIME CLOCK CHIP ######## */
/*
On the first application of power to the device the time and date registers
are reset to 01/01/00 01 00:00:00 (DD/MM/YY DOW HH:MM:SS).
The CH bit in the seconds register will be set to a 0.
*/

#define  DS1338_ID              0xD0  /* Device address for slave target */
                                      /* Note: This address is specified in the */
							                        /* MAXIM-IC  datasheet. */
#define  DS1338_TIME_START_ADDR 0x00

                                      /*********************************************/
                                      /* D7 |   6     | 5  |  4  |  3   2   1   0  */
#define  DS1338_SEC_REG_ADDR    0x00  /* CH |  10Sec  |    |     |    Seconds      */
#define  DS1338_MIN_REG_ADDR    0x01  /* 0  |  10 Min |    |     |    Minutes      */
#define  DS1338_HRS_REG_ADDR    0x02  /* 0  |  12/24* |20H | 10H |    Hour         */
#define  DS1338_DOW_REG_ADDR    0x03  /* 0  |   0     | 0  |  0  |  0   D   A   Y  */
#define  DS1338_DOM_REG_ADDR    0x04  /* 0  |   0     | 10DATE   |     DATE        */
#define  DS1338_MON_REG_ADDR    0x05  /* 0  |   0     | 0  | 10M |     MONTH       */
#define  DS1338_YR_REG_ADDR     0x06  /*        10YEAR           |     YEAR        */
#define  DS1338_CTL_REG_ADDR    0x07  /* OUT|   0     |OSF |SQWE |  0   0  RS1 RS0 */
                                      /*********************************************/
                                      /*          Square-Wave Output               */
                                      /*  OUT RS1  RS0  SQW OUTPUT  SQWE           */
                                      /*   X   0    0      1Hz        1            */
                                      /*   X   0    1    4.096kHz     1            */
                                      /*   X   1    0    8.192kHz     1            */
                                      /*   X   1    1    32.768kHz    1            */
                                      /*   0   X    X       0         0            */
                                      /*   1   X    X       1         0            */
                                      /*********************************************/

                                      /* D7  6   5   4    -   3   2   1   0   */
                                      /* OUT 0  OSF SQWE  -   0   0  RS1 RS0  */
                                      /*  0  0   0    1   -   0   0   1   1   */

#define  DS1338_EN_32768HZ      0x13  /* Enable OSC, 32768Hz on Pin7(SQW/OUT) */

#define  DS1338_RAM_START_ADDR  0x08  /* Start address of RAM */
#define  DS1338_RAM_END_ADDR    0x3F  /* End address of RAM   */
#define  DS1338_RAM_LEN         56    /* number of RAM bytes  */

#define  RTC_CENTURY            0x20
#define  TF                     0x40
#define  PM                     0x20

#define  NUM_BYTES_RD           7     /* RTC number of registers to be read  */
#define  NUM_BYTES_WR           7     /* RTC number of registers to be write */

#define  MAX_DIGIT	            4     /* Change if you need */
#define	 MAX_HEX		            8     /* Maximum hex nibbles */

/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bit RTCFlag;
bit OK;

/* BCD data from DS1338  */
uchar RtcBcdHours;
uchar RtcBcdMins;
uchar RtcBcdSecs;
uchar RtcBcdDOW;
uchar RtcBcdDOM;
uchar RtcBcdMonth;
uchar RtcBcdYear;
uchar RtcBcdCentury;

/* Temporary storage for Time &Date Value */
uchar data TimeBuffer[3];
uchar data DateBuffer[4];
uchar data RtcReadBuff[7];

 /* Local constants -----------------------------------------------------------*/
uchar code DS1338TimeBuffer[3] = {0x01, 0x59, 0x11};        /* HH:MM:SS */
uchar code DS1338DateBuffer[4] = {0x26, 0x03, 0x20, 0x16};  /* DD:MM:CC:YY */

char code * code Time = "\x1b[8;10HTime = ";
char code * code NewTime = "\x1b[10;10HNewTime = ";
char code * code Date = "\x1b[12;10HDate = ";
char code * code NewDate = "\x1b[14;10HNewDate = ";

uchar code DaysPerMonth[12] = { 0x31, 0x28, 0x31, 0x30, 0x31, 0x30,\
		                        0x31, 0x31, 0x30, 0x31, 0x30, 0x31};

char code * code AskTime = "\x1b[8;10H Time = ";
char code * code AskDate = "\x1b[9;10H Date = ";

/*** DS1338 RTC ***/
char code * code DS1338Menu[6]={"DS1338 RTC Test\n\r",
       "1. Change Time\n\r",
       "2. Change Date\n\r",
       "3. Display Time and Date\n\r",
       "Select option (1-3)",
       "\x1b[24;10HPress ESCAPE to exit\x1b[10;20H",
  };

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void ReadDS1338TimeAndDate(void);
void WriteDS1338TimeAndDate(void);

char DisplayDS1338TimeAndDate(void);

void SetDS1338Time(void);
void SetDS1338Date(void);

char ChangeDS1338Time(void);
char ChangeDS1338Date(void);

bit IsLeapYear(uchar Year, uchar Century);
uchar CalculateDay(uchar Date, uchar Month, uchar Century, uchar Year);

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
char DisplayDS1338TimeAndDate(void)
{
  clear_screen();                  /* clear VDU screen */

  while(1)
  {
    ReadDS1338TimeAndDate();

    printf("%s %t",Time);         /* send *Time ="\x1b[8;10HTime = " */

    printf("%s %y",Date);         /* send *Date="\x1b[12;10HDate = " */

    DisplayTimer = DELAY_200MSEC; /* delay for 200 mSec */

    while(DisplayTimer)           /* Check whether user wants to exit from this function */
    {
      if(RxData0_Avail)
      {
        if(ReceiveData==0x1b)     /* ESC ?  */
        {
          RxData0_Avail=0;        /* Yes, exit */
          return(0);
        }
      }
    }
  }
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
void SetDS1338Time(void)
{
  clear_screen();                       /* clear VDU screen */

  printf("%s", AskExit);               /* ask for key press or escape */

  printf("%s %t",Time);                /* send "Time ="\x1b[8;10HTime = "   */

  printf("%s__:__:__\x1b[8D",NewTime); /* send NewTime = __:__:__  */
                                       /* and reposition cursor \x1b[8D  */
  ChangeDS1338Time();

}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
void SetDS1338Date(void)
{
  clear_screen();	                      	/* clear VDU screen */

  printf("%s %y",Date);                   /* send   */

  printf("%s__/__/____\x1b[10D",NewDate); /* send "NewDate = __/__/____"  */

  ChangeDS1338Date();                     /* set date in DS1338 */

}


/**
  * @brief  ChangeTime.
  * @param  None
  * @retval None
  * @note    Century = 00-39,  Year = 00-99
  *          Month = 01-12,  Date = 01-31,  Day = 01-07
  *          Hour = 00-23,  Minutes = 00-59,  Seconds = 00-59
  */
char ChangeDS1338Time(void)
{
  char cp;                        /* input from keyboard */
  uchar RTCdata;                  /* buffer */
  uchar TimeFlag=0;               /* flag for time data is available */
  uchar count = 0;                /* input character counter */
  uchar State = 0;                /* state for hh=1, mm=2, ss=3 */

  uchar digit = 0;                /* indication for 1st and 2nd digit of hh, mm, ss */
  uchar MaxCount = 6;             /* maximum no. of digits */
  uchar AsciiLSB;                 /* temporary storage for lsb ascii input */
  uchar AsciiMSB;                 /* temporary storage for msb ascii input */

  while(1)
  {
    cp = getche();                /* get input character */

    if ( cp == In_ESC )           /* if ESCAPE pressed then exit */
    {
      return ( cp );
    }

    else if ( cp == In_CR )       /* CARRIAGE RETURN ? */
    {
      if(count==0)                /* any characters at all ? */
      {
        continue;                 /* no, so get another character */
      }

      if(TimeFlag==1)
      {                           /* update real time clock  */
        SMbusWrite(DS1338_ID, DS1338_TIME_START_ADDR, TimeBuffer, sizeof(TimeBuffer));  /* Read RTC */
        RTCFlag = 0;

        return(0);
      }
      else
      break;                      /* check for any character */
    }

    else if ( cp == In_DELETE || cp == In_BACKSPACE ) /* delete or back space */
    {
      if(count==0)                /* any characters entered */
      {
        continue;                 /* no, so get another character */
      }

      if(digit == 0)
      {
        EraseSemiColon();

        digit = 1;                /* now its point to 2nd digit */
        State--;                  /* point to one up state */
        count--;                  /* decrement character count */

        continue;
      }
      else
      {
        if(TimeFlag == 1)
        {
          EraseCharWithUnderScore();
          TimeFlag = 0;          /* changed mind so not ready */
          digit = 1;             /* point to 2nd digit */
          count--;               /* decrement count */

          continue;              /* goback and get another input */
        }

        EraseCharWithUnderScore();

        digit = 0;               /* now its point to 1st digit */
        count--;                 /* decrement character count */

        continue;
      }
    }
    else if (cp>= '0' && cp <='9')/* is character between 0 to 9? */
    {
      RTCdata = cp;

        /*********** check hours **********/
      if(count < MaxCount )
      {
        if( State == 0 )
        {
          if(digit==0)
          {
            putchar(RTCdata);     /* echo 1st character */
            AsciiMSB = RTCdata;   /* store 1st byte MSB  */
            count++;              /* increment character count,will be 1 */
            digit++;              /* increment digit, will be 1 */
            continue;
          }
          else
          {
            putchar(RTCdata);                     /* echo 2nd character */
            AsciiLSB = RTCdata;                   /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10);   /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);         /* add to nibble, integer */

            if(RTCdata <=23)                      /* check hour range ( 0-23) */
            {
              RTCdata = ((AsciiMSB & 0x0F) << 4);
              RTCdata = ((AsciiLSB & 0x0F) | RTCdata);
              TimeBuffer[2] = RTCdata;            /* store hh in time buffer */

              State++;                            /* increment to mm state */
              count++;                            /* increment character count,will be 2 */
              digit = 0;                          /* set digit to zero  */

              putchar(':');                       /* and write ':' on the screen */

              continue;                           /* continue for minutes */
            }
            else
            {
              EraseAndRingTheBell();

              continue;
            }
          }
        }

        /********** check minutes ***********/

        if( State == 1 )
        {
          if(digit==0)
          {
            putchar(RTCdata);               /* echo 1st character */
            AsciiMSB = RTCdata;             /* store 1st byte MSB  */
            count++;                        /* increment character count,will be 3 */
            digit++;                        /* increment digit, will be 1  */
            continue;
          }
          else
          {
            putchar(RTCdata);               /* echo 2nd character */
            AsciiLSB = RTCdata;             /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10);/* store nibble */
            RTCdata += (AsciiLSB & 0x0F);   /* add to nibble, integer */

            if(RTCdata <=59)                /* check minutes range ( 0-59) */
            {
              RTCdata = ((AsciiMSB & 0x0F) << 4);
              RTCdata = ((AsciiLSB & 0x0F) | RTCdata);
              TimeBuffer[1] = RTCdata;      /* store mm in time buffer */
              State++;                      /* increment to ss state */
              count++;                      /* increment character count,will be 4 */
              digit = 0;                    /* set digit to zero  */
              putchar(':');                 /* and write ':' on the screen */

              continue;                     /* continue for seconds */
            }
            else
            {
              EraseAndRingTheBell();

              continue;
            }
          }
        }

        /*********** check seconds ***************/
        if( State == 2 )
        {
          if(digit==0)
          {
            putchar(RTCdata);               /* echo 1st character */
            AsciiMSB = RTCdata;             /* store 1st byte MSB  */
            count++;                        /* increment character count,will be 5 */
            digit++;                        /* increment digit no.  */

            continue;
          }
          else
          {
            putchar(RTCdata);                    /* echo 2nd character */
            AsciiLSB = RTCdata;                  /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10);  /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);        /* add to nibble, integer */

            if(RTCdata <=59)                     /* check seconds range ( 0-59) */
            {
              RTCdata = ((AsciiMSB & 0x0F) << 4);
              RTCdata = ((AsciiLSB & 0x0F) | RTCdata);
              TimeBuffer[0] = RTCdata;           /* store ss in time buffer */

              TimeFlag = 1;                      /* Time data is ready for RTC */
              count++;                           /* increment character count,will be 6 */

              continue;                          /* go back and find the decision  */
            }
            else
            {
              EraseAndRingTheBell();

              continue;
            }
          }
        }

        continue;                 /* number of char is more than reqd */
                                  /* so go back to find the decision */
      }
          /*************************************************/
      else
      {
        putchar(cp);              /* echo character */
        EraseCharWithUnderScore();
        putchar('\7');            /* ring the bell */

        continue;
      }
    }
  }
  return(0);
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note   user input from kb
             Century = 00-39,  Year = 00-99
             Month = 01-12,  Date = 01-31,  Day = 01-07
             Hour = 00-23,  Minutes = 00-59,  Seconds = 00-59
             1st January 1900 is Monday
  */
char ChangeDS1338Date(void)
{
  char  cp;                            /* input from keyboard */
  uchar RTCdata;                       /* buffer */
  uchar DateFlag=0;                    /* flag for date data is available */
  uchar count = 0;                     /* input character counter */
  uchar State = 0;                     /* state for dd=1, mm=2, ccyy=3 */

  uchar digit = 0;                     /* indication for 1st, 2nd, 3rd and 4th digit */
                                       /* for dd, mm, yycc */

  uchar MaxCount = 8;                  /* maximum no. of digits */
  uchar AsciiLSB;                      /* temp storage for LSB ascii input */
  uchar AsciiMSB;                      /* temp storage for MSB ascii input */
  uchar DaysThisMonth;                 /* number of days for the given month */

    while(1)
    {
      cp = getche();                   /* get input character */

      if(cp == In_ESC)                 /* if ESCAPE pressed then exit */
      {
        return ( cp );
      }
      else if ( cp == In_CR )          /* CARRIAGE RETURN ? */
      {
        if(count==0)                   /* any characters at all ? */
        {
          continue;                    /* no, so get another character */
        }

        if(DateFlag==1)
        {
          /* Read RTC, update real time clock  */
          SMbusWrite(DS1338_ID, DS1338_DOW_REG_ADDR, DateBuffer, sizeof(DateBuffer));
          RTCFlag = 0;
          return(0);
        }
        else
        {
          break;                       /* check for any character */
        }
      }
      else if ( cp == In_DELETE || cp == In_BACKSPACE ) /* delete or back space */
      {
        if(count==0)                   /* any characters entered */
        {
          continue;                    /* no, so get another character */
        }

        if(State == 2)                 /* State 2 cointains 4 digits */
        {
          if(digit == 0)
          {
            EraseBackLash();

            digit = 1;                 /* now its point to 2nd digit of month */
            State--;                   /* point to month's state */
            count--;                   /* decrement character count */

            continue;
        }
        else
        {
          EraseCharWithUnderScore();

          digit--;                     /* point to 2nd digit */
          count--;                     /* decrement count */

          continue;                    /* goback and get another input */
        }
      }
      else
      {
        if(digit ==0)
        {
          EraseBackLash();

          State--;                     /* point to month's state */
          digit = 1;                   /* now its point to 2nd digit of month */
          count--;                     /* decrement character count */

          continue;
        }
        else
        {
          EraseCharWithUnderScore();

          digit = 0;                   /* point to 1st digit */
          count--;                     /* decrement count */

          continue;                    /* goback and get another input */
        }
      }
    }
    else if (cp>= '0' && cp <='9')     /* is character between 0 to 9? */
    {
      RTCdata = cp;

      /****** check date 1 - 31 ****/
      if(count < MaxCount)
      {
        if( State == 0 )
        {
          if(digit==0)
          {
            putchar(RTCdata);          /* echo 1st character */
            AsciiMSB = RTCdata;        /* store 1st byte MSB  */
            count++;                   /* increment character count,will be 1 */
            digit++;                   /* increment digit, will be 1 */

            continue;
          }
          else
          {
            putchar(RTCdata);                   /* echo 2nd character */
            AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

            if(RTCdata >= 1 && RTCdata <=31)    /* check date range ( 1-31) */
            {
              RTCdata = ((AsciiMSB & 0x0F) << 4);
              RTCdata = ((AsciiLSB & 0x0F) | RTCdata);
              DateBuffer[1] = RTCdata; /* store DOM in DateBuffer[0] */

              State++;                 /* increment to mm state */
              count++;                 /* increment character count,will be 2 */
              digit = 0;               /* set digit to zero  */
              putchar('/');            /* and write '/' on the screen */

              continue;                /* continue for minutes */
            }
            else
            {
              EraseAndRingTheBell();

              continue;
            }
          }
        }

        /************************* check month *************************/

        if( State == 1 )
        {
          if(digit==0)
          {
            putchar(RTCdata);          /* echo 1st character */
            AsciiMSB = RTCdata;        /* store 1st byte MSB  */
            count++;                   /* increment character count,will be 3 */
            digit++;                   /* increment digit, will be 1  */

            continue;
          }
          else
          {
            putchar(RTCdata);                   /* echo 2nd character */
            AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

            if(RTCdata >= 1 && RTCdata <=12)    /* check month range ( 1-12) */
            {
              RTCdata = ((AsciiMSB & 0x0F) << 4);
              RTCdata = ((AsciiLSB & 0x0F) | RTCdata);
              DateBuffer[2] = RTCdata;          /* store mm in time buffer */

              State++;                         /* increment to ss state */
              count++;                         /* increment character count,will be 4 */
              digit = 0;                       /* set digit to zero  */
              putchar('/');                    /* and write '/' on the screen */

              continue;                         /* continue for seconds */
            }
            else
            {
              EraseAndRingTheBell();

              continue;
            }
          }
        }

        /********** check year and century *********/
        if( State == 2 )
        {
          if(digit==0)
          {
            putchar(RTCdata);          /* echo 1st character */
            AsciiMSB = RTCdata;        /* store 1st byte MSB  */

            count++;                   /* increment character count,will be 5 */
            digit++;                   /* increment digit no.  */

            continue;
          }
          if(digit == 1)
          {
            putchar(RTCdata);                   /* echo 2nd character */
            AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

            if(RTCdata >= 19 && RTCdata <=39)   /* check century range (19-39) */
            {
              RTCdata = ((AsciiMSB & 0x0F) << 4);
              RTCdata = ((AsciiLSB & 0x0F) | RTCdata);
              RtcBcdCentury = RTCdata;         /* store in century buffer */
              count++;                         /* increment character count,will be 6 */
              digit++;                         /* increment digit number, will be 3  */

              continue;
            }
            else
            {
              EraseCharWithUnderScore();
              putchar('\7');          /* ring the bell */

              continue;
            }
          }

          /**********  check year  ************/
          if(digit == 2)
          {
            putchar(RTCdata);                   /* echo 1st character */
            AsciiMSB = RTCdata;                 /* store 1st byte MSB  */

            count++;                            /* increment character count,will be 7 */
            digit++;                            /* increment digit no.  */

            continue;
          }

          if(digit == 3)
          {
            putchar(RTCdata);                    /* echo 2nd character */
            AsciiLSB = RTCdata;                  /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10);  /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);        /* add to nibble, integer */

            if(RTCdata <=99)                     /* check year range (00-99) */
            {
              RTCdata = ((AsciiMSB & 0x0F) << 4);
              RTCdata = ((AsciiLSB & 0x0F) | RTCdata);
              DateBuffer[3] = RTCdata;           /* store in year buffer */

              /******* Check if the operator is sneakily to enter an invalid date *****/
              DaysThisMonth = DaysPerMonth[ BCDToInt(DateBuffer[1]) - 1 ];

              if( IsLeapYear( DateBuffer[3], DateBuffer[2] ) && (DateBuffer[1] == 2) )
              {
                ++DaysThisMonth;
              }

              if( DateBuffer[0] > DaysThisMonth )
              {
                DateBuffer[0] = DaysThisMonth;
              }

              if(IsLeapYear(DateBuffer[3], DateBuffer[2]) && (DateBuffer[1] == 2))
              {
                ++DaysThisMonth;
              }
              if(DateBuffer[0] > DaysThisMonth)
              {
                DateBuffer[0] = DaysThisMonth;
              }
              DateBuffer[0] = CalculateDay(DateBuffer[0], DateBuffer[1], DateBuffer[2], DateBuffer[3]);

              DateFlag = 1;            /* Time data is ready for RTC */
              count++;                 /* increment character count,will be 8 */
              digit++;                 /* increment digit number, will be 4  */

              continue;
            }
            else
            {
              EraseCharWithUnderScore();

              putchar('\7');           /* ring the bell */

              continue;
            }
          }
        }

        continue;                      /* number of char is more than reqd */
                                       /* so go back to find the decision */
      }

      else
      {
        putchar(cp);                   /* echo character */
        EraseChar();
        putchar('\7');                 /* ring the bell */

        continue;
      }
    }
  }
 return(0);
}

/**
  * @brief  Determines whether the supplied year is a leap year
  * @param  None
  * @retval Bit indicating if a leap year
  * @note
  */
bit IsLeapYear(uchar Year, uchar Century)
{
  /* If year not a multiple of 4 then not a leap year */
  if( (Year % 4) != 0 ) return NO;

  /* If year a multiple of 4 and non zero then is a leap year */
  if( Year != 0 ) return YES;

  /* Must check the century is a multiple of 4 also */
  return ((Century % 4) == 0);
}


/**
  * @brief  uchar CalculateDay(uchar Date, uchar Month, uchar Century, uchar Year)
  * @param:  Date, Month, Century, Year
  * @param:
  * @retval: Day
  * @note:   Calculates the day number (1 to 7) for the date specified
  *   Century = 00-39,  Year = 00-99
  *   Month = 01-12,    Date = 01-31,     Day = 01-07
  *   Hour = 00-23,     Minutes = 00-59,  Seconds = 00-59
  *   1st January 1900 is Monday
  *                             1      2      3      4      5      6      7
  *   char code days[7][3] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
  *                             0      1      2      3      4      5      6
  */
uchar CalculateDay(uchar Date, uchar Month, uchar Century, uchar Year)
{
  uint16 code CumulativeDays[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

  long	TotalDays = 0;
  uchar	Day;

  /* first calculate no. of years since 1900. */
  TotalDays = 100 * BCDToInt(Century) + BCDToInt(Year) - 1900;

  /* then multiply by 365 plus calculate no. of leap years to add extra days */
  /* (TotalDays -1)/4, minus 1 because we do not know the present year's  month */
  TotalDays = TotalDays * 365 + (TotalDays -1)/4;

  /* now calculate current year's no of days. last months plus present month no. of days */
  TotalDays += CumulativeDays[ BCDToInt( Month ) -1 ] + BCDToInt( Date ) - 1;

  /* check whether present year is leap year or not and we passed February or not */
  if( IsLeapYear( Year, Century ) && (Month > 2) )
  {
    ++TotalDays; /* if leap year and passed Fab then add 1 */
  }

  Day = (TotalDays % 7) + 1; /* divide by 7, remainder will give you the day of the month */

  return(Day);
}

/**
  * @brief  Read Time and Date
  * @param  None
  * @retval None
  * @note
  */
void ReadDS1338TimeAndDate(void)
{
    /* void ReadSMbus(uchar device_id, uchar* dest_addr, uchar src_addr, uchar len) */
    SMbusRead(DS1338_ID, RtcReadBuff, DS1338_TIME_START_ADDR, sizeof(RtcReadBuff));  /* Read RTC */ 

    RtcBcdSecs  = RtcReadBuff[0] & 0x7F;   /* CH=0 Clock is running, Seconds 00-59 */
    RtcBcdMins  = RtcReadBuff[1] & 0x7F;   /* Minutes 00-59 */
    RtcBcdHours = RtcReadBuff[2] & 0xFF;   /* Clock set for 24 hour mode, Hours 00-23 */
    RtcBcdDOW   = RtcReadBuff[3] & 0x07;   /* Day 1-7 */
    RtcBcdDOM   = RtcReadBuff[4] & 0x3F;   /* Date of Month 01-31 */
    RtcBcdMonth = RtcReadBuff[5] & 0x1F;   /* Month 01-12 */
    RtcBcdYear  = RtcReadBuff[6];          /* Year 00-99 */

    RtcBcdCentury = RTC_CENTURY;           /* Century 20 */
}

/**
  * @brief  Write Time and Date
  * @param  None
  * @retval None
  * @note
  */
void WriteDS1338TimeAndDate(void)
{
  SMbusWrite(DS1338_ID, DS1338_TIME_START_ADDR, DS1338TimeBuffer, sizeof(DS1338TimeBuffer));
  SMbusWrite(DS1338_ID, DS1338_DOM_REG_ADDR, DS1338DateBuffer, sizeof(DS1338DateBuffer));
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
void PrintDS1338Menu(void)
{
  uchar Count;

  ClrScrRstCur();

  for( Count = 0; Count<6; ++Count)
  {
    printf(DS1338Menu[Count]);
  }
}



/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/

