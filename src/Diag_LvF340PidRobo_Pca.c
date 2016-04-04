/**
  ******************************************************************************
  * @file    Diag_F340_Pca.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   PCA module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of PCA peripheral:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup PCA
  * @ PCA driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* PAC0 PWM compare values */
uint16 CEX4_FAN_Compare_Value;         /* Holds current PCACPL4 compare value */

/* PAC0 PWM compare values */
uint16 CEX0_Gripper_Compare_Value;    /* Holds current PCACPL0 compare value */
uint16 CEX1_Wrist_Compare_Value;      /* Holds current PCACPL1 compare value */
uint16 CEX2_Hand_Compare_Value;       /* Holds current PCACPL2 compare value */
uint16 CEX3_Elbow_Compare_Value;      /* Holds current PCACPL3 compare value */
uint16 CEX4_Shoulder_Compare_Value;   /* Holds current PCACPL4 compare value */

uint16 CEX5_Base_Compare_Value;       /* Holds current T2 value */

bit base_motor_pwm;                   /* Flag for Base Motor PWM start */
bit base_motor_pwm_ontime;            /* Flag for Base Motor PWM on time */
bit base_motor_pwm_offtime;           /* Flag for Base Motor PWM off time */


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *****************************************************************************
  * @brief
  * @param  None
  * @retval None
  * @note
  *  This function configures the PCA time base, and sets up 16-bit PWM output
  *  mode for Module 4 (CEX4 pin, P3.4).
  *
  *  The frequency of the PWM signal generated at the CEX4 pin is equal to the
  *  PCA main timebase frequency divided by 65536.
  *
  *  The PCA time base in this example is configured to use SYSCLK = 12 MHZ
  *  and SYSCLK is set up to use the internal oscillator running at 12 MHz.
  *  Therefore,the minimum frequency of the PWM signal will be 12MHz/65536 = 183.11 Hz.
  *
  *              |<--- 5.46mSec -->|     183.11 Hz PWM Frequency
  *               __                __                 __
  *              |  |              |  |               |  |
  *  ____________|  |______________|  |_______________|  |_______
  *
  *        ----->|  |<--- Pulse Width 0.0833uSec(min) - 5.46ms(max)
  *
  *
  *  ---------------------------------------------------------------------------------
  *  How "16-Bit PWM Mode" Works:
  *
  *  The PCA's 16-bit PWM Mode works by setting an output pin low every
  *  time the main 16-bit PCA counter (PCA0H) overflows, and then setting
  *  the pin high whenever a specific match condition is met.
  *
  *  Upon a PCA0 overflow (PCA0 incrementing from 0xFFFF to 0x0000), the
  *  CEX4 pin will be set low.
  *
  *  When the PCA0 register increments and matches the PCA0CPn register for
  *  the selected module, the CEXn pin will be set high, except when the
  *  ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
  *  PCA0CPn register, the duty cycle of the waveform can also be varied.
  *
  *  When ECOMn = '1', the duty cycle of the PWM waveform is:
  *
  *  16-bit PWM Duty Cycle = (65536 - PCA0CPn) / 65536
  *
  *  To set the duty cycle to 100%, a value of 0x0000 should be loaded into
  *  the PCA0CPn register for the module being used (with ECOMn set to '1').
  *  When the value of PCA0CPn is equal to 0x0000, the pin will never be set low.
  *
  *  To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn register
  *  should be cleared to 0.  This prevents the PCA0CPn match from occurring,
  *  which results in the pin never being set high.
  *
  *  When adjusting the PWM duty cycle, the low byte of the PCA0CPn register
  *  (PCA0CPLn) should be written first, followed by the high byte (PCA0CPHn).
  *  Writing the low byte clears the ECOMn bit, and writing the high byte will
  *  restore it.  This ensures that a match does not occur until the full
  *  16-bit value has been written to the compare register.  Writing the high
  *  byte first will result in the ECOMn bit being set to '0' after the 16-bit
  *  write, and the duty cycle will also go to 0%.
  *
  *  It is also advisable to wait until just after a match occurs before
  *  updating the PWM duty cycle.  This will help ensure that the ECOMn
  *  bit is not cleared (by writing PCA0CPLn) at the time when a match was
  *  supposed to happen.  This code implements the compare register update in
  *  the PCA ISR upon a match interrupt.
  *
  *****************************************************************************
  */

