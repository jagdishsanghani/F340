/**
  ******************************************************************************
  * @file    Diag_LvF340PidRobo_24LC1025.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   This file provides all the EEPROM firmware functions.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup EEPROM
  * @ EEprom 24LC1025 driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constant-----------------------------------------------------------*/
/*** 24LC1025 EEprom ***/
char code * code Eep24LC1025Menu[7]={"\t\t24LC1025 EEprom Test\r\n",
    "  1. Display EEprom contents\n\r",
    "  2. Write EEprom\n\r",
    "  3. Write incremental Value to EEprom\n\r",
    "  4. Erase EEprom Chip\n\r",
    "\tSelect option (1-4)",
    "\x1b[24;10HPress ESCAPE to exit\x1b[6;28H",
};

/* Variables for SMbus -------------------------------------------------------*/
/* EEP 24LC1025 */
uchar EepBufferWrite[129];          /* eeprom's write data buffer */
uchar EepBufferRead[129];           /* eeprom's read data buffer */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void FillBuffer(uchar PageNumber);
void Print24LC1025Menu(void);


/**
  * @brief
  * @param  None
  * @retval None
  */
uchar Write24LC1025Eeprom(void)
{
  uchar count;                         /* character counter */
  uchar line;                          /* line counter */

  uchar Finish;                        /* finished flag */
  //bit eep_busy;                      /* eep write in progress flag */
  uchar idx;                           /* index for buffers */
  uchar PageIdx;

  uint16  StartAddress;                /* start address */
  uint16  EndAddress;                  /* end address, last address =  0xFFFF */

  StartAddress = 0x0000;               /* transfer start address */
  EndAddress = 0x007F;                 /* transfer end address */

  PageIdx = 1;
  FillBuffer(PageIdx);                 /* fill EepBuffer incremetally */

    /* now temp buffer is full so write to eeprom */
  SMbusWrite(EEP24LC1025_ID, StartAddress, EepBufferWrite, 0x7F);

  DisplayTimer = 100;
  while(DisplayTimer){/* wait */};

  StartAddress = 0x0000;               /* reset eep address */

  SMbusRead(EEP24LC1025_ID, EepBufferRead, StartAddress, 0x7F);

  /* Step1 display eep content */
  count = 0;                           /* clear uchar count */
  line = 0;                            /* clear line count */
  idx = 0;                             /* set EepBufferRead[] index to zero */
   
  CarrRtnLineFeed();                   /* start a new line */

  while(1)
  { /* step_a */
    if(StartAddress == EndAddress)     /* check for finished */
    {
      printf("%x02 ", (uint16)EepBufferRead[idx]);

      CarrRtnLineFeed();               /* start a new line */
      printf(AskExit);                 /* ask for key press or escape */

      break;	                         /* no so run test again */
    }

    /* step_c */
    if(count==0)                       /* start of a line */
    {
      printf("%p ", StartAddress);     /* print address and 1 space */
    }

    /* step_d */

  /* print uchar,1 space and point to next value */
    printf("%x02 ", (uint16)EepBufferRead[idx]);

    idx++;
    count++;                           /* increment uchar counter */
    StartAddress++;                    /* increment address */

    if(count != 16)                    /* end of line ? */
    {
      continue;                        /* no, go back to step_a */
    }
    count = 0;                         /* yes, so set zero to uchar counter */
    line++;                            /* increment line counter */

    if(line == 8 )                     /* end of 128 bytes (page length) but not finish */
    {
      line = 0;                        /* yes zero line counter */
      Finish = 0;                      /* clear flag Finish */
      idx = 0;                         /* reset idx to zero */
      EepBufferRead[0] = PageIdx++;    /* write PageIdx number */

      CarrRtnLineFeed();               /* start a new line */

      if(StartAddress == EndAddress)
      {
        Finish = 1;                    /* set Finish to indicate finished */
        printf(AskExit);	           /* ask for exit, to run test agian */
      }
      else
      {
        printf(AskMore);               /* any key for more */
      }
    }
    CarrRtnLineFeed();                 /* if not finished then do some more */

    if(EscFlag)                        /* escape ? */ 
    {
      EscFlag = 0;
      SMB_EEPROM = 0;

      return(0);                       /* exit to menu */
    }
    continue;                          /* show some more, go to step_a */
  }
  SMB_EEPROM = 0;
  return (0);                          /* exit to menu */
}

/**
  * @brief
  * @param  None
  * @retval None
  */
