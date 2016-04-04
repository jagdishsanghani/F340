/**
  ******************************************************************************
  * @file    Diag_F340_Switches.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Switches module driver.
  *          This file provides set of firmware functions to manage switches
  *          available on LvF340PidRobo kit.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup SWITCHES
  * @ SWITCHES modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *****************************************************************************
  * @brief
  * @param  None
  * @retval None
  * @note
  *  Returns the value of any specified switches that are currently pressed, or 0.
  *  For example, if the PB14_ROBOT_START and PB14_ROBOT_STOP switches are both
  *  pressed, this method will return (PB14_ROBOT_START | PB14_ROBOT_STOP).
  *  The argument 'switches' can refer to multiple switches using the bitwise
  *  OR operator |.wait for any of the specified switches to be pressed, at which
  *  point execution will return from this method.
  *  The argument 'switches' can refer to multiple switchess using the bitwise OR
  *  operator |.The return value is the ID of the switch that was pressed.
  *	 Note that this method takes care of switch debouncing.
  *****************************************************************************
  */

uchar SwitchIsPressed(uchar Switches)
{
  uchar sw;

  sw = P1 & Switches;

  return (sw);  /* return the pressed switches(s) */
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note     Wait for any of the specified switchess to be released, at which point
  *              execution will return from this method.The argument 'switchess' can refer
  *            to multiple switchess using the bitwise OR operator |.  The return value
  *            is the ID of the switch that was released.  Note that this method takes
  *            care of switch debouncing.
  *            Example: wait_for_switch_release(PB14_ROBOT_START | PB14_ROBOT_STOP);
  */

uchar WaitForSwitchRelease(uchar Switches)
{
  do
  {
    while ((P1 & 0x03) & Switches)
    {
      /* wait for switch to be released */
    }
    msecDelay(DELAY_10MSEC);              /* debounce the switch press  */
  }
  while((P1 & 0x03) & Switches);	      /* if switch isn't still pressed, loop */

  return ((P1 & 0x03) | Switches);        /* return the pressed switch(s) */
}


/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/