/**
  *****************************************************************************************
Function: void pca0_init (void)

Arg1: -

Arg2: -

Return(s): nothing

Description:
    This function configures the PCA time base, and sets up 16-bit PWM output
    mode for Module 0,1, 2, 3 and 4 (CEX0, CEX1, CEX2, CEX3 and CEX4 pin).

    The frequency of the PWM signal generated at the CEX0 pin is equal to the
    PCA main time base frequency divided by 65536.

    The PCA time base in this example is configured to use SYSCLK/12 = 1 MHZ
    and SYSCLK is set up to use the internal oscillator running at 12 MHz.
    Therefore,the minimum frequency of the PWM signal will be 1 MHz/65536 = 15.25 Hz.

    To generate 50 Hz PWM frequency with 1 MHz input to PCA0, load initial value to
    65536-20,000 = 45,536 counts. Period of 50 Hz is 20mSec -> 20,000 counts. Each count
    is equivalent to 1 uSec.

                |<--- 20 msec --->|     50 Hz PWM Frequency
                 __                __                 __
                |  |              |  |               |  |
    ____________|  |______________|  |_______________|  |_______

          ----->|  |<--- Pulse Width 0.5 ms(min) - 2.5ms(max)

    The angle is determined by the duration of a pulse that is applied to the control wire.
    This is called Pulse width Modulation. The servo expects to see a pulse every 20 ms.
    The length of the pulse will determine how far the motor turns. For example, a 1.5 ms
    pulse will make the motor turn to the 90 degree position (neutral position).

    Servos are controlled by sending them a pulse of variable width. The control wire
    is used to send this pulse. The parameters for this pulse are that it has a minimum
    pulse, a maximum pulse, and a repetition rate. Given the rotation constraints of the
    servo, neutral is defined to be the position where the servo has exactly the same
    amount of potential rotation in the clockwise direction as it does in the counter
    clockwise direction. It is important to note that different servos will have different
    constraints on their rotation but they all have a neutral position, and that position
    is always around 1.5 milliseconds (ms).

    ---------------------------------------------------------------------------------
    How "16-Bit PWM Mode" Works:

    The PCA's 16-bit PWM Mode works by setting an output pin low every
    time the main 16-bit PCA counter (PCA0H) overflows, and then setting
    the pin high whenever a specific match condition is met.

    Upon a PCA0 overflow (PCA0 incrementing from 0xFFFF to 0x0000), the
    CEXn pin will be set low.

    When the PCA0 register increments and matches the PCA0CPn register for
    the selected module, the CEXn pin will be set high, except when the
    ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
    PCA0CPn register, the duty cycle of the waveform can also be varied.

    When ECOMn = '1', the duty cycle of the PWM waveform is:

    16-bit PWM Duty Cycle = (65536 - PCA0CPn) / 65536

    To set the duty cycle to 100%, a value of 0x0000 should be loaded into
    the PCA0CPn register for the module being used (with ECOMn set to '1').
    When the value of PCA0CPn is equal to 0x0000, the pin will never be set low.

    To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn register
    should be cleared to 0.  This prevents the PCA0CPn match from occuring,
    which results in the pin never being set high.

    When adjusting the PWM duty cycle, the low byte of the PCA0CPn register
    (PCA0CPLn) should be written first, followed by the high byte (PCA0CPHn).
    Writing the low byte clears the ECOMn bit, and writing the high byte will
    restore it.  This ensures that a match does not occur until the full
    16-bit value has been written to the compare register.  Writing the high
    byte first will result in the ECOMn bit being set to '0' after the 16-bit
    write, and the duty cycle will also go to 0%.

    It is also advisable to wait until just after a match occurs before
    updating the PWM duty cycle.  This will help ensure that the ECOMn
    bit is not cleared (by writing PCA0CPLn) at the time when a match was
    supposed to happen.  This code implements the compare register update in
    the PCA ISR upon a match interrupt.

CEX0_Gripper_Compare_Value;
CEX1_Wrist_Compare_Value;
CEX2_Hand_Compare_Value;
CEX3_Elbow_Compare_Value;
CEX4_Shoulder_Compare_Value;

CEX5_Base_Compare_Value;
  *****************************************************************************************
  */

