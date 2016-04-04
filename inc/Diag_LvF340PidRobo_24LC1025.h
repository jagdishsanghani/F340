/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_24LC1025.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Adc.c module.
  *          This file contains the headers of the interrupt handlers.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_24LC1025_H
#define __LVF340_PID_ROBO_24LC1025_H

/** @addtogroup BSP
  * @{
  */

/** @addtogroup EEPROM
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* c8051F340 Word Registers --------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/**** I2C EEPROM  24LC1025 128k8 ****/

/*****************************************************
       |-Control Code  |
   ------------------------------------------------
   | S | 1 | 0 | 1 | 0 | B0 | A1 | A0 | R/Wn| ACK |
   ------------------------------------------------
       |--------Slave Address---------|

B0     -> Block Select Bits
A1, A0 -> Chip Select Bits
S      -> Start Bit
ACK    -> Acknowledge Bit
R/Wn   -> Read/Write Bit

*****************************************************/
#define  EEP24LC1025_ID              0xA0    /* slave address */

#define  EEP24LC1025_PAGE_LEN        0x7F    /* 128 bytes page length */

#define  EEP24LC1025_START_ADDR      0x00000 /* BO = 0; 0000h -> FFFFh   */
#define  EEP24LC1025_END_ADDR        0x1FFFF /* BO = 1; 10000h - 1FFFFh  */

#define  EEP24LC1025_BO1_CMD         0x08    /* 0b0000 1000 Block Select-1 */
#define  EEP24LC1025_BO1_START_ADDR  0x10000 /* Start address of Block 1 */

#define  MAX_DIGIT      4       /* Change if you need */
#define  MAX_HEX        8       /* Maximum hex nibbles */


/* Write cycle time (byte or page) 5 ms max , 3 ms, typical */

/* Exported variables ------------------------------------------------------------*/
/* EEP 24LC1025 */
extern uchar EepBufferWrite[129];          /* eeprom's write data buffer */
extern uchar EepBufferRead[129];           /* eeprom's read data buffer */

/* Exported functions ------------------------------------------------------- */
uchar Read24LC1025Eeprom(void);
uchar Write24LC1025Eeprom(void);
uchar Erase24LC1025Eeprom(void);
void Print24LC1025Menu(void);


#endif /* __LVF340_PID_ROBO_24LC1025_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/


