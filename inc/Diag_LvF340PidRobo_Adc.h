/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Adc.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Adc.c module.
  *          This file contains the headers of the interrupt handlers.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_ADC_H
#define __LVF340_PID_ROBO_ADC_H

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* c8051F340 Word Registers --------------------------------------------------*/
sfr16 ADC0     = 0xBD;   /* ADC0 result  */

/* Exported constants --------------------------------------------------------*/
#define  ADC_OFFSET         776   /* Temp. Sensor reading in millivolts at 0 deg C */
#define  ADC_SPAN          1023   /* ADC 10bit = 1023 */
#define  VREF_mV           2400   /* ADC Vref = 2.400V  */
#define  TEMP_GAIN          286   /* GAIN (V/ deg c) = 2.86 mV/deg C = (2.86 * 100)/ 100 */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------------*/
extern uchar  AdcAvail;
extern uint16 AdcValue;

/* Exported functions ------------------------------------------------------- */
void InitADC0 (void);
uint16 ResistorPot(void);
uint16 TempSensor (void);


#endif /* __LVF340_PID_ROBO_ADC_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
