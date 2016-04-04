/**
  ******************************************************************************
  * @file    DiagLvF340PidRobo_conf.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   LvF340PID_ROBO board configuration file
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_CONF_H
#define __LVF340_PID_ROBO_CONF_H

#include "c8051f340.h"

/* Exported types ------------------------------------------------------------*/
typedef unsigned char  uchar;      /* 8-bit types */
typedef unsigned char  u_char;
typedef signed char    schar;
typedef signed char    s_char;

typedef unsigned int   uint16;     /* 16-bit types */
typedef unsigned int   u_int16;
typedef signed int     int16;

typedef unsigned long  ulong32;    /* 32-bit types */
typedef unsigned long  u_long32;
typedef signed long    long32;

typedef unsigned char  BYTE;       /* 8-bit types */

/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the LVF340_PID_ROBO driver
  */

#define LVF340_ADC_MODULE_ENABLED
#define LVF340_DAC_MODULE_ENABLED
#define LVF340_DIGIO_MODULE_ENABLED
#define LVF340_HALLSEN_MODULE_ENABLED
#define LVF340_LCD_MODULE_ENABLED
#define LVF340_MSG_MODULE_ENABLED
#define LVF340_PCA_MODULE_ENABLED
#define LVF340_PHOTOCELL_MODULE_ENABLED
#define LVF340_PRINTF_MODULE_ENABLED
#define LVF340_RTC_MODULE_ENABLED
#define LVF340_SERVOMOTORS_MODULE_ENABLED
#define LVF340_SMBUS_MODULE_ENABLED
#define LVF340_SPEAKER_MODULE_ENABLED
#define LVF340_SPI_MODULE_ENABLED
#define LVF340_SWITCHES_MODULE_ENABLED
#define LVF340_SYSTEM_MODULE_ENABLED
#define LVF340_TEST_MODULE_ENABLED
#define LVF340_TIMERS_MODULE_ENABLED
#define LVF340_UART_MODULE_ENABLED
#define LVF340_UTILITY_MODULE_ENABLED
#define LVF340_VT220_MODULE_ENABLED
#define LVF340_24LC1025_MODULE_ENABLED

/* ########################## HSE/HSI Values adaptation ##################### */
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the xxxxx module to compute the system frequency
  *        (when HSE is used as system clock source, directly or through the PLL).  
  */
#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)16000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL). 
  */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)12000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */


/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

/* ########################### System Configuration ######################### */
/**
  * @brief This is the xxx system configuration section
  */     

/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file 
  */

#ifdef LVF340_ADC_MODULE_ENABLED
  #include "Diag_LvF340PidRobo_Adc.h"
#endif /* LVF340_ADC_MODULE_ENABLED */

#ifdef LVF340_DAC_MODULE_ENABLED
  #include "Diag_LvF340PidRobo_Dac.h"
#endif /* LVF340_DAC_MODULE_ENABLED */

#ifdef LVF340_DIGIO_MODULE_ENABLED
  #include "Diag_LvF340PidRobo_DigIO.h"
#endif /* LVF340_DIGIO_MODULE_ENABLED */

#ifdef LVF340_HALLSEN_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_HallSensor.h"
#endif /* LVF340_HALLSEN_MODULE_ENABLED */

#ifdef LVF340_LCD_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Lcd.h"
#endif /* LVF340_LCD_MODULE_ENABLED */

#ifdef LVF340_MSG_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Msg.h"
#endif /* LVF340_MSG_MODULE_ENABLED */

#ifdef LVF340_PCA_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Pca.h"
#endif /* LVF340_PCA_MODULE_ENABLED */

#ifdef LVF340_PHOTOCELL_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Photocell.h"
#endif /* LVF340_PHOTOCELL_MODULE_ENABLED */

#ifdef LVF340_PRINTF_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Printf.h"
#endif /* LVF340_PRINTF_MODULE_ENABLED */

#ifdef LVF340_RTC_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Rtc.h"
#endif /* LVF340_RTC_MODULE_ENABLED */

#ifdef LVF340_SERVOMOTORS_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_ServoMotors.h"
#endif /* LVF340_SERVOMOTORS_MODULE_ENABLED */

#ifdef LVF340_SMBUS_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_SMBus.h"
#endif /* LVF340_SMBUS_MODULE_ENABLED */

#ifdef LVF340_SPEAKER_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Speaker.h"
#endif /* LVF340_SPEAKER_MODULE_ENABLED */

#ifdef LVF340_SPI_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_SpiBus.h"
#endif /* LVF340_SPI_MODULE_ENABLED */

#ifdef LVF340_SWITCHES_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Switches.h"
#endif /* LVF340_SWITCHES_MODULE_ENABLED */

#ifdef LVF340_SYSTEM_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_System.h"
#endif /* LVF340_SYSTEM_MODULE_ENABLED */

#ifdef LVF340_TEST_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Test.h"
#endif /* LVF340_TEST_MODULE_ENABLED */

#ifdef LVF340_TIMERS_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Timers.h"
#endif /* LVF340_TIMERS_MODULE_ENABLED */

#ifdef LVF340_UART_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Uart.h"
#endif /* LVF340_UART_MODULE_ENABLED */

#ifdef LVF340_UTILITY_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_Utility.h"
#endif /* LVF340_UTILITY_MODULE_ENABLED */

#ifdef LVF340_VT220_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_VT220.h"
#endif /* LVF340_VT220_MODULE_ENABLED */

#ifdef LVF340_24LC1025_MODULE_ENABLED
 #include "Diag_LvF340PidRobo_24LC1025.h"
#endif /* LVF340_24LC1025_MODULE_ENABLED */

/* Exported macro ------------------------------------------------------------*/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef NULL
#define NULL  0
#endif

#ifndef FORWARD
#define FORWARD 1
#endif

#ifndef REVERSE
#define REVERSE 0
#endif

#ifndef START
#define START 1
#endif

#ifndef STOP
#define STOP  0
#endif

#ifndef ENABLE
#define ENABLE 1
#endif

#ifndef DISABLE
#define DISABLE 0
#endif

#ifndef	ToUpper
#define	ToUpper(c)		('a' <= (c) && (c) <= 'z' ? (c)-32 : (c))
#endif

#define  MINUS_SIGN     0xAD; /* put minus sign in location result[0] */

/* Exported Constants --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


#endif /* __F340_PID_ROBO_CONF_H */
 

/*****************************END OF FILE**************************************/
