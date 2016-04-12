/**
  ******************************************************************************
  * @file    Diag_F340_SpiBus.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   SPI Bus module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of SPI Bus peripheral:
  *          SPI bus clock 250kHz
  *          Pinout: For SPI Bus
  *          P0.0 - SPI SCK    (digital output, push-pull)
  *          P0.1 - SPI MISO   (digital input, open-drain)
  *          P0.2 - SPI MOSI   (digital output, push-pull)
  *          P0.3 - SPI NSS    (digital output, push-pull)
  *          |-----------------------------------------------------------------|
  *          |                       __     __                                 |
  *          |DAC7614_XFER      P2^5   |   |      load data to output  latch   |
  *          |                         |___|                                   |
  *          |-----------------------------------------------------------------|
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup SPIBus
  * @ SPI Bus driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  SPI_CLOCK           250000    /* Maximum SPI clock */
#define  MAX_BUFFER_SIZE     8         /* Maximum buffer Master will send */

#define  F_SCK_MAX           2000000   /* Max SCK freq (Hz) */
#define  T_NSS_DISABLE_MIN   500       /* Min NSS disable time (ns) */

#define  ERROR_OCCURRED      0x40      /* Indicator for the Slave to tell the */
                                       /* Master an error occurred */

/* Bit Definitions for F340PID-ROBO board-------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/***SPI Bus main menu *****/
char code * code SpiDevicesMenu[6] = {
    "  SPI Devices Menu\n\r\n",
    "1. Dac7614 Test\n\r",
    "2. E2Prom 25LC1024 Test\n\r",
    "3. Flash SST25VF032B Test\n\r",
    "Select option (1-3)",
    "\x1b[24;10HPress ESCAPE to exit\x1b[8;20H"
};
/* Private variables ---------------------------------------------------------*/
/*
*****************************************************************************************
                   SPI BUS INTERRUPT SERVICE, READ & WRITE PARAMETERS
*****************************************************************************************
*/

uchar SpiCommand = 0x00;   /* variable to store command byte */

uchar* pSPI_DATA_IN;       /* Global pointer for SPIBus data */
                           /* All receive data is written here */

uchar* pSPI_DATA_OUT;      /* Global pointer for SPIBus data. */
                           /* All transmit data is read from here */

uchar SPI_SINGLEBYTE_OUT;  /* Global holder for single byte writes. */

uchar SPI_DATA_LEN;        /* Global holder for number of bytes */
                           /* to send or receive in the current SPIBus transfer. */

ulong32 SPI_WORDADDR;      /* Global holder for the EEPROM word address that will */
                           /* be accessed in the next transfer */

uchar SPI_TARGET;          /* Target SPIBus slave address */

bit SPI_BUSY;              /* Software flag to indicate spi bus is buzy */

bit SPI_WR;                /* Software flag to indicate for write STATE */

bit SPI_SENDWORDADDR;      /* When set, this flag causes the ISR to send the */
                           /* 8-bit <WORD_ADDR> after sending the slave address. */

bit SPI_RANDOMREAD;        /* When set, this flag causes the ISR to send a START */
                           /* signal after sending the word address. The ISR handles */
                           /* this switch over if the <SMB_RANDOMREAD> bit is set. */

bit SPI_DAC;               /* DAC flag */

bit DAC7614All;            /* Flag for DAC all registers */

uchar data Dac7614Msb;     /* Buffer for MSB of DAC7614  */
uchar data Dac7614Lsb;     /* Buffer for LSB of DAC7614  */

bit Error_Flag = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief
  * @param  None
  * @retval None
  * @note   Configures SPI0 to use 4-wire Single Master mode. The SPI timing is
  *         configured for Mode 0,0 (data centered on first edge of clock phase and
  *         SCK line low in idle state).
  */
