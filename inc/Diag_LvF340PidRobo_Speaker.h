/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Speaker.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Speaker.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_SPEAKER_H
#define __LVF340_PID_ROBO_SPEAKER_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup SPEAKER
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board------------------------------------*/
sbit BUZZER = P2^4;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables for Speaker --------------------------------------------*/

/* Speaker */
extern uint16 TimeOut;             /* timeout for note */
extern uint16 OnTime;
extern uint16 OffTime;

extern uint16 OnTimer;
extern uint16 OffTimer;

extern bit PlayNote;
extern bit Playing;

/* Exported functions ------------------------------------------------------- */
void PlayWelcomeTune(void);
void PlayTune(void);

#endif /* __LVF340_PID_ROBO_SPEAKER_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
