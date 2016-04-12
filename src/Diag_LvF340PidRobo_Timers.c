/**
  ******************************************************************************
  * @file    Diag_F340_Timers.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Timers module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of Timers peripheral:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup TIMERS
  * @ Timers driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  SMB_FREQUENCY  50000  /* Target SCL clock rate */
                               /* This example supports between 10kHz and 100kHz */

/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
sbit BASE_SMOT           = P3^5;    /* Base Servo Motor PWM signal */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bit TimerTick;
uint16 HeartBeatTimer;      /* Timer for LED */
uint16 DelayTimer;          /* Delay Timer */
uint16 DisplayTimer;
uint16 LedDelay;

bit base_motor_pwm_start;
bit is_t3_period_finish;

uint16 t3_pwm_on_time;
uint16 t3_pwm_off_time;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * ***************************************************************************
  * @brief
  * @param  None
  * @retval None
  * @note
  *    : This routine initializes the system clock to use the internal
  *    : oscillator at its maximum frequency.The OSCICL register is factory
  *    : calibrated to obtain a 12 MHz internal oscillator frequency.
  *    : ControlBit7:IOSCEN: Internal H-F Oscillator Enable Bit.
  *    : Also enables the Missing Clock Detector.
  *    :
  *    : OSCICN:    Bit7   Bit6   Bit5   Bit4   Bit3    Bit2    Bit1    Bit0
  *    : SFR 0xB2  IOSCEN  IFRDY  SUSPEND  -      -       -    IFCN1   IFCN0
  *    : Reset Value  1      0      0      0      0       0       1      1
  *    :
  *    : CLKSEL:      -      0      0      0      -       0       0      0
  *    : SFR 0xA9 --> Reset Value 0x00 CLKSL = 000;Clock000Internal Oscillator
  *    : (as determined by the IFCN bits in register OSCICN)
  *****************************************************************************
  */
void SYSCLK_Init (void)
{
  OSCICN |= 0x03;    /*  Configure internal oscillator for its maximum frequency 12MHz */
//   OSCICN |= 0x83;    /*  Bits1–0:IFCN1–0: Internal H-F Oscillator Frequency Control.   */
                      /*  00: SYSCLK derived from Internal H-F Oscillator divided by 8. */
                      /*  01: SYSCLK derived from Internal H-F Oscillator divided by 4. */
                      /*  10: SYSCLK derived from Internal H-F Oscillator divided by 2. */
                      /*  11: SYSCLK derived from Internal H-F Oscillator divided by 1. */

    RSTSRC  = 0x04;   /* Enable missing clock detector */
}

/**
  *****************************************************************************
  * @brief
  * @param  None
  * @retval None
  * @note Timer0 is configured as the SMBus clock source as follows:
  *         - Timer0 in 8-bit auto-reload mode
  *         - SYSCLK / 12 as Timer0 clock source
  *         - Timer0 overflow rate => 3 * SMB_FREQUENCY
  *         - The maximum SCL clock rate will be ~1/3 the Timer0 overflow rate
  *         - Timer0 enabled
  *****************************************************************************
  */
void Timer0_Init (void)
{
/* Make sure the Timer can produce the appropriate frequency in 8-bit mode     */
/* Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register */
/* settings may need to change for frequencies outside this range.             */

#if ((SYSCLK/SMB_FREQUENCY/3) < 255)   /* 12000000/50000/3 = 80  */
   #define SCALE 1

   CKCON |= 0x04;           /*               CKCON REGISTER 0x8E               */
                            /*  D7    D6    D5    D4      D3   D2    D1    D0  */
                            /* T3MH  T3ML  T2MH  T2ML --  T1M  T0M  SCA1  SCA0 */
                            /*   0     0     0     0       0    1    0     0   */
                            /* Timer0 clock source = SYSCLK                    */

#elif ((SYSCLK/SMB_FREQUENCY/4/3) < 255)
   #define SCALE 4
      CKCON |= 0x01;
      CKCON &= ~0x06;       /* Timer0 clock source = SYSCLK / 4 */

#endif

   TMOD = 0x02;                          /* Timer0 in 8-bit auto-reload mode2      */

   TH0 = -(SYSCLK/SMB_FREQUENCY/12/3);   /* Timer0 configured to overflow at 1/3   */
                                         /* the rate defined by SMB_FREQUENCY      */
                                         /* 12000000/50000/12/3 = 6 --> -6 -->0xFA */
   TL0 = TH0;                            /* Init Timer0                            */

   TR0 = 1;    /*   TCON Timer Control REGISTER 0x88        */
               /* D7   D6   D5   D4      D3   D2   D1   D0  */
               /* TF1  TR1  TF0  TR0 --  IE1  IT1  IE0  IT0 */
               /*  0    0    0    1       0    0    0    0  */
               /* Timer0 enabled                            */
}

