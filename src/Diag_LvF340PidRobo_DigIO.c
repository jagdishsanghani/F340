/**
  ******************************************************************************
  * @file    Diag_F340_DigIO.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Digital IO module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of Digital IO:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup DigIO
  * @ DigIO driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Switch relay on/off
  * @param1  RelayNum
  * @param2  RelayOnOff
  * @retval  None
  * @note
  */

void WriteRelays(uchar RelayNum, bit RelayOnOff)
{
  uchar RelayNo;

  RelayNo = RelayNum;                      /* Get Port Number */

  switch (RelayNo)
  {
    case 1:
      if(RelayOnOff) {RELAY1 = ON;}    /* Set Relay1 (0x01) On into Port2 */
      else           {RELAY1 = OFF;}    /* Set Relay1 Off into Port2 */

      break;

    case 2:
      if(RelayOnOff) {RELAY2 = ON; }     /* Set Relay2 (0x02) On into Port2 */
      else           {RELAY2 = OFF; }    /* Set Relay2 Off into Port2 */

      break;

    default:
      break;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/



