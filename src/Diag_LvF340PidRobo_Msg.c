/**
  ******************************************************************************
  * @file    Diag_F340_Msg.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Main diagnostic message module driver.
  *          This file provides message functions for various modules.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup MSG
  * @ MSG modules
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
char code * code PressEscape = "\x1b[24;10H Press ESCAPE to exit";
char code * code PressBarEscape = "\x1b[24;10H Press SpaceBar to continue or ESCAPE to exit\x1b\x50\x27\x0a";

char code * code AskExit = "\x1b[24;10H Press ESCAPE to exit or any key to run test again";
char code * code AskMore = "\x1b[24;10H Press ESCAPE to run test again or any key for more";
char code * code ProgUndDev = "\x1b[10;10H Program under development";

char code * code MainMenu[18] = {"\t\t LvF340PID_Robo board  \n\r",
		"Diagnostic Software Ver 1.0  10-March-2016  J S\n\n\r",
		"\t\tMain Menu\n\r",
		"  1.  LEDs Test\n\r",
		"  2.  Push Buttons Test\n\r",
		"  3.  DigIO Test\n\r",
    "  4.  Speaker Test\n\r",
		"  5.  ADC Test\n\r",
		"  6.  Pot Test\n\r",
		"  7.  A1301 Hall Sensor Test\n\r",
		"  8.  VT90 Photocell Test\n\r",
		"  9.  Dac Test\n\r",
		" 10.  Rtc Test\n\r",
		" 11.  FanDCmotor Test\n\r",
		" 12.  ServoMotors Test\n\r",
		" 13.  LCD Test\n\r",
    " 14.  EEprom Test\n\r\n",

		"\tSelect option (1-14) and press ENTER  "
};

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief
 * @param  None
 * @retval None
 */
void PrintMainMenu(void)
{
  uchar count;

  ClrScrRstCur();
  for( count = 0; count<18; ++count)
  {
    printf(MainMenu[count]);
  }
}

/**
 * @brief
 * @param  None
 * @retval None
 */
void PrintEscape(void)
{
  printf(PressEscape);  /* "\x1b[24;10H Press ESCAPE to exit"; */
}

/**
 * @brief
 * @param  None
 * @retval None
 */
void PrintProgUnderDev(void)
{
  printf(ProgUndDev);  /* "\x1b[24;10H Press ESCAPE to exit"; */
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
