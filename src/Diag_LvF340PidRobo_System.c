/**
  ******************************************************************************
  * @file    Diag_F340_System.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   System module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of System initialization:
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup SYSTEM
  * @ SYSTEM modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void InitPort (void);

/**
  * @brief  Initialize all system parameters.
  * @param  None
  * @retval None
  * @note
  */
void init_system (void)
{
  uchar i;               /* Temporary counter variable */
  bit error_flag = 0;    /* Flag for checking SMBus */

  PCA0MD &= ~0x40;   /* WDTE = 0 (clear watchdog timer enable) */

  /* Check I2C Bus */
  /* If slave is holding SDA low because of an improper SMBus reset or error */

  while(!SDA)
  {
    /* Provide clock pulses to allow the slave to advance out */
    /* of its current state. This will allow it to release SDA.*/
    XBR1 = 0x40;                     /* Enable Crossbar */
    SCL = 0;                         /* Drive the clock low */
    for(i = 0; i < 255; i++);        /* Hold the clock low */
    SCL = 1;                         /* Release the clock */
    while(!SCL);                     /* Wait for open-drain */
	                                 /* clock output to rise */
    for(i = 0; i < 10; i++);         /* Hold the clock high */
    XBR1 = 0x00;                     /* Disable Crossbar */
  }

  SYSCLK_Init ();      /* Initialize Oscillator for 12MHz */

  Timer0_Init();       /* Timer0 to generate SMBus Clock. */

  Timer2_Init();       /* Timer2 to generate interrupts at a 1000Hz rate. */

  Timer3_Init();       /* Timer3 for use with SMBus low timeout detect */

  InitPort ();

  InitSMBus();

  UART0_Init();        /* Initialize UART 0, BR = 9600 */

  InitADC0();

  SpioInit();

  InitPca0();

  HeartBeatTimer = 50; /*                */

  PCA0MD &= ~0x40;     /* WDTE = 0 (clear watchdog timer enable) */

  BUZZER = OFF;        /* Turn off the BUZZER before the test starts */
  PlayNote = OFF;

  HEART_BEAT = 1;

  EIE1 |= 0x01;        /* Enable the SMBus interrupt */

  LedDelay = 10;

   EA = 1;              /* Global interrupt enable */

   lcd_init();          /* Initialize LCD display */

   /* Indicate communication is good */
   if (error_flag == 0)
   {
	   TICK_LED = 1;     /* LED = ON indicates that the test passed */
   }
}

