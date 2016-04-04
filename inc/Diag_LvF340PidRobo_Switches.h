/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Switches.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Switches.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_SWITCHES_H
#define __LVF340_PID_ROBO_SWITCHES_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup SWITCHES
  * @{
  */

/* Bit Definitions for F340PID-ROBO board------------------------------------*/
sbit LEFT_PB    = P1^2;      /* SW2 */
sbit UP_PB      = P1^3;      /* SW3 */
sbit DOWN_PB    = P1^4;      /* SW4 */
sbit RIGHT_PB   = P1^6;      /* SW5 */
sbit ENTER_PB   = P1^7;      /* SW6 */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Switches  */
#define LEFT_SW2       0x04   /* SW2 ='1' means switch pressed */
#define UP_SW3         0x08   /* SW3 ='1' means switch pressed */
#define DOWN_SW4       0x10   /* SW4 ='1' means switch pressed */
#define RIGHT_SW5      0x40   /* SW5 ='1' means switch pressed */
#define ENTER_SW6      0x80   /* SW6 ='1' means switch pressed */

#define ALL_SWITCHES   (LEFT_SW2 | UP_SW3 | DOWN_SW4 | RIGHT_SW5 | ENTER_SW6)
#define ANY_SWITCHES   ALL_SWITCHES  /* 1 1 0 1 - 1 1 0 0 -> 0xDC */
/* Exported functions ------------------------------------------------------- */
uchar SwitchIsPressed(uchar Switches);
uchar WaitForSwitchRelease(uchar Switches);

#endif /* __LVF340_PID_ROBO_SWITCHES_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