void InitPca0 (void)
{
   uint16 pca0_start_val;

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
                                       // D7  6   5    4     3     2     1     D0
                                       // CF  CR  --  CCF4  CCF3  CCF2  CCF1  CCF0
                                       //  0   0   0    0     0     0     0     0

   PCA0MD = 0x00;                      // Use SYSCLK/12 = 1 MHz as time base
                                       //  D7     6     5    4    3     2     1    D0
                                       // CIDL  WDTE  WDLCK  --  CPS2  CPS1  CPS0  ECF
                                       //  0      0     0    0    0     0     0     0

   // Set 16bit PWM mode; Enable Comparator;
   PCA0CPM0 = 0xC2;                    // Module 0 = 16-bit PWM mode and
   PCA0CPM1 = 0xC2;                    // enable Module 0 Match and Interrupt Flags
   PCA0CPM2 = 0xC2;                    //   D7     6     5    4     3     2   1    D0
   PCA0CPM3 = 0xC2;                    // PWM16n ECOMn CAPPn CAPNn MATn TOGn PWMn ECCFn
   PCA0CPM4 = 0xC2;                    //   1      1     0     0    1     0    1    1
                                       //   1      1     0     0    0     0    1    0 Try this

   // Configure initial PWM duty cycle = 1.5 mSec ON, 18.5mSec OFF (Neutral Position)
   CEX0_Gripper_Compare_Value = PCA0CPn_NEUTRAL_VAL;   // For 1.5 msec: 20 - 1.5 = 18.5 msec
                                                       // 45536 + 18500 = 64036

   CEX1_Wrist_Compare_Value = PCA0CPn_NEUTRAL_VAL;     // For 1.5 msec: 20 - 1.5 = 18.5 msec
   CEX2_Hand_Compare_Value = PCA0CPn_NEUTRAL_VAL;      // For 1.5 msec: 20 - 1.5 = 18.5 msec
   CEX3_Elbow_Compare_Value = PCA0CPn_NEUTRAL_VAL;     // For 1.5 msec: 20 - 1.5 = 18.5 msec
   CEX4_Shoulder_Compare_Value = PCA0CPn_NEUTRAL_VAL;  // For 1.5 msec: 20 - 1.5 = 18.5 msec


   PCA0CPL0 = (CEX0_Gripper_Compare_Value & 0x00FF);     /* 64036 = Neutral Position */
   PCA0CPH0 = (CEX0_Gripper_Compare_Value & 0xFF00)>>8;  /* P3.0-CEX0(PCA), GRIPPER_SMOT */

   PCA0CPL1 = (CEX1_Wrist_Compare_Value & 0x00FF);       /* 64036 = Neutral Position */
   PCA0CPH1 = (CEX1_Wrist_Compare_Value & 0xFF00)>>8;    /* P3.1-CEX1(PCA), WRIST_SMOT */

   PCA0CPL2 = (CEX2_Hand_Compare_Value & 0x00FF);        /* 64036 = Neutral Position */
   PCA0CPH2 = (CEX2_Hand_Compare_Value & 0xFF00)>>8;     /* P3.2-CEX2(PCA), HAND_SMOT */

   PCA0CPL3 = (CEX3_Elbow_Compare_Value & 0x00FF);       /* 64036 = Neutral Position */
   PCA0CPH3 = (CEX3_Elbow_Compare_Value & 0xFF00)>>8;    /* P3.3-CEX3(PCA), ELBOW_SMOT */

   PCA0CPL4 = (CEX4_Shoulder_Compare_Value & 0x00FF);    /* 64036 = Neutral Position */
   PCA0CPH4 = (CEX4_Shoulder_Compare_Value & 0xFF00)>>8; /* P3.4-CEX4(PCA), SHOULDER_SMOT */


   /* Load PCA0L,H with 65536-20,000 = 45536) */
   pca0_start_val = PCA0_BASE_VAL;

   /* Initialize PCA0 Counter Value to 45536 (0xB1E0) */
   PCA0L = (pca0_start_val & 0x00FF);                    /* Generate 50Hz, 20mSec PWM Period */
   PCA0H = (pca0_start_val & 0xFF00) >> 8;

   EIE1 |= 0x10;                                         /* EPCA0 = 1 -> Enable PCA interrupts */

   CR = 1;                                               /* run PCA counter */

}