/******************************************************************************
  * @brief
  * @param  None
  * @retval None
  * @note
  * This function configures the crossbar and ports pins. To configure a
  * pin as a digital input, the pin is configured as digital and open-drain and the port
  * latch should be set to a '1'.  The weak-pullups are used to pull the pins high.
  * Pressing the switch pulls the pins low.
  *
  * To configure a pin as a digital output, the pin is configured as digital and push-pull.
  *
  * An output pin can also be configured to be an open-drain output if system requires it.
  * For example, if the pin is an output on a multi-device bus, it will probably be
  * configured as an open-drain output instead of a push-pull output.  For the purposes of
  * this example, the pin is configured as push-pull output because the pin in only
  * connected to an LED.
  *
  *  // P0.0  -  SCK  (SPI0), Push-Pull,  Digital
  *  // P0.1  -  MISO (SPI0), Open-Drain, Digital PULLUP RESISTOR 2K2
  *  // P0.2  -  MOSI (SPI0), Push-Pull,  Digital
  *  // P0.3  -  NSS  (SPI0), Push-Pull,  Digital
  *  // P0.4  -  TX0 (UART0), Push-Pull,  Digital
  *  // P0.5  -  RX0 (UART0), Open-Drain, Digital
  *  // P0.6  -  Skipped,     Open-Drain, Digital  X0
  *  // P0.7  -  Skipped,     Open-Drain, Digital  X1
  *
  *  // P1.0  -  SDA (SMBus), Open-Drain, Digital
  *  // P1.1  -  SCL (SMBus), Push-Pull,  Digital
  *  // P1.2  -  Skipped,     Open-Drain, Digital  ENTER-PB
  *  // P1.3  -  Skipped,     Open-Drain, Digital  LEFT-PB
  *  // P1.4  -  Skipped,     Open-Drain, Digital  UP-PB
  *  // P1.5  -  Skipped,     Open-Drain, Analog   VREF
  *  // P1.6  -  Skipped,     Open-Drain, Digital  DOWN-PB
  *  // P1.7  -  Skipped,     Open-Drain, Digital  RIGHT-PB
  *
  *  // P2.0  -  Skipped,     Push-Pull,  Digital  RELAY1
  *  // P2.1  -  Skipped,     Push-Pull,  Digital  RELAY2
  *  // P2.2  -  Skipped,     Push-Pull,  Digital  TICK-LED
  *  // P2.3  -  Skipped,     Push-Pull,  Digital  HB-LED
  *  // P2.4  -  Skipped,     Push-Pull,  Digital  SPEAKER
  *  // P2.5  -  Skipped,     Push-Pull,  Digital  LDAC
  *  // P2.6  -  Skipped,     Open-Drain, Digital  EXT-INP1
  *  // P2.7  -  Skipped,     Open-Drain, Digital  EXT-INP2
  *
  *  // P3.0  -  CEX0  (PCA), Push-Pull,  Digital  PWM0-SERVO1
  *  // P3.1  -  CEX1  (PCA), Push-Pull,  Digital  PWM1-SERVO2
  *  // P3.2  -  CEX2  (PCA), Push-Pull,  Digital  PWM2-SERVO3
  *  // P3.3  -  CEX3  (PCA), Push-Pull,  Digital  PWM3-SERVO4
  *  // P3.4  -  CEX4  (PCA), Push-Pull,  Digital  PWM4-DC-MOT
  *  // P3.5  -  Skipped,     Push-Pull,  Digital  BASE-MOT
  *  // P3.6  -  TX1 (UART1), Push-Pull,  Digital
  *  // P3.7  -  RX1 (UART1), Open-Drain, Digital
  *
  *  // P4.0  -  Unassigned,  Open-Drain, Analog  ADC1/POT
  *  // P4.1  -  Unassigned,  Open-Drain, Digital  NU
  *  // P4.2  -  Unassigned,  Open-Drain, Digital  NU
  *  // P4.3  -  Unassigned,  Open-Drain, Analog   ADC2/HALL
  *  // P4.4  -  Unassigned,  Open-Drain, Analog   ADC3/LDR
  *  // P4.5  -  Unassigned,  Open-Drain, Analog   ADC4/ANA-IP
  *  // P4.6  -  Unassigned,  Open-Drain, Digital  NU
  *  // P4.7  -  Unassigned,  Open-Drain, Digital  NU
  *
  *******************************************************************************
*/
void InitPort (void)
{
	P1MDIN    = 0xDF;    /* 1  1  0  1 - 1  1  1  1 */
	P4MDIN    = 0xC6;    /* 1  1  0  0 - 0  1  1  0 */

	P0MDOUT   = 0x1D;    /* 0  0  0  1 - 1  1  0  1 */
	P1MDOUT   = 0x02;    /* 0  0  0  0 - 0  0  1  0 */
	P2MDOUT   = 0x3F;    /* 0  0  1  1 - 1  1  1  1 */
	P3MDOUT   = 0x7F;    /* 0  1  1  1 - 1  1  1  1 */

	P0SKIP    = 0xC0;    /* 1  1  0  0 - 0  0  0  0 */
	P1SKIP    = 0xFC;    /* 1  1  1  1 - 1  1  0  0 */
	P2SKIP    = 0xFF;    /* 1  1  1  1 - 1  1  1  1 */
	P3SKIP    = 0x20;    /* 0  0  1  0 - 0  0  0  0 */

	XBR0      = 0x07;    /* 0  0  0  0 - 0  1  1  1 */
	XBR1      = 0x45;    /* 0  1  0  0 - 0  1  0  1 */
	XBR2      = 0x01;    /* 0  0  0  0 - 0  0  0  1 */
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/

