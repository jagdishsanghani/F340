/**
  ******************************************************************************
  * @file    Diag_F340_Test.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Test module for testing various IO.
  *          This file provides firmware functions to manage the following
  *          functionalities of various IO:
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup BSP
  * @{
  */
/** @addtogroup TEST
  * @ TEST modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
char code * code space = "                        ";  /* print space */

char code * code Pot_mVolt = "mVolt = ";
char code * code Temp_degC = "    Temp_degC = ";

    /*  Hall Sensor output:                    DecimalVal  Hall_mV    PV%    Gauss   Pole */
char code * code HallSensorSPole = "\x1b[2;04H Gauss(South Pole)= DecVal  Hall_mV    PV%   Guass \n\r";
char code * code HallSensorNPole = "\x1b[2;04H Gauss(North Pole)= DecVal  Hall_mV    PV%   Guass \n\r";
char code * code HallSensor__ = "\x1b[2;04H Gauss(--------)= DecVal  Hall_mV    PV%";


    /*  Photocell output:                DecimalVal  Photocell_mV    PhotoPV%    milliLux  */
char code * code Photocell = "\x1b[02;04HDecimalVal  Photocell_mV    PhotoPV%    milliLux";

/******** LCD **********/
char code *code welcome = "WELCOME";
char code *code msurobotic = "MSU ROBOTIC";
char code * code Lcd = "\x1b[02;04H LCD Test displaying CPU Temperature";

char code *code TempSensor_mV = "mVolt = ";
char code *code Temperature = "Temp in DegC";

/* DAC7614 */
const char  *DacDataLabel = "\x1b[10;10HDacValue = ";

