/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Photocell.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Photocell.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_PHOTOCELL_H
#define __LVF340_PID_ROBO_PHOTOCELL_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup PHOTOCELL
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Photocell values */
extern uint16  Photocell_Raw;      /* Measured in bits */
extern uint16  Photocell_mV;       /* Measured voltage in mV */
extern uint16  Photocell_PV_Pct;   /* Photo Conductive Cell sensor value in percentage */

extern uint16  PhotocellMaxValue;  /* the maximum value returned by this function  */

/* here we store calibrated values of photocell sensor. min & max  */
extern uint16  CalibratedPhotocellMin;
extern uint16  CalibratedPhotocellMax;

/* Exported macro ------------------------------------------------------------*/
/* Exported Photocell variables ----------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
uint16 ReadPhotocell (void);
uint16 milliLuxPhotocell(void);
void CalibrateMinMaxPhotocellValues(void);
void ResetPhotocellCalVal(void);

#endif /* __LVF340_PID_ROBO_PHOTOCELL_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
