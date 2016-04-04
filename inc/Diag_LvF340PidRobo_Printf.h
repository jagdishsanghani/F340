/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Printf.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Printf.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_PRINTF_H
#define __LVF340_PID_ROBO_PRINTF_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup PRINTF
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
schar putchar (uchar c);
schar getche(void);
int printf(const char *format, ...);

#endif /* __LVF340_PID_ROBO_PRINTF_H */


/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
