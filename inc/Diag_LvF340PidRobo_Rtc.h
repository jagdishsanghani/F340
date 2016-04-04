/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Rtc.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Rtc.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_RTC_H
#define __LVF340_PID_ROBO_RTC_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup RTC
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern char code * code Time;
extern char code * code Date;
extern char code * code DS1338Menu[6];

/* Exported macro ------------------------------------------------------------*/
#define  DS1338_ID      0xD0  /* Device address for slave target */
                              /* Note: This address is specified in the */
                              /* MAXIM-IC  datasheet. */
/* Exported variables for RTC DS1338 -----------------------------------------*/
 extern uchar RtcBcdHours;               /* BCD data from DS1338  */
 extern uchar RtcBcdMins;
 extern uchar RtcBcdSecs;
 extern uchar RtcBcdDOW;
 extern uchar RtcBcdDOM;
 extern uchar RtcBcdMonth;
 extern uchar RtcBcdYear;
 extern uchar RtcBcdCentury;

 extern uchar data TimeBuffer[3];           /* Temporary storage for Time &Date Value */
 extern uchar data DateBuffer[4];           /* Temporary storage for Time &Date Value */

 extern uchar data RtcReadBuff[7];

/* Exported functions ------------------------------------------------------- */
void ReadDS1338TimeAndDate(void);
void WriteDS1338TimeAndDate(void);
char DisplayDS1338TimeAndDate(void);

void SetDS1338Time(void);
void SetDS1338Date(void);
char ChangeDS1338Time(void);
char ChangeDS1338Date(void);

void PrintDS1338Menu(void);

#endif /* __LVF340_PID_ROBO_RTC_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