uchar Read24LC1025Eeprom(void)
{
  uchar  digit;                        /* number of digit */
  uchar  count;                        /* character counter */
  uchar  line;                         /* line counter */
  uchar  ReadData;                     /* char */
  uint16 StartAddress;                 /* start address */
  uint16 EndAddress;                   /* end address */
  bit    Finish;                       /* finished flag */

  uchar idx = 0;                       /* index for EepBufferRead  */ 

  /* Ram1 */
  for(;;)
  {
    ClrScrRstCur();                    /* clear VDU screen */

//    printf("%s",Eep24LC1025Menu[1]);	/* ShowChoice Display eeprom locations */

    digit = MAX_DIGIT;                 /* maximum 4 characters */

    GetMemStartEndAddr(digit);         /* get start and end addresses */

    StartAddress = MemStartAddress;    /* transfer start address */
    EndAddress = MemEndAddress;        /* transfer end address */

    CarrRtnLineFeed();                    /* start a new line CR\LF */
    CarrRtnLineFeed();                     /* start a new line CR\LF */

    printf("%x04  ",StartAddress);
    printf("%x04  ",EndAddress);

    if(EscFlag)                        /* pressed escape ? */
    {
      return(0);
    }

    /* ram1a */
    count = 0;                         /* clear byte count */
    line = 0;                          /* clear line count */
    putchar('\r');                     /* start a new line CR\LF */
    putchar('\n');                     /* again CR\LF */

    SMbusRead(EEP24LC1025_ID, EepBufferRead, StartAddress, 0x7F);

    /* ram1b */
    for(;;)
    {
      if(StartAddress == EndAddress)   /* chech for finished */		
      {
        ReadData = EepBufferRead[idx];  /* read and display last  byte */
        printf("%x02  ",(uint16) ReadData);

        CarrRtnLineFeed();                 /* 2 more CR\LF for message */
        CarrRtnLineFeed();

        printf("%s", AskExit);         /* ask for key press or escape */
        getche();

        if(EscFlag)                    /* escape ? */ 
        {/* ram1c */
          return(0);                   /* exit to menu */
        }
        break;	                       /* go to ram1 */ /* no so run test again */
      }

      /* ram1d */
      if(count==0)                     /* start of a line */           
      {
        printf("%x04  ", StartAddress); /* print address and 2 spaces */
      }

      /* ram1e */
      ReadData = EepBufferRead[idx++];  /* read a byte */
      StartAddress++;                  /* increment address pointer */		
      printf("%x02  ", (uint16) ReadData);      /* print byte and value 2 spaces */

      count++;                         /* increment byte counter */

      if(count != 16)                  /* end of line ? */
      {
        continue;                      /* no, jump to ram1b */
      }

      count = 0;                       /* yes, so zero byte counter */
      line++;                          /* increment line counter */

      if(line == 16 )                  /* end of 256 bytes but not finish */
      {
        line = 0;                      /* yes zero line counter */
        Finish = 0;                    /* clear flag Finish */		

        CarrRtnLineFeed();                 /* 2 more CR\LF for message */
        CarrRtnLineFeed();

        if(StartAddress == EndAddress)
        {
          Finish = 1;                  /* set Finish to indicate finished */
          printf("%s",AskExit);        /* ask for exit, to run test agian */
        }

        else
        {
          printf("%s", AskMore);       /* ram1f,  any key for more */
        }

        /* ram1g */
        getche();                      /* get character */

        if(EscFlag)
        {/* if escape flag */
          EscFlag = 0;                 /* clear escape flag */
					
          /* ram1i */
          if(!Finish)                  /* if not finished then restart test */
          {                            /* otherwise exit from test */				
            break;
          }

          else 
          {
            return(0);
          }
        }
      }

      CarrRtnLineFeed();               /* if not finished then do some more */
                                       /* start a new line */
      continue;                        /* ram1b  show some more */
    }
    continue; /* go to ram1 */
  }
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void FillBuffer(uchar PageNumber)
{
uchar idx;     /* index for buffer address */
uchar zdx;     /* incremental number */
/* fill buffer incrementally */
  for (idx=0,zdx=0; idx<128; idx++)
  {
//   EepBufferRead[idx++] = zdx++;    /* write incremental number and increment address */
//   EepBufferRead[idx] = PageNumber; /* write page number */

    EepBufferWrite[idx] = zdx++;    /* write incremental number and increment address */
  }
  EepBufferRead[128] = 0xFF;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
uchar Erase24LC1025Eeprom(void)
{
  return(0);
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void Print24LC1025Menu(void)
{
  uchar Count;

  ClrScrRstCur();

  for( Count = 0; Count<7; ++Count)
  {
    printf(Eep24LC1025Menu[Count]);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
