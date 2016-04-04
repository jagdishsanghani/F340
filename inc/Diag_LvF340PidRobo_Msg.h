/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Msg.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Msg.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_MSG_H
#define __LVF340_PID_ROBO_MSG_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup MSG
  * @{
  */
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern char code * code AskExit;
extern char code * code AskMore;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void PrintMainMenu(void);
void PrintEscape(void);
void PrintProgUnderDev(void);


#endif /* __LVF340_PID_ROBO_MSG_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
