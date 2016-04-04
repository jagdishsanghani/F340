/**
  ******************************************************************************
  * @file    Diag_F340_ServoMotors.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   ServoMotors module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of ServoMotors peripheral:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup SERVO_MOTORS
  * @ SERVO MOTORS modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
         /* ################## PCA0 ##################### */
/* PWM signal 20msec period, 50 Hz */
/* Period 19.5 msec OFF, 0.5 msec ON --> Minimum Pulse Width   0 degree */
/* Period 17.5 msec OFF, 2.5 msec ON --> Neutral Point        90 degree */
/* Period 18.5 msec OFF, 1.5 msec ON --> Maximum Pulse Width 180 degree */

#define MICRO_SEC_IN_ONE_SECOND   1000000   /* Number of uSec in 1 second */
#define PCA0_16BIT_LENGTH         0x10000   /* PCA0 counter/timer lenght 65536 */
#define CEXn_FREQ_50Hz                 50   /* PWM Frequency to output on CEXn (Hz) */

// 50Hz period value in uSecond 20,000 uSec -> 20mSec
#define PERIOD_VALUE_IN_USEC  USEC_IN_ONE_SECOND/CEXn_FREQ_50Hz

// 65536 counts equal to 20,000 uSec,so how many counts require for 1.5mSec
// if PCA0 input freq is 3MHz?
//#define CEXn_PERIOD_50Hz      65536   /* PWM Frequency to output on CEXn (Hz) */
// Initialize vale of various servo motors
#define GRIPPER_INIT_VALUE    61036   /* For 1.5 msec: 20 - 1.5 = 18.5 msec 0xEE6C */


/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Control servo motors
  * @param
  * @note   CEX0_SM1_Compare_Value;
  *         CEX0_SM1_Compare_Value;
  *
  *         PCA0CPn_MIN_VAL       64036    For 0.5 msec: 20 - 0.5 = 19.5 msec 0xFA24
  *         PCA0CPn_NEUTRAL_VAL   61036    For 1.5 msec: 20 - 1.5 = 18.5 msec 0xEE6C
  *         PCA0CPn_MAX_VAL       58036    For 2.5 msec: 20 - 2.5 = 17.5 msec 0xE2B4
  * @retval
  */



/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/

                            
