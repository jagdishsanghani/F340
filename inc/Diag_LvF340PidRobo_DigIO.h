/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_DigIO.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_DigIO.c module.
  *          This file contains the headers of the interrupt handlers.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_DIGIO_H
#define __LVF340_PID_ROBO_DIGIO_H

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup DigIO
  * @{
  */


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported Bit Definitions for F340PID-ROBO board----------------------------*/
sbit RELAY1     = P2^0;      /* RELAY 1 */
sbit RELAY2     = P2^1;      /* RELAY 2 */

/* Exported functions ------------------------------------------------------- */
void WriteRelays(uchar RelayNum, bit RelayOnOff);


#endif /* __LVF340_PID_ROBO_DIGIO_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
