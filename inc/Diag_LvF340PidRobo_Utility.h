/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Utility.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Utility.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_UTILITY_H
#define __LVF340_PID_ROBO_UTILITY_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup UTILITY
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern char code ascii[10];

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint16 data MemStartAddress;      /* buffer for start address */
extern uint16 data MemEndAddress;        /* buffer for end address */

/* Exported functions --------------------------------------------------------*/
void UintToAscii(uint16 u_val, uchar *result);
uint16 AsciiToUint(char *str);
void IntegerToAscii(int u_val, uchar *result);
uchar BCDToInt(uchar BCD);

void HexToBit(uchar number);
uchar HexToBin(uchar val);
void WordToBit(uint16 number);

char GetAtoI(void);
uchar Get2ASCIIToBCD(uchar MSB, uchar LSB);

char GetMemStartEndAddr(uchar digit);
uchar GetValue(uchar MaxDigit);
uint16 GetHex(uchar digit);

void EraseChar (void);
void EraseCharWithUnderScore(void);

void EraseBackLash(void);
void EraseSemiColon(void);
void EraseAndRingTheBell(void);

#endif /* __LVF340_PID_ROBO_UTILITY_H */

/**
  * @}
  */

/**
  * @}
  */


/*****************************END OF FILE**************************************/
