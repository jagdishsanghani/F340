/**
  ******************************************************************************
  * @file    Diag_F340_Photocell.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Photo cell module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of Photo cell interface with ADC peripheral:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup PHOTOCELL
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  PHOTOCELL_MUX     0x0D   /* AMXOP4-0  0000-1101 */
#define  PHOTOCELL_SAMPLES 4

#define  PHOTOCELL_MAX_VAL   1023   /*  */
#define  HALLSENSOR_MAX_VAL  1023   /*  */

// HALL SENSOR PARAMETERS   ########
#define NOFIELD 505L                  // Analog output with no applied field, calibrate this

// Uncomment one of the lines below according to device in use A1301 or A1302
// This is used to convert the analog voltage reading to milliGauss
#define TOMILLIGAUSS 1953L            // For A1301: 2.5mV = 1Gauss, and
                                      // 1024 analog steps = 5V, so 1 step = 1953mG
#define MILLIGAUSS_IN_ONE_STEP 1953L  // For A1301: 2.5mV = 1Gauss, and
                                      // 1024 analog steps = 5V, so 1 step = 1953mG

// #define TOMILLIGAUSS 3756L         // For A1302: 1.3mV = 1Gauss, and
                                      // 1024 analog steps = 5V, so 1 step = 3756mG


// PHOTOCELL PARAMETERS   ########
#define NOLIGHT 49L                    // Analog input with no applied field, calibrate this

// This is used to convert the analog voltage reading to Gauss
#define TOMILLILUX_IN_ONE_STEP    17L  // For VT90N1 2.5mV = 1LUX 1024 analog steps = 5V,
                                       // so 1 step = 19.53mLUX
#define MILLILUX_IN_ONE_STEP      17L  // For VT90N1 2.5mV = 1LUX 1024 analog steps = 5V,
                                       // so 1 step = 19.53mLUX

#define FOOTCANDLE_IN_ONE_STEP    16/100L  // For VT90N1 2.5mV = 1LUX 1024 analog steps = 5V,
                                           // so 1 step = 19.53mLUX

/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16  Photocell_Raw;      /* Measured in bits */
uint16  Photocell_mV;       /* Measured voltage in mV */
uint16  Photocell_PV_Pct;   /* Photo Conductive Cell sensor value in percentage */

uint16  PhotocellMaxValue;  /* the maximum value returned by this function  */

/* here we store calibrated values of photocell sensor. min & max  */
uint16  CalibratedPhotocellMin;
uint16  CalibratedPhotocellMax;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief
  * @param  None
  * @retval Photocell value
  * @note
  *   #############################################################
  *   #                          Vref (mV) * result (bits)        #
  *   #  PhotoCondRes_mV  =   ------------------------------      #
  *   #                              (2^10)-1 (bits)              #
  *   #                                                           #
  *   #  PhotoCondRes_PV%  =  (PhotocellAccum * 10000) / ADC_SPAN #
  *   #############################################################
  */
uint16 ReadPhotocell (void)
{
  ulong32 PhotocellAccum = 0;    /* Accumulator for averaging */
  uchar i;

  while (AD0BUSY) { };           /* wait for any current conversion to finish  */

  AD0INT = 0;                    /* Clear ADC conversion complete overflow */

  AMX0P = PHOTOCELL_MUX;         /* ADC0 Photocell input   */
                                 /* AMXOP  SFR 0xBB, Reset Value 0x00, PHOTOCELL_MUX = 0x?? */
                                 /* Bit7   Bit6   Bit5   Bit4   Bit3    Bit2   Bit1   Bit0  */
                                 /*  -      -      -    AMXOP4 AMXOP3  AMXOP2 AMXOP1 AMXOP0 */
                                 /*  0      0      0      1      1       1       1      0   */

  for(i = 0; i < 25; i++) { };   /* wait 10uSec for mux to settle down */

  for(i = 0; i < PHOTOCELL_SAMPLES; i++)
  {
    AD0BUSY = 1 ;                /* start the conversion */

    while (!AD0INT){ };          /* wait for conversion interrupt flag */

    AD0INT = 0;                  /* Reset Flag */

    PhotocellAccum += ADC0;     /* add into the conversion result */
  }

  PhotocellAccum = PhotocellAccum / PHOTOCELL_SAMPLES;

  Photocell_Raw = (uint16) PhotocellAccum;       /* value in unsigned integer */

  if(Photocell_Raw > 1023)
  {
    Photocell_Raw = 1023;
  }

  return(Photocell_Raw);
}

/**
  * @brief
  * @param  None
  * @retval millilux value
  * @note
  */
uint16 milliLuxPhotocell(void)
{
  uint16 RawLux;
  uint16 milliLux;

  long PhotoCompensated;

  /* measure magnetic field */
  RawLux = ReadPhotocell();   /* Range : 0..1023 = 2400mV */

  /* adjust relative to no applied field  */
  PhotoCompensated = RawLux - NOLIGHT;

  /* adjust scale to milliLux, 19.53 milliLux/bit */
  milliLux = (uint16) ((PhotoCompensated * TOMILLILUX_IN_ONE_STEP) / 1000);

  return(milliLux);  
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  *
  *  Calibration:
  *  Demonstrates one technique for calibrating sensor input.  The
  *  sensor readings during the first five seconds of the sketch
  *  execution define the minimum and maximum of expected values
  *  attached to the sensor pin.
  *
  *  The sensor minimum and maximum initial values may seem backwards.
  *  Initially, you set the minimum high and listen for anything
  *  lower, saving it as the new minimum. Likewise, you set the
  *  maximum low and listen for anything higher as the new maximum.
  *****************************************************************************
  */

void CalibrateMinMaxPhotocellValues(void)
{
  uint16 PhotocellValue;

  uint16 MaxPhotocellValue;  /* storage for max & min sensor values */
  uint16 MinPhotocellValue;

  ResetPhotocellCalVal();
  CalibratedPhotocellMin = PHOTOCELL_MAX_VAL;   /* stores max value 1023 */
  CalibratedPhotocellMax = 0;                   /* reset buffer */

  DelayTimer = DELAY_5000MSEC;

  // calibrate during the first five seconds 
  while (DelayTimer)
  {
    PhotocellValue = ReadPhotocell();

    // set the max we found THIS time
    if (MaxPhotocellValue < PhotocellValue)
    {
      MaxPhotocellValue = PhotocellValue;
    }

    // set the min we found THIS time
    if (MinPhotocellValue > PhotocellValue)
    {
      MinPhotocellValue = PhotocellValue;
    }
  }

  // record the min and max calibration values
  if(MinPhotocellValue > CalibratedPhotocellMax)
  {
    CalibratedPhotocellMax = MinPhotocellValue;
  }

  if(MaxPhotocellValue < CalibratedPhotocellMin)
  {
    CalibratedPhotocellMin = MaxPhotocellValue;
  }
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note   Resets the calibration.
  */
void ResetPhotocellCalVal(void)
{
  CalibratedPhotocellMin = PHOTOCELL_MAX_VAL;   /* stores max value 1023 */
  CalibratedPhotocellMax = 0;   
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
