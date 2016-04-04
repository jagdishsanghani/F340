/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Uart.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Uart.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_UART_H
#define __LVF340_PID_ROBO_UART_H

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup UART
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern bit  TxBuff0_Avail;        /* indicates empty tx buffer */
extern bit  RxData0_Avail;        /* indicates Received data is available in buffer */
extern bit  EscFlag;              /* indicates escape key is pressed */
extern data ReceiveData;          /* Buffer for receive data */

/* Exported functions --------------------------------------------------------*/
void UART0_Init (void);


#endif /* __LVF340_PID_ROBO_UART_H */

/**
  * @}
  */

/**
  * @}
  */


/*****************************END OF FILE**************************************/
