/**
  ******************************************************************************
  * @file    Diag_F340_VT220.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   VT200 module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of VT220 Terminal:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup VT220
  * @ VT220 modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char code *CrLf = "\r\n";            /* VT100- code for carriage return & line feed */
char code *ResetCursor = "\x1b[H";   /* VT100- code for set cursor to Home */
char code *ClearScreen = "\x1b[2J";  /* VT100- code for to clear entire screen */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Reset cursor to top left.
  * @param  None
  * @retval None
  * @note
  */
void reset_cursor(void)
{
 printf(ResetCursor);   /* escape sequence for vt220 ^[H sets cursor to Home */
}
						/* same thing can be written as printf("%s","\033[2H");  */

/**
  * @brief  Clear screen.
  * @param  None
  * @retval None
  * @note
  */
void clear_screen(void)
{
 printf(ClearScreen);   /* escape sequence for vt220 ESC[2J clears screen */
}

/**
  * @brief  Clear screen & Reset cursor
  * @param  None
  * @retval None
  * @note
  */
void ClrScrRstCur(void)
{
  clear_screen();
  reset_cursor();
}


/**
  * @brief  Carriage Return and Line Feed
  * @param  None
  * @retval None
  * @note
  */
void CarrRtnLineFeed(void)
{
    putchar('\r');	/* if not finished then do some more */
    putchar('\n');
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/




