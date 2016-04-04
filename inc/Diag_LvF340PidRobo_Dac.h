/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Dac.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   This file contains all the functions prototypes for the DAC firmware
  *          library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_DAC_H
#define __LVF340_PID_ROBO_DAC_H

/** @addtogroup F340_PID_ROBO
  * @{
  */
/** @addtogroup DAC
  * @{
  */


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
sbit DAC7614_XFER  = P2^5;              /* LOAD DAC LED on port P2.5 */

/* Exported variables -------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void PrintDac7614Menu(void);

#endif /* __LVF340_PID_ROBO_DAC_H */

/**
  * @}
  */

/**
  * @}
  */


/*****************************END OF FILE**************************************/
