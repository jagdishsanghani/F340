/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_SpiBus.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_SpiBus.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_SPIBUS_H
#define __LVF340_PID_ROBO_SPIBUS_H

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup SPIBus
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Instruction Set */
#define  SPI_WRITE         0x04        /* Send a byte from the Master to the Slave */
#define  SPI_READ          0x08        /* Send a byte from the Slave to the Master */
#define  SPI_WRITE_BUFFER  0x10        /* Send a series of bytes from the Master to the Slave */
#define  SPI_READ_BUFFER   0x20        /* Send a series of bytes from the Slave to the Master */
#define  ERROR_OCCURRED    0x40        /* Indicator for an error occurred */

#define  SPI_WRITE_CMD            0x01        /* Send a command byte from the Master to the Slave */
#define  SPI_WRITE_ADDR           0x02        /* Send address byte from the Master to the Slave */
#define  SPI_CMD_READ             0x03   /* Read Command  */
#define  SPI_CMD_WRITE            0x02   /* Write Command  */

#define  SPI_SEND_DAC_LSB         0x20        /* Spi DAC7614 lsb */
#define  SPI_DAC_XFER             0x30        /* Spi command state for DAC7614 LDAC */


/* SPI DEVICE ID */
#define  EEP25LC1024_ID   0x20   /*  0  0  1  0 - 0  0  0   0   */
#define  EXT_DEVICE_ID    0x80   /*  1  0  0  0 - 0  0  0   0   */

#define  DAC7614_ID       0x40   /*  0  1  0  0 - 0  0  0   0   */
                                 /* D7 D6 D5 D4 - D3 D2 D1 D0   */
#define  DAC7614_CH0      0x4A   /*  0  1  0  0 -  1  0  1  0   */
#define  DAC7614_CH1      0x4B   /*  0  1  0  0 -  1  0  1  1   */
#define  DAC7614_CH2      0x4C   /*  0  1  0  0 -  1  1  0  0   */
#define  DAC7614_CH3      0x4D   /*  0  1  0  0 -  1  1  0  1   */
#define  DAC7614ALL       0x4F   /*  0  1  0  0 -  1  1  1  1   */

#define  SPIDAC_MASK      0xF0   /* 1 1 1 1 - 0 0 0 0 */

#define  DAC_MSB_CH0        0x00   /* 0 0 0 0 - 0 0 0 0 */
#define  DAC_MSB_CH1        0x40   /* 0 1 0 0 - 0 0 0 0 */
#define  DAC_MSB_CH2        0x80   /* 1 0 0 0 - 0 0 0 0 */
#define  DAC_MSB_CH3        0xC0   /* 1 1 0 0 - 0 0 0 0 */

/* Exported variables --------------------------------------------------------*/
extern uchar data Dac7614Msb;     /* Buffer for MSB of DAC7614  */
extern uchar data Dac7614Lsb;     /* Buffer for LSB of DAC7614  */

/* Exported functions --------------------------------------------------------*/
void SpioInit(void);
void SpiBusWrite(uchar SpiDeviceID, ulong32 DestAddress, uchar *pDataStart, uchar Len);
void SpiBusRead(uchar DeviceID, uchar* pDestAddr, ulong32 SrcAddr, uchar Length);

void PrintSpiDevicesMenu(void);
void PrintDac7614Menu (void);
void Print25lc1024Menu (void);

#endif /* __LVF340_PID_ROBO_SPIBUS_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