/**
  * @brief  This function configures Timer2 as a 16-bit reload timer, interrupt
  *         enabled. Using the SYSCLK at 12MHz/8 with a 1:12 prescaler.
  * @param  None
  * @retval None
  * @note   The Timer2 uses a 1:12 prescaler.  If this setting changes, the
  *         TIMER_PRESCALER constant must also be changed.
  */
void Timer2_Init(void)
{

                           /* Timer0         CKCON REGISTER                   */
                           /*  D7    D6    D5    D4      D3   D2    D1    D0
                           /* T3MH  T3ML  T2MH  T2ML --  T1M  T0M  SCA1  SCA0 */
                           /*   0     0     0     0       0    1    0     0   */
                           /* Timer0 clock source = SYSCLK                    */
                           /* SCA1:0 --> 0 0 = SYSCLK/12 --> 1MHz             */

                           /* Timer2 setting */
   CKCON &= ~0x60;         /*  D7    D6    D5    D4      D3   D2    D1    D0  */
                           /* T3MH  T3ML  T2MH  T2ML --  T1M  T0M  SCA1  SCA0 */
                           /*   0     0     0     0       0    1    0     0   */
                           /* Timer2 uses SYSCLK/12 --> 12MHz/12 = 1MHz       */

/* T2XCLK = 0 --> Timer2 external clock selection is SYSCLK/12 --> 1MHz   */
   TMR2CN &= ~0x01;        /*  TMR2CN Timer2 Control Register             */
                           /*  D7    D6     D5     D4        D3     D2    D1     D0    */
                           /* TF2H  TF2L TF2LENH  T2CE --  T2SPLIT  TR2  T2CSS  T2XCLK */
                           /*   0     0     0      0          0      0     0      0    */

   TMR2RL = TIMER2_RELOAD_1000Hz;  /* Reload value to be used in Timer2 */
//   TMR2RL = TIMER2_RELOAD_100Hz;  /* Reload value to be used in Timer2 */
   TMR2 = TMR2RL;           /* Init the Timer2 register          */

   TMR2CN = 0x04;          /*  D7    D6     D5     D4        D3     D2    D1     D0    */
                           /* TF2H  TF2L TF2LENH  T2CE --  T2SPLIT  TR2  T2CSS  T2XCLK */
                           /*   0     0     0      0        0        1     0      0    */
                           /* Enable Timer2 in auto-reload mode                        */

   ET2 = 1;                /*  IE Interrupt Enable REGISTER               */           
                           /*  D7   D6    D5   D4      D3   D2   D1   D0  */
                           /*  EA  ESPIO  ET2  ES0 --  ET1  EX1  ET0  EX0 */
                           /*  0     0     1    0       0    0    0    0  */
                           /* Timer2 interrupt enabled                    */
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note   This routine changes the state of the LED whenever Timer2 overflows.
  */
void Timer2_ISR (void) interrupt 5
{
  if(HeartBeatTimer)
  {
    --HeartBeatTimer;	          /*decrement LED timer */
  }
  else
  {
    HEART_BEAT = ~HEART_BEAT;     /* Change state of LED */
    HeartBeatTimer = 50;          /* Reload Delay        */
  }

  if(DisplayTimer)
  {
    --DisplayTimer;	              /*decrement LED timer */
  }

  if(DelayTimer)
  {
    --DelayTimer;	              /*decrement LED timer */
  }
  if(LedDelay)
  {
    --LedDelay;	                  /*decrement LED timer */
  }

 /* generate one note; freq + duration */
  if(PlayNote)
  {
    if(TimeOut)  /* generate freq */
    {
      if(OnTimer)
      {
        BUZZER = ON;
        --OnTimer;
      }
      else
      {
        BUZZER = OFF;
        --OffTimer;
      }
      if(OffTimer == 0)
      {
        OnTimer = OnTime;
        OffTimer = OffTime;
      }
      --TimeOut;  /*decrement timer */
    }
  }
  TF2H = 0;                       /* Clear Timer2 interrupt flag */
}


/******************************************************************************
  * @brief   Timer3 configured for use by the BASE_SERVO to generate 50Hz PWM signal:
  *            - Timer3 in 16-bit auto-reload mode
  *            - SYSCLK/12 as Timer3 clock source
  *            - Timer3 enabled
  * @param  None
  * @retval None
  * @note   For Timer3 100usec Interrupt
  *         TIMER3_AUTORL_100USEC_INTR  65336
  *      //65536-200=65336, 100usec (0.1msec) Interrupt
  *
  *    TIMER3_MIN_ON_VAL               5  // 5 x 100uSec = 0.5mSec
  *    TIMER3_MAX_ON_VAL              25  // 25 x 100uSec = 2.5mSec
  *    TIMER3_PERIOD_VAL             200  // 200 x 100uSec = 20mSec; T=1/F
  *    Add code here for P3.5-SBASE (using timer T3 interrupt, 0.1msec Timer3Tick),
  *    BASE_SMOT
  *    CEX5_Base_Compare_Value = TIMER3_NEUTRAL_VAL; // 15 for 1.5mSec PWM Period
  *******************************************************************************
  */
void Timer3_Init (void)
{
  TMR3CN = 0x00;           /* Timer3 configured for 16-bit auto-reload  */
                           /* low-byte interrupt disabled */

                           /* T3XCLK = 0 --> SYSCLK/12 --> 1MHz   */
  TMR3CN &= ~0x01;         /* TMR3CN Timer3 Control Register      */
                           /*  D7    D6     D5     D4        D3     D2    D1     D0    */
                           /* TF3H  TF3L TF3LENH  T3CE --  T3SPLIT  TR3  T3CSS  T3XCLK */
                           /*   0     0     0      0          0      0     0      0    */


                           /* CKCON Register */
  CKCON &= ~0x40;          /*  D7    D6    D5    D4      D3   D2    D1    D0  */
                           /* T3MH  T3ML  T2MH  T2ML --  T1M  T0M  SCA1  SCA0 */
                           /*   0     0     0     0       0    0    0     0   */
                           /* Timer3 uses SYSCLK/12 --> 12MHz/12 = 1MHz       */

  TMR3RL = TIMER3_RELOAD_10kHz;   /* Timer3 configured to overflow after  */
  TMR3 = TMR3RL;

  /* Add code here for P3.5-SBASE (using timer T3 interrupt, 0.1msec Timer3Tick), BASE_SMOT */
  CEX5_Base_Compare_Value = TIMER3_NEUTRAL_VAL;         /* 15 for 1.5mSec PWM Period */

  t3_pwm_on_time = CEX5_Base_Compare_Value;
  t3_pwm_off_time = TIMER3_RELOAD_10kHz - CEX5_Base_Compare_Value;

  EIE1 |= 0x80;            /* Timer3 interrupt enable */

  TMR3CN |= 0x04;          /* TR3=1 -> Timer3 enable */

}

/*
*****************************************************************************************
Function: void timer3_isr (void) interrupt 14

Arg1: -

Arg2: -

Return(s): nothing
                    :
Description: P3.5-SBASE (using timer T3 interrupt, 0.1msec Timer3Tick), BASE_SMOT
             CEX5_Base_Compare_Value = TIMER3_NEUTRAL_VAL;   15 for 1.5mSec PWM Period

*****************************************************************************************
*/
void Timer3_ISR (void) interrupt 14
{
  if(base_motor_pwm_start)            /* start PWM signal ? */
  {
    if(is_t3_period_finish)
    {
      // update data for next period
      t3_pwm_on_time = CEX5_Base_Compare_Value;
      t3_pwm_off_time = TIMER3_RELOAD_10kHz - CEX5_Base_Compare_Value;

      BASE_SMOT = ON;               /* set PWM signal ON */
      is_t3_period_finish = NO;
    }

    else if(t3_pwm_on_time)         /* If ON */
    {
      BASE_SMOT = ON;               /* set PWM signal ON */
      --t3_pwm_on_time;             /* decrement ON time */
    }

    else if(t3_pwm_off_time)
    {
      BASE_SMOT = OFF;              /* set PWM signal OFF */
      --t3_pwm_off_time;            /* decrement OFF time */
      if(t3_pwm_off_time == 0)      /* If OFF time finish */
      {
        is_t3_period_finish = YES;  /* Finished One Period */
      }
    }
  }

  TMR3CN &= ~0x80;                  /* Clear TF3H Timer3 Hih Byte Overflow flag */
}

/**
  * @brief  This function displays the sensor readings using a bar graph.
  * @param  None
  * @retval None
  * @note
  */
void msecDelay(uint16 msec)
{
  DelayTimer = msec;
  while(DelayTimer)
  {
    /* do nothing */
  }
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/

