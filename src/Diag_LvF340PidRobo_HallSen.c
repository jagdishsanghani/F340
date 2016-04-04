/**
  ******************************************************************************
  * @file    Diag_F340_HallSen.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Hall sensor module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of ADC peripheral:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup HALL_SENSOR
  * @ HALLL SENSOR modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  HALL_NUM_OF_SAMPLES       4
#define  HALL_MUX          0x0C   /* AMXOP4-0  0000-1100 */
#define NOFIELD 505L    // Analog output with no applied field, calibrate this

#define MILLIGAUSS_IN_ONE_STEP 1953L  // For A1301: 2.5mV = 1Gauss, and 1024
                                      // analog steps = 5V, so 1 step = 1953mG
// #define TOMILLIGAUSS 3756L         // For A1302: 1.3mV = 1Gauss, and 1024
                                      // analog steps = 5V, so 1 step = 3756mG

/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Hall sensor values */
uint16  Hall_Raw;           /* Measured in bits */
uint16  Hall_mV;            /* Measured voltage in mV */
uint16  Hall_PV_Pct;        /* Hall sensor process value in percentage */

uint16  HallMaxValue;       /* the maximum value returned by this function  */

/* here we store calibrated values of hall sensor. min & max  */
uint16  HallCalMin;
uint16  HallCalMax;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */


/**
  * @brief
  * @param  None
  * @retval Raw Hall sensor value in binary
  * @note
  */
int16 ReadHallSensor (void)
{
  long32 HallAccum = 0;          /* Accumulator for averaging */
  uchar i;

  while (AD0BUSY) { };           /* wait for any current conversion to finish  */

  AD0INT = 0;                    /* Clear ADC conversion complete overflow */

  AMX0P = HALL_MUX;              /* ADC0 Hall Sensor input   */
                                 /* AMXOP  SFR 0xBB, Reset Value 0x00, HALL_MUX = 0x???      */
                                 /* Bit7   Bit6   Bit5   Bit4   Bit3    Bit2   Bit1   Bit0  */
                                 /*  -      -      -    AMXOP4 AMXOP3  AMXOP2 AMXOP1 AMXOP0 */
                                 /*  0      0      0      1      1       1       1      0   */

  for(i = 0; i < 25; i++) { };   /* wait 10uSec for mux to settle down */

  for(i = 0; i < HALL_NUM_OF_SAMPLES; i++)
  {
    AD0BUSY = 1 ;                /* start the conversion */

    while (!AD0INT){ };          /* wait for conversion interrupt flag */

    AD0INT = 0;                  /* Reset Flag */

    HallAccum += ADC0;           /* add into the conversion result */
  }

  HallAccum = HallAccum / HALL_NUM_OF_SAMPLES;

  Hall_Raw = (int16)HallAccum;   /* value in unsigned integer */

  return(Hall_Raw);
}


/**
  *****************************************************************************************
  * @brief
  * @param  None
  * @retval None
  * @note   A Hall effect device produces a voltage when placed in a magnetic field.
  *  Here is some simple code to use an A1301 or A1302 Hall effect sensor,
  *  which give output proportional to field strength. (Other Hall effect sensors
  *  may be simpler and just switch on or off when the field exceeds a certain value).
  *
  *  If there is no magnetic field applied the sensor outputs approximately half of the
  *  supply voltage, typically 2.5V, assuming the supply is 5V. The output drops towards
  *  0V or rises towards 5V, according to magnetic polarity, at a rate of 2.5mv / Gauss
  *  for the A1301, or 1.3mV / Gauss for the A1302.
  *
  *  These devices come in LH and UA packages; beware that the pinouts depend on the package
  *  (see sample code for details). No other components are needed for the example.
  *
  *  The device has a specific orientation; the magnet should be on the logo side of the device.
  *  (It works quite well on the other side too but the logo side is probably more accurate).
  *
  *  Note that these sensors are relatively inaccurate (their sensitivity to magnetic field can
  *  vary within 20% or so), so there is probably not much point in using a precise 5V voltage
  *  reference. On the other hand, you probably do expect a reading of zero when no magnet is
  *  near the device. So, before trying to measure any fields, you need to find the voltage
  *  output by the device when no magnet is near it, as this voltage varies slightly from
  *  device to device. This can be done by uncommenting the lines in the code which print the
  *  raw reading. For the particular sensor I used, this raw value was 505.
  *
  *  The strength of the field around a magnet depends on the distance in a complicated way
  *  according to the type of magnet - see http://en.wikipedia.org/wiki/Magnet to get started.
  *
  *
  *  HALL SENSOR PARAMETERS   ########
  *
  * #define NOFIELD 505L    // Analog output with no applied field, calibrate this
  *
  * // Uncomment one of the lines below according to device in use A1301 or A1302
  * // This is used to convert the analog voltage reading to milliGauss
  * #define MILLIGAUSS_IN_ONE_STEP 1953L  // For A1301: 2.5mV = 1Gauss, and
  *                                       // 1024 analog steps = 5V, so 1 step = 1953mG
  * // #define TOMILLIGAUSS 3756L         // For A1302: 1.3mV = 1Gauss, and 1024
  *                                       // analog steps = 5V, so 1 step = 3756mG
  *
  * Pin Layout LH Package     Pin1     Pin2    Pin3
  * =====================     VCC 5V  Signal   Gnd
  *
  * Pin Layout UA Package     Pin1     Pin2    Pin3
  * =====================     VCC 5V   Gnd    Signal
  * Display ADC reading in Hex, mV and Gauss
  *****************************************************************************************
  */
int16 HallSenMagField(int16 RawHallVal)
{
  int16 RawHall;
  int16 Gauss;

  long Compensated;

  /* measure magnetic field */
  RawHall = RawHallVal;                /* Range : 0..1023 = 2400mV */

  // Gauss South Pole = 0 - -985;  North Pole = 0 - 1013

  /* adjust relative to no applied field  */
  Compensated = RawHall - NOFIELD;
  /* adjust scale to Gauss, 1953 mG per bit */
  /* devide by 1000 for Gauss  */
  Gauss = (int16)((Compensated * MILLIGAUSS_IN_ONE_STEP) / 1000);

  return(Gauss);  
}


/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/

