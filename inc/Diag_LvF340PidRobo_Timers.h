/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Timers.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Timers.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_TIMERS_H
#define __LVF340_PID_ROBO_TIMERS_H

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup TIMERS
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define  TIMER2_PRESCALER   12   /* Based on Timer2 CKCON and TMR2CN settings */

#define  AUX1                     TIMER2_TICKS_PER_10MS     /* 0x2710 */
#define  AUX2                     -AUX1                     /* 0xD8F0 */
#define  TIMER2_RELOAD_100Hz       AUX2    /* Reload value for Timer2 */

/* There are SYSCLK/TIMER_PRESCALER timer ticks per second, so */
/* SYSCLK/TIMER_PRESCALER/100 = 10000 counts per 10 millisecond. */
#define  TIMER2_TICKS_PER_10MS    SYSCLK/TIMER2_PRESCALER/100
#define  AUX1                     TIMER2_TICKS_PER_10MS     /* 0x2710 */
#define  AUX2                     -AUX1                     /* 0xD8F0 */
#define  TIMER2_RELOAD_100Hz      AUX2     /* Reload value for Timer2 */

/* SYSCLK/TIMER_PRESCALER/1000 = 1000 counts per 1 millisecond. */
#define  TIMER2_TICKS_PER_1MS     SYSCLK/TIMER2_PRESCALER/1000
#define  AUX3                     TIMER2_TICKS_PER_1MS      /* 0x03E8 */
#define  AUX4                     -AUX3                     /* 0xFA18 */
#define  TIMER2_RELOAD_1000Hz      AUX4    /* Reload value for Timer2 */

/* SYSCLK/TIMER_PRESCALER/2000 = 500 counts per 1/2 millisecond. */
#define  TIMER2_TICKS_PER_500uS    SYSCLK/TIMER2_PRESCALER/2000
#define  AUX5                      TIMER2_TICKS_PER_500uS    /* 0x01F4 */
#define  AUX6                      -AUX5                     /* 0xFE0C */
#define  TIMER2_RELOAD_2000Hz      AUX6     /* Reload value for Timer2 */

/* SYSCLK/TIMER_PRESCALER/10000 = 100 counts per 1/10 millisecond. */
#define  TIMER2_TICKS_PER_100uS    SYSCLK/TIMER2_PRESCALER/10000
#define  AUX7                      TIMER2_TICKS_PER_100uS    /* 0x0064 */
#define  AUX8                      -AUX7                     /* 0xFF9C */
#define  TIMER2_RELOAD_10kHz       AUX8      /* Reload value for Timer2 */

/* SYSCLK/TIMER_PRESCALER/20000 = 50 counts per 1/20 millisecond. */
#define  TIMER2_TICKS_PER_50uS     SYSCLK/TIMER2_PRESCALER/20000
#define  AUX9                      TIMER2_TICKS_PER_50uS     /* 0x0032 */
#define  AUX10                     -AUX9                     /* 0xFFE0 */
#define  TIMER2_RELOAD_20kHz       AUX10    /* Reload value for Timer2 */

/* TIMER3 SETUP */
#define  TIMER3_PRESCALER   12     /* Based on Timer3 CKCON and TMR3CN settings */

/* SYSCLK/TIMER_PRESCALER/10000 = 100 counts per 1/10 millisecond. */
#define  TIMER3_TICKS_PER_100uS    SYSCLK/TIMER3_PRESCALER/10000
#define  T3_AUX1     TIMER3_TICKS_PER_100uS    /* 0x0064 = 100 */
#define  T3_AUX2     -T3_AUX1                  /* 0xFF9C */
#define  TIMER3_RELOAD_10kHz   T3_AUX2         /* Reload value for Timer2 */


/* SYSCLK/TIMER_PRESCALER/20000 = 50 counts per 1/20 millisecond. */
#define  TIMER2_TICKS_PER_50uS    SYSCLK/TIMER2_PRESCALER/20000
#define  T3_AUX3     TIMER2_TICKS_PER_50uS     /* 0x0032 */
#define  T3_AUX4    -T3_AUX3                   /* 0xFFE0 */
#define  TIMER3_RELOAD_20kHz   T3_AUX4         /* Reload value for Timer2 */

/* Delays-----------*/
#define	 DELAY_10MSEC      10       /* Value for 10 minutes Delay */
#define	 DELAY_20MSEC      20       /* Value for 20 minutes Delay */
#define	 DELAY_50MSEC      50       /* Value for 50 minutes Delay */

#define	 DELAY_100MSEC    100       /* Value for 100 minutes Delay */
#define	 DELAY_200MSEC    200       /* Value for 200 minutes Delay */
#define	 DELAY_500MSEC    500       /* Value for 500 minutes Delay */
#define	 DELAY_5000MSEC  5000       /* Value for 500 minutes Delay */

/* LEDs -----------*/
sbit HEART_BEAT = P2^2;             /* heart-beat LED timer2 interrupt */
sbit TICK_LED   = P2^3;             /* TICK-LED */

/* Exported macro -----------------------------------------------------------*/

/* SFR16 Definitions --------------------------------------------------------*/
sfr16 TMR2RL   = 0xCA;   /* Timer2 reload  registers. TMR2RLL  TMR2RLH */
                         /*                            0xCA     0xCB   */

sfr16 TMR2     = 0xCC;   /* Timer2 counter registers. TMR2L    TMR2H   */
                         /*                            0xCC     0xCD   */

sfr16 TMR3RL   = 0x92;   /* Timer3 reload registers.  TMRRLL   TMR3RLH */
                         /*                            0x92     0x93   */

sfr16 TMR3     = 0x94;   /* Timer3 counter registers. TMR3L    TMR3H   */
                         /*                            0x94     0x95   */

/* Exported variables -------------------------------------------------------*/
extern bit TimerTick;
extern uint16 HeartBeatTimer;      /* Timer for LED */
extern uint16 DelayTimer;          /* Delay Timer */
extern uint16 DisplayTimer;
extern uint16 LedDelay;
extern uchar LED_Timer;           /* Timer for LED */

/* Exported functions -------------------------------------------------------*/
void SYSCLK_Init (void);
void Timer0_Init(void);       /* Timer0 to generate SMBus Clock. */
void Timer2_Init (void);       /* Timer2 to generate interrupts at a 1000Hz rate. */
void Timer3_Init(void);       /* Timer3 for use with SMBus low timeout detect */
void msecDelay(uint16 msec);


#endif /* __LVF340_PID_ROBO_TIMERS_H */

/**
  * @}
  */

/**
  * @}
  */


/*****************************END OF FILE**************************************/
