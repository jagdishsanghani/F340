/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_SMBus.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_SMBus.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_SMBUS_H
#define __LVF340_PID_ROBO_SMBUS_H

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup SMBus
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
sbit SDA         = P1^0;              /* SMBus on P1.0 */
sbit SCL         = P1^1;              /* and P1.1 */

/* Exported SMBus variables --------------------------------------------------*/
/***** variables for SMBus  ******/
extern uchar* pSMB_DATA_IN;       /* Global pointer for SMBus data */
                                  /* All receive data is written here */

extern uchar SMB_SINGLEBYTE_OUT;  /* Global holder for single byte writes. */

extern uchar* pSMB_DATA_OUT;      /* Global pointer for SMBus data. */
                                  /* All transmit data is read from here */

extern uchar SMB_DATA_LEN;        /* Global holder for number of bytes */
                                  /* to send or receive in the current SMBus transfer. */

extern uint16 WORD_ADDR;          /* Global holder for the EEPROM word address that will */
                                  /* be accessed in the next transfer */

extern uchar TARGET;              /* Target SMBus slave address */

extern bit SMB_BUSY;              /* Software flag to indicate when the RTC_ByteRead() or */
                                  /* RTC_ByteWrite() functions have claimed the SMBus */

extern bit SMB_RW;                /* Software flag to indicate the */
                                  /* direction of the current transfer */

extern bit SMB_SENDWORDADDR;      /* When set, this flag causes the ISR to send the 8-bit */
                                  /* <WORD_ADDR> after sending the slave address. */

extern bit SMB_RANDOMREAD;        /* When set, this flag causes the ISR to send a */
                                  /* START signal after sending the word address. */

extern bit SMB_ACKPOLL;           /* When set,this flag causes the ISR to send a repeated */
                                  /* START until the slave has acknowledged its address */

extern bit SMB_EEPROM;           /* Software flag to indicate that device is eeprom */
extern bit SMB_EEP_HIGHBYTE;      /* Flag for EEProm high byte address */

/* Exported functions ------------------------------------------------------- */
void InitSMBus (void);
void SMbusWrite(uchar SMBusDeviceID, uint16 DestAddr, uchar* pDataStart, uchar Len);
void SMbusRead(uchar SMBusDeviceID, uchar* DestAddr, uint16 SrcAddr, uchar Len);

#endif /* __LVF340_PID_ROBO_SMBUS_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