/**
  * @brief
  * @param  None
  * @retval None
  * @note    This is the ISR for the PCA. It handles the case when a match occurs
  *          on channel 4 and updates the PCA0CP4 compare registers with the
  *          value held in the global variables "CEX4_FAN_Compare_Value. ".
  *****************************************************************************
  */
INTERRUPT(PCA0_ISR, PCA0_IRQn)

//void PCA0_ISR (void) interrupt 11
{

   if (CCF0)                           // If Module 0 caused the interrupt
   {
     CCF0 = 0;                         // Clear module 0 interrupt flag.

     PCA0CPL0 = (CEX0_Gripper_Compare_Value & 0x00FF);
     PCA0CPH0 = (CEX0_Gripper_Compare_Value & 0xFF00)>>8;
   }

   else if (CCF1)                     // If Module 1 caused the interrupt
   {
     CCF1 = 0;                        // Clear module 1 interrupt flag.

     PCA0CPL1 = (CEX1_Wrist_Compare_Value & 0x00FF);
     PCA0CPH1 = (CEX1_Wrist_Compare_Value & 0xFF00)>>8;
   }

   else if (CCF2)                     // If Module 2 caused the interrupt
   {
     CCF2 = 0;                        // Clear module 2 interrupt flag.

     PCA0CPL2 = (CEX2_Hand_Compare_Value & 0x00FF);
     PCA0CPH2 = (CEX2_Hand_Compare_Value & 0xFF00)>>8;
   }

   else if (CCF3)                     // If Module 3 caused the interrupt
   {
     CCF3 = 0;                        // Clear module 3 interrupt flag.

     PCA0CPL3 = (CEX3_Elbow_Compare_Value & 0x00FF);
     PCA0CPH3 = (CEX3_Elbow_Compare_Value & 0xFF00)>>8;
   }

   else if (CCF4)                     // If Module 4 caused the interrupt
   {
     CCF4 = 0;                        // Clear module 4 interrupt flag.

     PCA0CPL3 = (CEX4_Shoulder_Compare_Value & 0x00FF);
     PCA0CPH3 = (CEX4_Shoulder_Compare_Value & 0xFF00)>>8;
   }

   else                               // Interrupt was caused by other bits.
   {
      PCA0CN &= ~0x9F;                // Clear other interrupt flags for PCA
   }                                  // Stop counter; clear all flags
                                      // D7  6   5    4     3     2     1     D0
                                      // CF  CR  --  CCF4  CCF3  CCF2  CCF1  CCF0
                                      //  0   0   0    0     0     0     0     0
}


/**
  * @brief
  * @param  uint16 speed  requested speed to drive the motor.
  * @retval None
  * @note   Sets the motor to be driven at the requested speed.
  *         speed = 65536 produces the maximum speed while speed = 0 is full brake.
  */

void F340FanSpeed(uint16 LvFanRPM)
{
  uint16 Speed;

  Speed = LvFanRPM;

  // Set the PWM duty cycle to match that of the requested speed
  if(Speed == 0)
  {
     PCA0CPM4 &= ~0x40;  /* To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn */
                         /* register should be cleared to 0.  This prevents the PCA0CPLn */
						 /* match from occurring,which results in the pin never being set high.*/
  }
  else
  {
    // previously requested speed being zero
    PCA0CPM4 = 0x42;    /* Enable Comparator function and PWM signal is output on CEX4 pin */

    //set the pulse width to the requested value
    PCA0CPH4 = Speed;

    CR = 1;            /* Start PCA counter */
  }
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
