/**
  ******************************************************************************
  * @file    Diag_LvF340PidRobo_Adc.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   ADC module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of ADC peripheral:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup ADC
  * @ ADC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* ADC */
#define  ADC_OFFSET         776   /* Temp. Sensor reading in millivolts at 0 deg C */
#define  ADC_SPAN          1023   /* ADC 10bit = 1023 */
#define  VREF_mV           2400   /* ADC Vref = 2.400V  */
#define  TEMP_GAIN          286   /* GAIN (V/ deg c) = 2.86 mV/deg C = (2.86 * 100)/ 100 */

#define  POT_MUX           0x0B   /* AMXOP4-0  0000-1011 */
#define  TEMP_MUX          0x1E   /* AMXOP4-0  0001-1110 */

#define  NUM_OF_SAMPLES       4

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uchar  AdcAvail;
uint16 AdcValue;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief
  * @param  None
  * @retval None
  * @note     : AMXOP:      Bit7   Bit6   Bit5   Bit4   Bit3    Bit2   Bit1   Bit0
  *           : SFR 0xBB     -      -      -    AMXOP4 AMXOP3  AMXOP2 AMXOP1 AMXOP0
  *           : Reset Value  0      0      0      0      0       0       0      0
  *           : Temp Sensor  0      0      0      1      1       1       1      0
  */
void InitADC0 (void)
{
   ADC0CN = 0x02;   /* ADC0 disabled, normal tracking, */
                    /* conversion triggered on TMR2 overflow */

//   REF0CN = 0x03;   /* Enable on-chip VREF and buffer */

   REF0CN = 0x07;   /* enable temp sensor, on-chip VREF, and VREF output buffer */
                    /* REF0CN Reference Control */
                    /* REFOCN SFR 0xD1, Reset Value 0x00                       */
                    /* Bit7   Bit6   Bit5   Bit4   Bit3    Bit2   Bit1   Bit0  */
                    /*  -      -      -      -     REFSL   TEMPE  BIASE  REFBE */
                    /*  0      0      0      0      0       1       1      1   */

   AMX0P = 0x1E;    /* ADC0 Temp Sensor positive input   */
                    /* AMXOP  SFR 0xBB, Reset Value 0x00, Temp Sensor = 0x1E   */
                    /* Bit7   Bit6   Bit5   Bit4   Bit3    Bit2   Bit1   Bit0  */
                    /*  -      -      -    AMXOP4 AMXOP3  AMXOP2 AMXOP1 AMXOP0 */
                    /*  0      0      0      1      1       1       1      0   */

   AMX0N = 0x1F;    /* ADC0 negative input = GND i.e., single ended mode */

   ADC0CF = ((SYSCLK/3000000)-1)<<3;   /* set SAR clock to 3MHz */

   ADC0CF |= 0x00;              /* right-justify results */

   EIE1 |= 0x00;                /* enable ADC0 conversion complete int 0x08. */

   AD0EN = 1;                   /* enable ADC0 */
}

/**
  * @brief  The ISR is called after each ADC conversion
  *         which is triggered by Timer2.
  * @param  None
  * @retval None
  */
INTERRUPT(ADC0_ISR, ADC0EOC_IRQn)
//void ADC0_ISR (void) interrupt 10
{
  AD0INT = 0;       /* Clear ADC0 conv. complete flag */
  AdcValue = ADC0;  /* Read adc value */
  AdcAvail = 1;     /* Set AdcAvail flag */
}

/**
  * @brief
  * @param  None
  * @retval None
  */
uint16 TempSensor (void)
{
  ulong32 TempAccumulator = 0;   /* Accumulator for averaging */
  uint16  Temp_mV;               /* Measured voltage in mV */
  uint16  TempInDegC;

  unsigned char i;

  while (AD0BUSY) { };           /* wait for any current conversion to finish  */

  AD0INT = 0;                    /* Clear ADC conversion complete overflow */

  AMX0P = TEMP_MUX;              /* ADC0 Temperature input   */
                                 /* AMXOP  SFR 0xBB, Reset Value 0x00, TEMP_MUX = 0x1E      */
                                 /* Bit7   Bit6   Bit5   Bit4   Bit3    Bit2   Bit1   Bit0  */
                                 /*  -      -      -    AMXOP4 AMXOP3  AMXOP2 AMXOP1 AMXOP0 */
                                 /*  0      0      0      1      1       1       1      0   */

  for(i = 0; i < 25; i++) { };   /* wait 10uSec for mux to settle down */

  for(i = 0; i < NUM_OF_SAMPLES; i++)
  {
    AD0BUSY = 1 ;                /* start the conversion */

    while (!AD0INT){ };          /* wait for conversion interrupt flag */

    AD0INT = 0;                  /* Reset Flag */

    TempAccumulator += ADC0;     /* add into the conversion result */
  }

  TempAccumulator = TempAccumulator / NUM_OF_SAMPLES;

    /* The 10-bit ADC value is averaged across 4  measurements.   */  
    /*                                                            */
    /*                           Vref (mV)                        */
    /*   measurement (mV) =   --------------- * result (bits)     */
    /*                       (2^10)-1 (bits)                      */
    /*                                                            */
    /*   TempInCenti = (( mVtemp - Offset) * 100 )/ ( GAIN);      */


  Temp_mV = (uint16) ((TempAccumulator * VREF_mV) / 1023 );

  TempInDegC = (( Temp_mV - ADC_OFFSET) * 100 )/ (TEMP_GAIN);

  return(TempInDegC);
}

/**
  * @brief
  * @param  None
  * @retval None
  */
uint16 ResistorPot(void)
{
  ulong32 PotAccumulator = 0;          /* Accumulator for averaging */
  uint16  PotmVolt;                    /* Measured voltage in mV */

  unsigned char i;

  while (AD0BUSY) { };                 /* wait for any current conversion to finish  */

  AD0INT = 0;                          /* Clear ADC conversion complete overflow */

  AMX0P = POT_MUX;                     /* ADC0 Pot input   */
                                       /* AMXOP  SFR 0xBB, Reset Value 0x00,    POT_MUX  = 0x0B   */
                                       /* Bit7   Bit6   Bit5   Bit4   Bit3    Bit2   Bit1   Bit0  */
                                       /*  -      -      -    AMXOP4 AMXOP3  AMXOP2 AMXOP1 AMXOP0 */
                                       /*  0      0      0      0      1       0       1      1   */

  for(i = 0; i < 25; i++) { };         /* wait 10uSec for mux to settle down */

  for(i = 0; i < NUM_OF_SAMPLES; i++)
  {
    AD0BUSY = 1 ;                      /* start the conversion */

    while (!AD0INT){ };                /* wait for conversion interrupt flag */

    AD0INT = 0;                        /* Reset Flag */     

    PotAccumulator += ADC0;            /* add into the conversion result */
  }

  PotAccumulator = PotAccumulator / NUM_OF_SAMPLES;
  
        /* The 10-bit ADC value is averaged across 16 measurements.   */  
        /*                                                            */
        /*                           Vref (mV)                        */
        /*   measurement (mV) =   --------------- * result (bits)     */
        /*                       (2^10)-1 (bits)                      */
        /*                                                            */

  PotmVolt = (uint16) ((PotAccumulator * VREF_mV) / 1023 );

  return(PotmVolt);
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
