/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_HallSensor.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_HallSensor.c module.
  *          This file contains the headers of Hall sensor.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_HALLSENSOR_H
#define __LVF340_PID_ROBO_HALLSENSOR_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup HALL_SENSOR
  * @{
  */


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported Hall sensor values -----------------------------------------------*/
extern uint16  Hall_Raw;           /* Measured in bits */
extern uint16  Hall_mV;            /* Measured voltage in mV */
extern uint16  Hall_PV_Pct;        /* Hall sensor process value in percentage */

extern uint16  HallMaxValue;       /* the maximum value returned by this function  */

/* here we store calibrated values of hall sensor. min & max  */
extern uint16  HallCalMin;
extern uint16  HallCalMax;

/* Exported functions ------------------------------------------------------- */
int16 ReadHallSensor (void);
int16 HallSenMagField(int16 RawHallVal);

#endif /* __LVF340_PID_ROBO_HALLSENSOR_H */

/**
  * @}
  */

/**
  * @}
  */
/*****************************END OF FILE**************************************/
