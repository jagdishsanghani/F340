/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Pca.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Pca.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_PCA_H
#define __LVF340_PID_ROBO_PCA_H

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup PCA
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* PWM signal 20msec period, 50 Hz */
/* Period 19.5 msec OFF, 0.5 msec ON --> Minimum Pulse Width   0 degree */
/* Period 17.5 msec OFF, 2.5 msec ON --> Neutral Point        90 degree */
/* Period 18.5 msec OFF, 1.5 msec ON --> Maximum Pulse Width 180 degree */
#define CEXn_FREQUENCY        50       // PWM Frequency to output on CEXn (Hz)

#define DIVIDE_RATIO          (SYSCLK/12/CEXn_FREQUENCY) // number of cycles per interrupt
                                                         // 12,000,000/12/50 = 20,000

#define PCA0_BASE_VAL         45536    // 65536-20000 = 45536 for 20mSec Period

#define PCA0CPn_MIN_VAL       63036    // For 2.5 msec: 20 - 2.5 = 17.5 msec
                                       // 45536 + 17500 = 63036

#define PCA0CPn_MAX_VAL       65036    // For 0.5 msec: 20 - 0.5 = 19.5 msec
                                       // 45536 + 19500 = 65036

#define PCA0CPn_NEUTRAL_VAL   64036    // For 1.5 msec: 20 - 1.5 = 18.5 msec
                                       // 45536 + 18500 = 64036

// Initialize vale of various servo motors
#define GRIPPER_INIT_VALUE    61036   /* For 1.5 msec: 20 - 1.5 = 18.5 msec 0xEE6C */


#define TIMER3_MIN_ON_VAL         5    // 5 x 100uSec = 0.5mSec
#define TIMER3_MAX_ON_VAL        25    // 25 x 100uSec = 2.5mSec
#define TIMER3_PERIOD_VAL       200    // 200 x 100uSec = 20mSec; T=1/F

#define TIMER3_NEUTRAL_VAL       15    // 15 x 100uSec = 1.5mSec; T=1/F

// For Timer3 100usec Interrupt
#define TIMER3_AUTORL_100USEC_INTR  65336  // 65536-200=65336, 100usec (0.1msec) Interrupt

/* Exported variables --------------------------------------------------------*/
/* PAC0 PWM compare values */
extern uint16 CEX0_Gripper_Compare_Value;    /* Holds current PCACPL0 compare value */
extern uint16 CEX1_Wrist_Compare_Value;      /* Holds current PCACPL1 compare value */
extern uint16 CEX2_Hand_Compare_Value;       /* Holds current PCACPL2 compare value */
extern uint16 CEX3_Elbow_Compare_Value;      /* Holds current PCACPL3 compare value */
extern uint16 CEX4_Shoulder_Compare_Value;   /* Holds current PCACPL4 compare value */

extern uint16 CEX5_Base_Compare_Value;       /* Holds current T2 value */

extern bit base_motor_pwm;                   /* Flag for Base Motor PWM start */
extern bit base_motor_pwm_ontime;            /* Flag for Base Motor PWM on time */
extern bit base_motor_pwm_offtime;           /* Flag for Base Motor PWM off time */

/* Exported functions ------------------------------------------------------- */
void InitPca0 (void);
void F340FanSpeed(uint16 speed);


#endif /* __LVF340_PID_ROBO_PCA_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
