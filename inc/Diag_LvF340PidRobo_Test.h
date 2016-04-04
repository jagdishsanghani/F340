/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Test.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Test.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_TEST_H
#define __LVF340_PID_ROBO_TEST_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup TEST
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const char  *DacDataLabel;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uchar TickLedTest(void);
uchar SwitcheStatusTest(void);
uchar DigIOTest(void);
uchar SpeakerTest(void);
uchar AdcTest(void);
uchar PotTest(void);
uchar HallSensorTest(void);
uchar PhotocellTest(void);
uchar DacAllTest(void);
uchar RtcDS1338Test(void);
uchar FanDCmotorTest(void);
uchar ServoMotorsTest(void);
uchar SPIbusDevicesTest(void);
uchar LcdTest(void);
uchar EEpromTest(void);

#endif /* __LVF340_PID_ROBO_TEST_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
