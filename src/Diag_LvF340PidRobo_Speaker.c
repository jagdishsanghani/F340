/**
  ******************************************************************************
  * @file    Diag_F340_Speaker.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Speaker module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of Speaker:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup SPEAKER
  * @ SPEAKER modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
// key at 20kHz interrupt service routine Timer2
#define SILENT_NOTE   255  // 00    Hz
// key at 10000Hz
#define MELODY_LENGTH 95
#define SILENT_NOTE  255  /* 00 Hz  */

// key at 20kHz interrupt service routine Timer2
#define SILENT_NOTE   255  // 00  Hz

#define C4         76    //  263  Hz
#define C_SHARP4   72    //  278  Hz  Db4
#define D4         68    //  294  Hz
#define D_SHARP4   64    //  313  Hz  Eb4
#define E4         61    //  328  Hz
#define F4         57    //  351  Hz
#define F_SHARP4   54    //  370  Hz
#define G4         51    //  392  Hz
#define G_SHARP4   48    //  417  Hz
#define A4         45    //  444  Hz
#define A_SHARP4   43    //  465  Hz
#define B4         41    //  488  Hz

#define C5         38    //  526  Hz
#define C_SHARP5   36    //  556  Hz
#define D5         34    //  588  Hz
#define D_SHARP5   32    //  625  Hz
#define E5         30    //  667  Hz
#define F5         29    //  690  Hz
#define F_SHARP5   27    //  740  Hz

#define G5         26    //  769  Hz
#define G_SHARP5   24    //  833  Hz
#define A5         23    //  870  Hz
#define A_SHARP5   22    //  909  Hz
#define B5         20    // 1000  Hz

#define C6         19    // 1053  Hz
#define C_SHARP6   18    // 1111  Hz

#define D6         17    // 1175  Hz
#define D_SHARP6   16    // 1250  Hz
#define E6         15    // 1333  Hz
#define F6         14    // 1429  Hz

#define G6         13    // 1538  Hz
#define G_SHARP6   12    // 1667  Hz
#define A6         11    // 1818  Hz
#define B6         10    // 2000  Hz

#define C7          9    // 2222  Hz
#define D_SHARP7    8    // 2500  Hz
#define F7          7    // 2857  Hz
#define G_SHARP7    6    // 3333  Hz
#define B7          5    // 4000  Hz

#define D_SHARP8    4    // 5000  Hz

const unsigned char code note[MELODY_LENGTH] = 
{
  E5, SILENT_NOTE, E5, SILENT_NOTE, E5, SILENT_NOTE,

  C5, E5, G5, SILENT_NOTE, G4, SILENT_NOTE,
  
  C5, G4, SILENT_NOTE, E4, A4, B4, A_SHARP4, A4, G4,
  E5, G5, A5, F5, G5, SILENT_NOTE, E5, C5, D5, B4,

  C5, G4, SILENT_NOTE, E4, A4, B4, A_SHARP4, A4,G4,
  E5, G5, A5, F5, G5, SILENT_NOTE, E5, C5, D5, B4,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  G_SHARP4, A4, C5, SILENT_NOTE, A4, C5, D5,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  C6, SILENT_NOTE, C6, SILENT_NOTE, C6,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  G_SHARP4, A4, C5, SILENT_NOTE, A4, C5, D5,

  SILENT_NOTE, D_SHARP5, SILENT_NOTE, D5, C5

};

const unsigned int code duration[MELODY_LENGTH] =
{
  100, 25, 125, 125, 125, 125, 125, 250, 250, 250, 250, 250,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 125, 125, 125, 250, 125, 125, 200, 50, 100, 25, 500,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 250, 125, 375, 500
};

const unsigned char code WelcomeTune[] = { C4, D4, G4 };

/* Private variables ---------------------------------------------------------*/
uint16 TimeOut;       /* timeout for note */
uint16 OnTime;
uint16 OffTime;

uint16 OnTimer;
uint16 OffTimer;

bit PlayNote;
bit Playing;

uchar TempBuffer;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Set up timer 2 to play the desired frequency (in Hz) and for the the
  *         desired duration (in ms). Allowed frequencies are from 40 Hz to 5 kHz.
  * @param  None
  * @retval None
  */
void PlayFrequency(uint16 freq, uint16 dur)
{
  if(freq == 255)
  {
    OnTime = 0;              /* ON time of period */
	OffTime = 0;             /* OFF time of period */
	TimeOut = dur * 10;      /* set timeout (duration); multiply by 10 if 10kHz */
  }
  else
  {
    OnTime = freq / 2;
	OffTime = freq - OnTime;
	TimeOut = dur * 10;
  }

  OnTimer = OnTime * 10;     /* multiply by 10 if 10kHz */
  OffTimer = OffTime * 10;   /* multiply by 10 if 10kHz */
}

/**
  * @brief  This tune will be play at the end of the program
  * @param  None
  * @retval None
  */
void PlayTune(void)
{
  unsigned char i;
  unsigned int freq;
  unsigned int dur;

  PlayNote = ON;   /* start tune */

  // SET TIMER2 FREQUENCY TO 10000Hz
  TMR2RL = TIMER2_RELOAD_10kHz;
  TMR2 = TMR2RL;             /* Change Timer2 interrupt setting to 10kHz */

  for ( i = 0; i < MELODY_LENGTH; i++)
  {
    freq = note[i];
    dur = duration[i];
	PlayFrequency(freq, dur);	    
	while(TimeOut){ /* wait */};
  }
  PlayNote = OFF; /* stop tune */

  BUZZER = OFF;

  // SET TIMER2 FREQUENCY TO 1000Hz
  TMR2RL = TIMER2_RELOAD_1000Hz;
  TMR2 = TMR2RL;             /* Change Timer2 interrupt setting back to 1kHz */

}

/**
  * @brief  This welcome tune will be play on power-up.
  * @param  None
  * @retval None
  */
void PlayWelcomeTune(void)
{
  uchar i;
  uint16 freq;
  uint16 dur;
  uchar WelcomeTuneLength = sizeof(WelcomeTune);

  /* start tune */
  PlayNote = ON;

  /* SET TIMER2 FREQUENCY TO 10000Hz */
  TMR2RL = TIMER2_RELOAD_10kHz;
  TMR2 = TMR2RL;

  for ( i = 0; i < WelcomeTuneLength; i++)
  {
    freq = WelcomeTune[i];
    dur = 100;

	PlayFrequency(freq, dur);	    
	while(TimeOut)
	{
      /* wait */
	}
  }
  PlayNote = OFF; /* stop tune */

  BUZZER = OFF;

  /* SET TIMER2 FREQUENCY TO 1000Hz */
  TMR2RL = TIMER2_RELOAD_1000Hz;
  TMR2 = TMR2RL;
}


/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/


