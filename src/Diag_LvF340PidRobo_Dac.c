/**
  ******************************************************************************
  * @file    Diag_LvF340PidRobo_Dac.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Digital-to-Analog Converter (DAC) peripheral:
  *           + DAC channels configuration
  *           + Interrupts and flags management
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup F340_PID_ROBO
  * @{
  */
/** @defgroup DAC
  * @brief DAC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DAC_DELAY_CNT  0x0200
#define LOW_BYTE_MASK  0x00FF

#define MIN_DAC_VAL    0x0000
#define MAX_DAC_VAL    0x1000      /* 12 bit, 0-4095 */

#define MAX_DAC_NUM    4

/* Private macro -------------------------------------------------------------*/
/* Constants -----------------------------------------------------------------*/
uint16 code DAC_NUM[4]={0x0000,0x4000,0x8000,0xC000};

/**** DAC7614 *****/
char code * code Dac7614Menu[6]={"   DAC7614 Test (12 bits)\n\r\n",
       "1. Single Channel Test\n\r",
       "2. All Channels Test\n\r",
       "3. All Channels Auto Test\n\r\n",
       "Select option (1-3)",
       "\x1b[24;10HPress ESCAPE to exit\x1b[7;20H",
};

uint16 code DacVal[5]= {0x0000, 0x0400, 0x0800, 0x0C00, 0x0FFF};

/* Private variables ---------------------------------------------------------*/
uchar DAC_BYTES[2];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
void PrintDac7614Menu (void)
{
  uchar Count;

  ClrScrRstCur();
  for( Count = 0; Count<6; ++Count)
  {
    printf(Dac7614Menu[Count]);
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar DacAllTest(void)
{
  uint16 Idx;
  uint16 DacData;
  Idx = 0;
  ClrScrRstCur();
  PrintEscape();
  PrintDac7614Menu ();

  while(1)
  {
    DacData = DacVal[Idx++];
    DAC_BYTES[0] = DacData >> 8; /* transfer MSB */
    DAC_BYTES[1] = DacData;      /* transfer LSB */

    printf( "%s%x04", DacDataLabel,DacData );
    SpiBusWrite(DAC7614_CH0, NULL, DAC_BYTES, NULL);
    SpiBusWrite(DAC7614_CH1, NULL, DAC_BYTES, NULL);
    SpiBusWrite(DAC7614_CH2, NULL, DAC_BYTES, NULL);
    SpiBusWrite(DAC7614_CH3, NULL, DAC_BYTES, NULL);

    if(Idx == 5) { Idx = 0;}

    DelayTimer = 5000;       /* 5 second */
    while(DelayTimer)
    {
      if(RxData0_Avail)
      {
        if(ReceiveData==0x1b)  /* ESC ? */
        {
          RxData0_Avail=0;     /* Yes, exit */
          return(0);
        }
      }
    }
  }
}


/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