void SpioInit(void)
{
   SPI0CFG   = 0x70;                   /* Enable the SPI as a Master */
                                       /* CKPHA = '1', CKPOL = '1' */
   SPI0CN    = 0x0D;                   /* 4-wire Single Master, SPI enabled */

   /* SPI clock frequency equation from the data sheet */
   SPI0CKR   = (SYSCLK/(2 * SPI_CLOCK))-1;

   ESPI0 = 1;                          // Enable SPI interrupts
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note   Configures SPI0 to use 4-wire Single Master mode. The SPI timing is
  *         configured for Mode 0,0 (data centered on first edge of clock phase and
  *         SCK line low in idle state).
  */

/**
  *****************************************************************************
  *
  * @brief
  * @param  SpiDeviceID      Dac
  *
  * @retval None
  * Dest Address     For DAC, DestAddress is DAC(DAC1 to DAC4) and All DACs
  *                           For EEprom and Flash Memory, it is three bytes address
  *
  * Arg3: -   *pDataStart      Data for Device
  * Arg4: -   Len              Number of bytes to be sent to Slave device (Max = 256)
  *
  * Return(s): nothing
  *
  * Description: SPI DEVICE ID
  * DAC7614A_ID      0x3A     0  0  1  1 - 1  0  1   0
  * DAC7614B_ID      0x3B     0  0  1  1 - 1  0  1   1
  * DAC7614C_ID      0x3C     0  0  1  1 - 1  1  0   0
  * DAC7614D_ID      0x3D     0  0  1  1 - 1  1  0   1
  * DAC7614ALL_ID    0x3F     0  0  1  1 - 1  1  1   1
  * EXT_DEVICE_ID    0x40     0  1  0  0 - 0  0  0   0
  *   -------------------------------------------------------------------
  *  |             MSB                   |             LSB               |
  *   ------------------------------------|------------------------------
  *  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | - | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  *  |-------------------------------------------------------------------
  *  | X1| X0| 0 | 0 | D | D | D | D | - | D | D | D | D | D | D | D | D |
  *   -------------------------------------------------------------------
  * X -> Dac Number   D -> Dac Data (12 bit)
  *****************************************************************************
  */

/**
  * @brief
  * @param  None
  * @retval None
  * @note       : Interrupt Service Routines
  *             : Handles no error checks and single-byte writes.
  */

/**
  *****************************************************************************
  * Function name   : void SPI_ISR (void) interrupt 6
  *
  * Description     : Interrupt Service Routines
  *                : Handles all error checks and single-byte writes.
  *
  * Typical Write:
  *              ---------------------------------------------------------
  *              | 1st sent | 2nd sent | 3rd sent |   ...    | last sent |
  *              ---------------------------------------------------------
  *  Master NSSv | Command  |   Data1  |   Data2  |   ...    |   DataN   |  NSS^
  *  Slave       |   N/A    |    N/A   |    N/A   |   ...    |    N/A    |
  *              ---------------------------------------------------------
  *  Typical Read:
  *
  *              | 1st sent | 2nd sent | 3rd sent |   ...    | last sent |
  *              ---------------------------------------------------------
  *  Master NSSv | Command  |   dummy  |   dummy  |   ...    |   dummy   |  NSS^
  *  Slave       |   N/A    |   Data1  |   Data2  |   ...    |   DataN   |
  *              ---------------------------------------------------------
  *
  *  When the Master enters the ISR, the SPIF flag should be set from
  *  sending the Command byte.  This ISR handles the remaining steps of the
  *  SPI transfer process.
  *  <state> == 0: send command, send source or destination addresses for
  *             : writing or reading number of bytes of data.
  *             : for READ commands (first time, only a dummy byte is
  *             : sent but the second time, the data must be read from SPI0DAT)
  *  <state> == 1: Write or Read number of bytes to and from device.
  *             : NSSMD0 = 1 to end the transfer, final byte read or write.
  *             : Reset parameters.
  *  <state> == 2: Any errors in the Command parsing should go to state 2
  *             : where NSSMD0 is de-asserted.
  *  Note:       :This routine is tested for only EEPROM 25lc1024.
  *****************************************************************************
  */
void SPI_ISR (void) interrupt 6
{
  static uchar SpiState = 0;

  if (SpiState == 0)
  {
    switch (SpiCommand)
    {
      case SPI_WRITE_CMD:
           NSSMD0 = 1;                  /* Release the slave (not expecting data back) */
                                        /* Finish command for write enable latch */
           SpiCommand = SPI_WRITE_ADDR; /* set for Next case statement to send address */
           NSSMD0 = 0;                  /* Reselect device again for write command */
           SPI0DAT  = SPI_CMD_WRITE;    /* Send the WRITE command */

           break;                       /* exit to state 2 and clear SPIF flag */

      case SPI_WRITE_ADDR:
           /* add code here */
           break;  /* exit to state 2 and clear SPIF flag */

      case SPI_SEND_DAC_LSB:
           SPI0DAT = Dac7614Lsb;       /* Send lower byte of Dac7614 */
           SpiCommand = SPI_DAC_XFER;  /* next time go to SPI_DAC_XFER */
           SpiState = 1;               /* Advance to the next state */

           break;                      /* exit to state 2 and clear SPIF flag */

       default:
           SpiState = 2;               /* Any errors in the Command parsing should go to state 2 */
                                       /* where NSSMD0 is de-asserted */
    }
  }/* end of state 0 */

  else if (SpiState == 1)              /* This state is for READ commands where the data */
  {                                    /* must be read after the first dummy byte is sent */
    switch(SpiCommand)
    {
      case SPI_READ:
           /* add code here */
           break;/* exit to state 2 and clear SPIF flag */

      case SPI_WRITE:
          /* add code here */
           break;                      /* exit to state 2 and clear SPIF flag */

      case SPI_DAC_XFER:
           NSSMD0 = 1;                 /* De-select the Slave */
           SpiState = 0;               /* Reset the state */

           DAC7614_XFER = 1;
           DAC7614_XFER = 0;           /* Generate LDAC pulse */
           DAC7614_XFER = 1;

           break;                      /* exit to state 2 and clear SPIF flag */

           default:
              SpiState = 2;            /* Any errors in the Command parsing should go to state 2 */
    }
  }/* end of state 1 */

  else if (SpiState == 2)
  {
    NSSMD0 = 1;                        /* De-select the Slave */
    SpiState = 0;                      /* Reset the state */
  }/* end of state 2 */

  SPIF = 0;                            /* Clear the SPIF flag */
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
/**
  *****************************************************************************
  *  Function: void SpiBusWrite(uchar SpiDeviceID, uint16 DestAddress,
  *                 uchar* pDataStart, uchar len)
  *
  *  Arg1: -   SpiDeviceID      Dac, EEprom and Flash Memory
  *
  *  Arg2: -   Dest Address     For DAC, DestAddress is DAC(DAC1 to DAC4) and All DACs
  *                             For EEprom and Flash Memory, it is three bytes address
  *
  *  Arg3: -   *pDataStart      Data for Device
  *
  *  Arg4: -   Len              Number of bytes to be sent to Slave device (Max = 256)
  *
  *  Return(s): nothing
  *
  *  Description: SPI DEVICE ID
  *  DAC7614_ID       0x40     0  1  0  0 - 0  0  0  0
  *  DAC7614A_ID      0x4A     0  1  0  0 - 1  0  1  0
  *  DAC7614B_ID      0x4B     0  1  0  0 - 1  0  1  1
  *  DAC7614C_ID      0x4C     0  1  0  0 - 1  1  0  0
  *  DAC7614D_ID      0x4D     0  1  0  0 - 1  1  0  1
  *
  *  DAC7614ALL_ID    0x4F     0  1  0  0 - 1  1  1  1
  *
  *  EXT_DEVICE_ID    0x80     0  1  0  0 - 0  0  0  0
  *
  *  ---------------------------------------------------------------------
  *  |             MSB                 |             LSB                 |
  *  ----------------------------------|----------------------------------
  *  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | - | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  *  |--------------------------------------------------------------------
  *  | X1| X0| 0 | 0 | D | D | D | D | - | D | D | D | D | D | D | D | D |
  *  ---------------------------------------------------------------------
  *
  *   X -> Dac Number   D -> Dac Data (12 bit)
  *
  *  EEP25LC1024_ID   0xZZ     0  0  0  0 - 0  0  0   0 No device ID
  *****************************************************************************
*/

void SpiBusWrite(uchar SpiDeviceID, ulong32 DestAddress, uchar *pDataStart, uchar Len)
{
  while (!NSSMD0);       /* Wait until the SPI is free, in case it's already busy */

  ESPI0 = DISABLE;       /* Disable SPI interrupt */
  SPI_BUSY = YES;        /* Claim SPIbus (set to busy)  */

  /* Set target slave device DAC7614 or EEProm. */
  SPI_TARGET = SpiDeviceID & SPIDAC_MASK;

  if(SPI_TARGET == DAC7614_ID)      /* 0x40  */
  {
    SPI_TARGET = SpiDeviceID;       /* DAC CHANNEL CH0 or CH1 or CH2 or CH3 ? */
    DAC7614_XFER = 1;

    switch (SPI_TARGET)
    {
      case DAC7614_CH0:
        /* Just get DAC number D7,D6 bits in MSB byte */
        Dac7614Msb = (DAC_MSB_CH0 | *pDataStart++);
        Dac7614Lsb = *pDataStart;

        SpiCommand = SPI_SEND_DAC_LSB;

        NSSMD0 = 0;                 /* Select device */
        SPI0DAT = Dac7614Msb;       /* Send MSB byte first which will trigger */
                                    /* interrupt for LSB byte */
        break;

      case DAC7614_CH1:
        /* Just get DAC number D7,D6 bits in MSB byte */
        Dac7614Msb =  (DAC_MSB_CH1 | *pDataStart++);
        Dac7614Lsb =  *pDataStart;

        SpiCommand = SPI_SEND_DAC_LSB;

        NSSMD0 = 0;                 /* Select device */
        SPI0DAT = Dac7614Msb;       /* Send MSB byte first which will trigger */
                                    /* interrupt for LSB byte */
        break;

      case DAC7614_CH2:
        /* Just get DAC number D7,D6 bits in MSB byte */
        Dac7614Msb =  (DAC_MSB_CH2 | *pDataStart++);
        Dac7614Lsb =  *pDataStart;

        SpiCommand = SPI_SEND_DAC_LSB;

        NSSMD0 = 0;                 /* Select device */
        SPI0DAT = Dac7614Msb;       /* Send MSB byte first which will trigger */
                                    /* interrupt for LSB byte */
        break;

      case DAC7614_CH3:
        /* Just get DAC number D7,D6 bits in MSB byte */
        Dac7614Msb =  (DAC_MSB_CH3 | *pDataStart++);
        Dac7614Lsb =  *pDataStart;

        SpiCommand = SPI_SEND_DAC_LSB;

        NSSMD0 = 0;                 /* Select device */
        SPI0DAT = Dac7614Msb;       /* Send MSB byte first which will trigger */
                                    /* interrupt for LSB byte */
        break;
    }
  }
  else if (SPI_TARGET == EEP25LC1024_ID)
  {
    /* ###### Added this dummy code for not to show warnings ######  */
    SPI_WORDADDR = DestAddress;        /* Set the target address 24 bit address */
    pSPI_DATA_OUT = pDataStart;        /* The outgoing data pointer points to */
                                       /* the <len> data bytes */

    SPI_DATA_LEN = Len;                /* Specify to ISR that the next transfer */
                                       /* will contain length of bytes */
                                       /* time being allowed maximum 256 bytes */
  }

  else if (TARGET == EXT_DEVICE_ID)   /*  External Device ? */
  {
     /* add code here for external device */
  }

  ESPI0 = 1;                          /* Re-enable SPI interrupts */

  /* The rest of this command will be handled by the SPI ISR, which will */
  /* trigger when SPIF is set from sending the Command */
}

/**
  * @brief  This function will read data from the selected device
  * @param  SpiDeviceID     Device ID for Flash, EEprom and Ext Device
  * @param  *pDataStart     Destination address for data to be stored
  * @param  Dest Address    Source address from data to be read.
  * @param  Len             number of bytes to be read
  * @retval None
  * @note
  */
void SpiBusRead(uchar DeviceID, uchar* pDestAddr, ulong32 SrcAddr, uchar Length)
{
   while (!NSSMD0)
   {
     /* wait until the SPI is free, in case it's already busy */
   };

   /* Set SPIBus ISR Parameters and Address length */
   SPI_TARGET = DeviceID;              /* Set target slave address */

   SPI_WORDADDR = SrcAddr;             /* Set the target address, 8 or 24 bit */

   pSPI_DATA_IN = (uchar*)pDestAddr;   /* Specify the incoming data pointer */

   SPI_DATA_LEN = Length;              /* number of bytes to be read */

   SPI_WR = NO;                        /* Mark next transfer as a read */

   if(SPI_TARGET == EEP25LC1024_ID) /* if it is spi eeprom */
   {
     /*add code here */
   }

   else if(SPI_TARGET == EXT_DEVICE_ID) /* External Device */
   {
     /* add code here */
   }

   /* The rest of this command will be handled by the SPI ISR, which will  */
   /* trigger when SPIF is set from sending the Command  */
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
void PrintSpiDevicesMenu(void)
{
  uchar Count;

  ClrScrRstCur();

  for( Count = 0; Count<5; ++Count)
  {
    printf(SpiDevicesMenu[Count]);
  }
}

/**
  * @brief
  * @param  None
  * @retval None
  * @note
  */
void Print25lc1024Menu (void)
{
  PrintProgUnderDev();
}


/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/

