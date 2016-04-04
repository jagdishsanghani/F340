/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_ServoMotors.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_ServoMotors.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_SERVOMOTORS_H
#define __LVF340_PID_ROBO_SERVOMOTORS_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup SERVO_MOTORS
  * @{
  */


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ControlServosWithSwitches (void);
void EnableServoPWMsignals(void);

#endif /* __LVF340_PID_ROBO_SERVOMOTORS_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
