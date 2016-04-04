/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_VT220.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_VT220.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_VT220_H
#define __LVF340_PID_ROBO_VT220_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup VT220
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern char code *CrLf;         /* VT100- code for carriage return & line feed */
extern char code *ResetCursor;  /* VT100- code for set cursor to Home */
extern char code *ClearScreen;  /* VT100- code for to clear entire screen */

/* Exported macro ------------------------------------------------------------*/
#define  In_DELETE		0x7F		/* ASCII <DEL>  */
#define  In_ESC			0x1B		/* ASCII ESCAPE */
#define  In_CR			0x0D		/* ASCII Carriage Return */
#define  In_BACKSPACE	'\x8'		/* ASCII BACK_SPACE */

#define  Out_BACKSPACE	'\x8'		/* ASCII BACK_SPACE */
#define  Out_SPACE		'\x20'		/* ASCII SPACE */
#define  Out_DELETE		"\x8 \x8"	/* Back Space twice */
#define  EOL		    '\r'        /* ASCII End of Line */
#define  Out_CR			0x0D		/* ASCII Carriage Return */
#define  LF              '\n'		/* ASCII Line Feed */

/* Exported functions ------------------------------------------------------- */
void reset_cursor(void);
void clear_screen(void);
void ClrScrRstCur(void);
void CarrRtnLineFeed(void);

#endif /* __LVF340_PID_ROBO_VT220_H */

/**
  * @}
  */

/**
  * @}
  */


/*****************************END OF FILE**************************************/
