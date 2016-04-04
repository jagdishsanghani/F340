/**
  ******************************************************************************
  * @file    Diag_F340_Uart.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   UART0 module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of UART0 peripheral:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup UART
  * @ UART driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BR9600     9600      /* 12000000/9600/2/12 = 52 -->(-52) --> 0xCC  */
#define BR19200   19200      /* 12000000/19200/2/12 = 26 -->(-26) --> 0xE6 */
#define BR38400   38400      /* 12000000/38400/2/12 = 13 -->(-13) --> 0xF3 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bit  TxBuff0_Avail;         /* indicates empty tx buffer */
bit  RxData0_Avail;         /* indicates Received data is available in buffer */
bit  EscFlag;               /* indicates escape key is pressed */
data ReceiveData;           /* Buffer for UART_0 Rx Data buffer */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
  * @param  None
  * @retval None
  */

void UART0_Init (void)
{
   SCON0 = 0x10;    /* SCON0: 8-bit variable bit rate,  level of STOP bit is ignored */
                    /* RX enabled, ninth bits are zeros, clear RI0 and TI0 bits      */


   TH1 = -(SYSCLK/BR9600/2/12);  /* 12000000/9600/2/12 = 52 -->(-52) --> 0xCC */

                           /*   CKCON REGISTER  */
   CKCON = 0x00;           /*  D7    D6    D5    D4      D3   D2    D1    D0  */
                           /* T3MH  T3ML  T2MH  T2ML --  T1M  T0M  SCA1  SCA0 */
                           /*   0     0     0     0       0    0    0     0   */
                           /* SYSCLK/12 --> 12000000/12= 1MHz clock           */
						   /* Timer1 reload value ( Table 18.1)= 0x98 for 9600 BaudRate */
						   /* Timer0 and Timer1 uses 1MHz clock define by  SCA1:0 */

   TL1 = TH1;              /* init Timer1 */

   TMOD &= ~0xF0;          /* TMOD: timer 1 in 8-bit autoreload */
   TMOD |=  0x20;

   TR1 = 1;                /* START Timer1 TCON REGISTER                */
                           /* D7   D6   D5   D4      D3   D2   D1   D0  */
                           /* TF1  TR1  TF0  TR0 --  IE1  IT1  IE0  IT0 */
                           /*  0    1    0    0       0    0    0    0  */
                           /* Timer1 enabled                            */

   TxBuff0_Avail = 1;      /* Flag showing that UART0 can transmit */

   IP |= 0x10;             /* Make UART high priority */
   ES0 = 1;                /* Enable UART0 interrupts */

}

/**
  * @brief   This routine is invoked whenever a character is entered or displayed
  *          on the Hyperterminal.
  * @param  None
  * @retval None
  */
INTERRUPT(UART0_Interrupt, UART0_IRQn)
//void UART0_Interrupt (void) interrupt 4
{
   if (RI0 == 1)                /* Check if receive flag is set */
   {
      RI0 = 0;                  /* Clear interrupt flag        */
      ReceiveData = SBUF0;      /* Read a character from UART0 */
	  RxData0_Avail = 1;        /* Flag that Rx Data available */
   }

   if (TI0 == 1)                /* Check if transmit flag is set */
   {
      TI0 = 0;                  /* Clear interrupt flag */
      TxBuff0_Avail = 1;        /* Indicate transmission complete */
   }
}

/*****************************END OF FILE**************************************/