/* Private variables ---------------------------------------------------------*/
uint16 Val;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Print24LC1025Menu(void);
uchar EEpromTest(void);

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar AdcTest(void)
{
  unsigned int TempDegC;
  unsigned int PotVoltage;

  while(1)
  {
    CarrRtnLineFeed();

    PotVoltage = ResistorPot();
    printf(Pot_mVolt);            /* Print "Potentiometer = " */
    printf("%d04  ",PotVoltage);  /* Print value */

    TempDegC = TempSensor();
    printf(Temp_degC);            /* Print "Temp_degC = = " */
    printf("%d04  ",TempDegC);    /* Print value */

    // Check user wants to exit the test
    DisplayTimer = DELAY_50MSEC;  /* delay for 50 msec */
    while(DisplayTimer)
    {
      if(RxData0_Avail)            /* check for kb input for Escape character */
      {
        if(ReceiveData==In_ESC)
        {
          RxData0_Avail=0;
          return(0);              /* exit the test */
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar PotTest(void)
{
  uint16 PotVal;

  while(1)
  {
    PotVal = ResistorPot();            /* determine the trimpot position */

    printf("\x1b[4;1HPot_Value = ");   /* Print "Pot Value = " */
    printf( "%d04  ",PotVal);          /* Print temperature_sensor */

    // Check user wants to exit the test
    DisplayTimer = DELAY_50MSEC;  /* delay for 50 msec */
    while(DisplayTimer)
    {
      if(RxData0_Avail)            /* check for kb input for Escape character */
      {
        if(ReceiveData==In_ESC)
        {
          RxData0_Avail=0;
          return(0);              /* exit the test */
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar HallSensorTest(void)
{
  ulong32 HallSenVal;
  int16 DecVal;
  int16 Gauss;
  uint16 Hall_mV;

  ClrScrRstCur();

  while(1)
  {
    HallSenVal = ReadHallSensor();
    DecVal = (uint16)HallSenVal;        /* Get binary integer value int16 */

    Gauss = HallSenMagField((int16) HallSenVal);  /* Convert into Gauss */

    /**************************************************************/
    /*  ADC 2400mV is equal to 5000mV Hall Sensor output          */
    /*                                                            */
    /*                     Vref (mV)  * result (bits)             */
    /*  Hall_mV  =   ----------------------------------           */
    /*                         (2^10)-1 (bits)                    */
    /*                                                            */
    /*  Hall_PV%  =  (HallAccumulator * 10000) / ADC_SPAN         */
    /*                                                            */
    /*  Hall_Gauss =(HallCompensated * ONE_STEP_MILLIGAUSS/1000)  */
    /*                                                            */
    /*  ADC output:  Decimal    ADC_mV    Hall_mV    Gauss        */
    /* ---------------------------------------------------------  */
    /*                1023       2400      5000       2000        */
    /*                                                            */
    /**************************************************************/

    Hall_mV = (uint16)((HallSenVal * VREF_mV)/ADC_SPAN );  /* value in mVolts */

    Hall_PV_Pct = (uint16)((HallSenVal * 10000)/ADC_SPAN); /* process value in % */

    if (Gauss > 0)     printf(HallSensorSPole);
    else if(Gauss < 0) printf(HallSensorNPole);
    else               printf(HallSensor__);

    printf(space);
    printf("%d04    %d04     %d04  %d04", DecVal, Hall_mV, Hall_PV_Pct, Gauss );

    // Check user wants to exit the test
    DisplayTimer = DELAY_50MSEC;  /* delay for 50 msec */
    while(DisplayTimer)
    {
      if(RxData0_Avail)            /* check for kb input for Escape character */
      {
        if(ReceiveData==In_ESC)
        {
          RxData0_Avail=0;
          return(0);              /* exit the test */
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  **************************************************************
  *  ADC 2400mV is equal to 5000mV Photocell output            *
  *                                                            *
  *                     Vref (mV) * result (bits)              *
  *  Photocell_mV  =   ----------------------------            *
  *                          (2^10)-1 (bits)                   *
  *                                                            *
  *  Photocell_PV%  =  (PhotocellVal * 10000) / ADC_SPAN       *
  *                                                            *
  *  Photocell_Lux =(PhotocellCompensated * ONE_STEP_LUX/1000) *
  *                                                            *
  *  ADC output:  Decimal    ADC_mV    Photocell_mV    Lux     *
  * ---------------------------------------------------------  *
  *                1023       2400        5000        2000     *
  *                                                            *
  **************************************************************
  * @retval
  */
uchar PhotocellTest(void)
{
  ulong32 PhotocellVal;
  uint16  Photocell_mV;
  uint16  milliLux;

  ClrScrRstCur();

  CalibrateMinMaxPhotocellValues();

  while(1)
  {
    PhotocellVal = ReadPhotocell();
    milliLux = milliLuxPhotocell();

    Photocell_mV = (uint16)((PhotocellVal * VREF_mV)/ADC_SPAN );  /* value in mVolts */

    Photocell_PV_Pct = (uint16)((PhotocellVal * 10000)/ADC_SPAN); /* process value in % */

    printf(Photocell);          /* Photocell = "\x1b[10;10HPhotocell_mV  PhotoPV%  Lux" */

    printf("%d04  %d04    %d04  %d04", PhotocellVal, Photocell_mV, Photocell_PV_Pct, milliLux);

    // Check user wants to exit the test
    DisplayTimer = DELAY_50MSEC;  /* delay for 50 msec */
    while(DisplayTimer)
    {
      if(RxData0_Avail)            /* check for kb input for Escape character */
      {
        if(ReceiveData==In_ESC)
        {
          RxData0_Avail=0;
          return(0);              /* exit the test */
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar TickLedTest(void)
{
  PrintEscape();

  while(1)
  {
    TICK_LED = ~TICK_LED;     /* Change state of LED */

    LedDelay = 20;
    while(LedDelay)
    {
      if(RxData0_Avail)            /* check for kb input for Escape character */
      {
        if(ReceiveData==In_ESC)
        {
      	  RxData0_Avail=0;
          return(0);              /* exit the test */
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar SwitcheStatusTest (void)
{
  uchar Pattern;

  ClrScrRstCur();

  while(1)
  {
    reset_cursor();

    Pattern = P1;
    Pattern = Pattern & 0xDC;     /* 1 1 0 1 - 1 1 0 0 */

    printf("\x1b[4;1H SW2 SW3 SW4 SW5 SW6 = ");
    HexToBit(Pattern);

    /* Check user wants to exit the test */
    DisplayTimer = 20;             /* delay for 20 msec */
    while(DisplayTimer)
    {
      if(RxData0_Avail)            /* check for kb input for Escape character */
      {
        if(ReceiveData==In_ESC)
        {
          RxData0_Avail=0;
          return(0);               /* exit the test */
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar DigIOTest(void)
{
  uchar Pattern;

  ClrScrRstCur();

  while(1)
  {
    reset_cursor();

    Pattern = P2;
    Pattern = Pattern & 0xC0;     /* 1 1 0 0 - 0 0 0 0 */

    printf("\x1b[4;1HP2.6, P2.7 status = ");
    HexToBit(Pattern);

    WriteRelays(RELAY1, ON);
    msecDelay(DELAY_500MSEC);
    WriteRelays(RELAY1, OFF);

    WriteRelays(RELAY2, ON);
    msecDelay(DELAY_500MSEC);
    WriteRelays(RELAY2, OFF);

    /* Check user wants to exit the test */
    DisplayTimer = 20;            /* delay for 20 msec */
    while(DisplayTimer)
    {
      if(RxData0_Avail)            /* check for kb input for Escape character */
      {
        if(ReceiveData==In_ESC)
        {
          RxData0_Avail=0;
          return(0);              /* exit the test */
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar SpeakerTest(void)
{
  PrintEscape();

  printf("\x1b[2;04H Playing Melody");

  TMR2RL = TIMER2_RELOAD_10kHz;   /* Set Timer2 for 10kHz */
  TMR2 = TMR2RL;

  while(1)
  {
    PlayNote = ON;
    BUZZER = ON;

    PlayWelcomeTune();
    PlayTune();

    PlayNote = OFF;               /* stop tune */
    BUZZER = OFF;

    if(RxData0_Avail)              /* check for kb input for Escape character */
    {
      if(ReceiveData==In_ESC)
      {
        RxData0_Avail=0;

        TMR2RL = TIMER2_RELOAD_1000Hz;  /* Back to 1kHz */
        TMR2 = TMR2RL;

        return(0);
      }
    }
  }
}


/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar FanDCmotorTest(void)
{
  while(1)
  {
    F340FanSpeed(2000);

	DelayTimer = DELAY_200MSEC;

    while(DelayTimer)
    {						      /* send "hh:mm:ss"	*/
      if(RxData0_Avail)
      {
        if(ReceiveData==In_ESC)	  /* ESC ?  */
        {
        	RxData0_Avail = 0;		  /* Yes, exit */
          return(0);
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar ServoMotorsTest(void)
{
  while(1)
  {
    DelayTimer = DELAY_200MSEC;
    while(DelayTimer)
    {						      /* send "hh:mm:ss"	*/
      if(RxData0_Avail)
      {
        if(ReceiveData==In_ESC)	  /* ESC ?  */
        {
        	RxData0_Avail = 0;		  /* Yes, exit */
          return(0);
        }
      }
    }
  }
}

/**
  * @brief
  * @param
  * @note
  * @retval
  */
uchar LcdTest(void)
{
  int CpuTemp;

  PrintEscape();
  printf("\x1b[4;1H LCD Test displaying CPU Temperature");   /* Print "Pot Value = " */

  /* Display on LCD */
  print_string (welcome, 0x05, 0x00, YES);
  print_string (msurobotic, 0x02, 0x01, YES);

  while(1)
  {
    CpuTemp = TempSensor();

    print_string (Temperature, 0x03, 0x00, YES);
    print_integer (CpuTemp, 0x06, 0x01, YES);

    /* Check user wants to exit the test */
    DisplayTimer = 20;            /* delay for 20 msec */
    while(DisplayTimer)
    {
      if(RxData0_Avail)            /* check for kb input for Escape character */
      {
        if(ReceiveData==In_ESC)
        {
          RxData0_Avail=0;
          return(0);              /* exit the test */
        }
      }
    }
  }
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
uchar RtcDS1338Test(void)
{
  uchar State;                         /* variable for switch statement */

  ClrScrRstCur();

  while(1)
  {
    PrintDS1338Menu();

    State=getche();
    State = ToUpper(State);           /* if it is lower then convert to upper */

    switch(State)
    {
      case '1':
          SetDS1338Time();             /* Set time */
          break;

      case '2':
          SetDS1338Date();             /* Set date  */
          break;

      case '3':
          DisplayDS1338TimeAndDate();  /* Display time and date */
          break;

      default:
          break;

    }/* end of switch() */
  }/* end of while(1) */
  return(0);
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
uchar SPIbusDevicesTest(void)
{
  char  State;

  ClrScrRstCur();
  CarrRtnLineFeed();

  while(1)
  {
    PrintSpiDevicesMenu();

     State=getche();
     State = ToUpper(State);           /* if it is lower then convert to upper */

    switch(State)
    {
      case '1':
          PrintDac7614Menu();
          DacAllTest();
          break;

      case '2':
          PrintProgUnderDev();
          break;

      case '3':
          PrintProgUnderDev();
          break;

      default:
         break;
    }
  }
  return(0);
}

/**
  * @brief
  * @param  None
  * @retval None
  */
uchar EEpromTest(void)
{
  char  ch;

  for(;;)
  {
    Print24LC1025Menu();

    ch=getche();

    switch(ch)
    {
      case '1':
        Read24LC1025Eeprom();

        break;

      case '2':
        Write24LC1025Eeprom();

        break;

      case '3':
        Erase24LC1025Eeprom();

        break;

      default:
        if(ch==0x1b)
        {
          return(0);
        }

        break;
    }
  }
}



/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
