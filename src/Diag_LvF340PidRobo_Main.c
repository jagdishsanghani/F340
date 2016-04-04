/**
  ******************************************************************************
  * @file    Diag_F340_Main.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Main module driver.
  *          Main Loop, Initialize system and starts application tasks
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup MAIN
  * @ MAIN modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main (void)
{
  /* LVF340PidRobo board library initialization:
     - Configure the Systick to generate an interrupt each 1 msec
	 - Global initialization
   */
  uchar state;                 /* variable for switch statement */

  /* Configure the system clock to 16 MHz */
  init_system();
  CarrRtnLineFeed();           /* carriage return and line feed */

  while(1)
  {
    PrintMainMenu();

    /* 1. Start Test: Wait For User inputs -------------------------------------*/
    state = GetAtoI();       /* get user choice from kb input */
    switch(state)
    {
      case 1:              /* Test LEDs */
        ClrScrRstCur();
        TickLedTest();
        break;

      case 2:              /* Test Push Buttons */
        ClrScrRstCur();
        SwitcheStatusTest();
        break;

      case 3:              /* Test Digital IO */
        ClrScrRstCur();
        DigIOTest();
        break;

      case 4:              /* Test Buzzer */
        ClrScrRstCur();
        SpeakerTest();
        break;

      case 5:              /* Test ADC */
        ClrScrRstCur();
        AdcTest();
        break;

      case 6:              /* Test Potentiometer */
        ClrScrRstCur();
        PotTest();
        break;

      case 7:              /* Test Hall Sensor */
        ClrScrRstCur();
        HallSensorTest();
        break;

      case 8:              /* Test Photo Conductive Cell */
        ClrScrRstCur();
        PhotocellTest();
        break;

      case 9:              /* Test DAC channels */
        ClrScrRstCur();
        DacAllTest();
        break;

      case 10:              /* Test Real Time Clock */
        ClrScrRstCur();
        RtcDS1338Test();
        break;

      case 11:              /* Test Fan DC motor */
        ClrScrRstCur();
        FanDCmotorTest();
        break;

      case 12:              /* Test Servo motors */
        ClrScrRstCur();
        ServoMotorsTest();
        break;

      case 13:              /* Test LCD */
        ClrScrRstCur();
        LcdTest();
        break;

      case 14:              /* Test LCD */
        ClrScrRstCur();
        EEpromTest();
        break;

      default:
        break;
    }
  }/* end of while loop */
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/

