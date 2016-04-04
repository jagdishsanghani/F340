/**
  ******************************************************************************
  * @file    Diag_F340_SMBus.c
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   SMbus module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of SMBus peripheral:
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Diag_LvF340PidRobo_Main.h"

/** @addtogroup c8051F340_StdPeriph_Driver
  * @{
  */

/** @addtogroup SMBus
  * @ SMBus driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  WRITE          0x00   /* SMBus WRITE command */
#define  READ           0x01   /* SMBus READ command */

/* SMBus Buffer Size */
#define  SMB_BUFF_SIZE  0x08   /* Defines the maximum number of bytes that can be */
                               /* sent or received in a single transfer */

/* SMBus Status vector - top 4 bits only */
#define  SMB_MTSTA      0xE0    /* (MT) start transmitted */
#define  SMB_MTDB       0xC0    /* (MT) data byte transmitted */
#define  SMB_MRDB       0x80    /* (MR) data byte received */

/* Private macro -------------------------------------------------------------*/
/* Variables for SMbus -------------------------------------------------------------*/
bit SMB_BUSY;              /* Software flag for SMBUS busy */

uchar* pSMB_DATA_IN;       /* Global pointer for SMBus data */
                           /* All receive data is written here */

uchar SMB_SINGLEBYTE_OUT;  /* Global holder for single byte writes. */

uchar* pSMB_DATA_OUT;      /* Global pointer for SMBus data. */
                           /* All transmit data is read from here */

uchar SMB_DATA_LEN;        /* Global holder for number of bytes */
                           /* to send or receive in the current SMBus transfer. */

uint16 WORD_ADDR;          /* Global holder for the EEPROM word address that will */
                           /* be accessed in the next transfer */

uchar TARGET;              /* Target SMBus slave address */

bit SMB_BUSY;              /* Software flag to indicate when the RTC_ByteRead() */
                           /* or RTC_ByteWrite() functions have claimed the SMBus */

bit SMB_RW;                /* Software flag to indicate the */
                           /* direction of the current transfer */

bit SMB_SENDWORDADDR;      /* When set, this flag causes the ISR to send the */
                           /* 8-bit <WORD_ADDR> after sending the slave address. */

bit SMB_RANDOMREAD;        /* When set, this flag causes the ISR to send a START */
                           /* signal after sending the word address. The ISR handles */
                           /* this switchover if the <SMB_RANDOMREAD> bit is set. */

bit SMB_ACKPOLL;           /* When set, this flag causes the ISR to send a repeated START */
                           /* until the slave has acknowledged its address. */

bit SMB_EEPROM;           /* Software flag to indicate that device is eeprom */
bit SMB_EEP_HIGHBYTE;      /* Flag for EEProm high byte address */


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  ****************************************************************************
  * @brief
  * @param  None
  * @retval None
  * @note
  *    The SMBus peripheral is configured as follows:
  *      - SMBus enabled
  *      - Slave mode disabled
  *      - Timer0 used as clock source. The maximum SCL frequency will be
  *        approximately 1/3 the Timer0 overflow rate
  *      - Setup and hold time extensions enabled
  *      - Free and SCL low timeout detection enabled
  *****************************************************************************
  */
void InitSMBus (void)
{
/*  SMBUS Clock/Configuration REGISTER 0xC1 */
   SMB0CF = 0x5C;    /*  D7    D6    D5     D4          D3      D2     D1      D0 */
                     /* ENSMB  INH  BUSY  EXTHOLD --  SMBTOE  SMBFTE  SMBCS1  SMBCS0 */
                     /*   0     1     0     1           1       1       0        0   */
                     /* Use Timer0 overflows as SMBus clock source; */
                     /* Disable slave mode; */
                     /* Enable setup & hold time extensions; */
                     /* Enable SMBus Free timeout detect; */
                     /* Enable SCL low timeout detect; */

   SMB0CF |= 0x80;   /* Enable SMBus; */
}

/**
  ****************************************************************************
  * @brief
  * @param  None
  * @retval None
  * @note
  * SMBus ISR state machine
  *  - Master only implementation - no slave or arbitration states defined
  *  - All incoming data is written starting at the global pointer <pSMB_DATA_IN>
  *  - All outgoing data is read from the global pointer <pSMB_DATA_OUT>
  *****************************************************************************
  */
INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
   bit FAIL = 0;   /* Used by the ISR to flag failed transfers */
   static char i;  /* Used by the ISR to count the number of data bytes sent or received */

   static bit SEND_START = 0;   /* Send a start */
   switch (SMB0CN & 0xF0)       /* Status vector */
   {
     /* Master Transmitter/Receiver: START condition transmitted. */
     case SMB_MTSTA:
       SMB0DAT = TARGET;    /* Load address of the target slave */
       SMB0DAT &= 0xFE;     /* Clear the LSB of the address for the R/W bit */
       SMB0DAT |= SMB_RW;   /* Load R/W bit */
       STA = 0;             /* Manually clear START bit */
       i = 0;               /* Reset data byte counter */
       break;

     /* Master Transmitter: Data byte (or Slave Address) transmitted */
      case SMB_MTDB:
        if (ACK)   /* Slave Address or Data Byte Acknowledged? */
        {
          if (SEND_START)
          {
            STA = 1;
            SEND_START = 0;
            break;
          }
              
			    if(SMB_SENDWORDADDR)           /* Are we sending the word address? */
          {
            SMB0DAT = WORD_ADDR & 0xFF;  /* Send LOW BYTE address ONLY */
            SMB_SENDWORDADDR = 0;        /* Clear flag */

            if (SMB_RANDOMREAD)          /* this bit is set in ReadSMbus() function */
            {
              SEND_START = 1;            /* Send a START after the next ACK cycle */
              SMB_RW = READ;
            }

            break;
          }/** end of SMB_SENDWORDADDR **/

          if (SMB_RW==WRITE)             /* Is this transfer a WRITE? */
          {
            if (i < SMB_DATA_LEN)        /* Is there data to send? */
            {
              SMB0DAT = *pSMB_DATA_OUT; /* send data byte */
              pSMB_DATA_OUT++;          /* increment data out pointer */
              i++;                      /* increment number of bytes sent */
            }
            else
            {
              STO = 1;      /* Set STO to terminte transfer */
              SMB_BUSY = 0; /* Clear software busy flag */
            }
          }
          else {}           /* If this transfer is a READ, then take no action. */
                            /* Slave address was transmitted. A separate 'case' */
                            /* is defined for data byte recieved. */
        }
        else                /* If slave NACK, */
        {
          if(SMB_ACKPOLL)
          {
            STA = 1;        /* Restart transfer */
          }
          else
          {
            FAIL = 1;    /* Indicate failed transfer and handle at end of ISR */
          }
        }
        break;

      /* Master Receiver: byte received */
      case SMB_MRDB:
        if ( i < SMB_DATA_LEN )      /* Is there any data remaining? */
        {
          *pSMB_DATA_IN = SMB0DAT;   /* Store received byte */
          pSMB_DATA_IN++;            /* Increment data in pointer */
          i++;                       /* Increment number of bytes received */
          ACK = 1;                   /* Set ACK bit (may be cleared later in the code) */
        }

        if (i == SMB_DATA_LEN)       /* This is the last byte */
        {
          SMB_BUSY = 0;              /* Free SMBus interface */
          ACK = 0;                   /* Send NACK to indicate last byte of this transfer */
          STO = 1;                   /* Send STOP to terminate transfer */
        }
        break;

      default:
        FAIL = 1;        /* Indicate failed transfer and handle at end of ISR */

        break;
   }/* end of switch statement */

   if (FAIL)             /* If the transfer failed */
   {
     SMB0CF &= ~0x80;    /* Reset communication */
     SMB0CF |= 0x80;
     STA = 0;
     STO = 0;
     ACK = 0;

     SMB_BUSY = 0;       /* Free SMBus */
     FAIL = 0;
   }
   SI = 0;               /* Clear interrupt flag */
}


/**
  * @brief
  * @param1 device_id - Device ID number
  * @param2 uchar dest_addr - destination address of data to be written into the device
  * @param3 uchar* ptr_data_start - pointer to the array of data to be written into the device
  * @param4 uchar len - length of the data array to be written
  * @retval None
  * @note   This function writes <len> data bytes to the slave device.
  */
void SMbusWrite(uchar SMBusDeviceID, uint16 DestAddr, uchar* pDataStart, uchar Len)
{
  while (SMB_BUSY)
  {
    /* Wait for SMBus to be free. */
  }

  SMB_BUSY = 1;           /* Claim SMBus (set to busy)  */

  /* Set SMBus ISR parameters */
  TARGET = SMBusDeviceID;     /* Set target slave address PCF2119 */

  SMB_RW = WRITE;         /* Mark next transfer as a write either control or data byte */

  if(TARGET == PCF2119_ID)      /* PCF2119 LCD Display Driver ? */
  {
    SMB_SENDWORDADDR = 0;       /* do not send any address after Slave Address */
  }
  else if(TARGET == DS1338_ID)  /* DS1338 RTC ? */
  {
    SMB_SENDWORDADDR = 1;       /* Send Byte Address after Slave Address */
  }
  else if(TARGET == EEP24LC1025_ID)  /* 24LC1025 Serial EEProm ? */
  {
    SMB_EEPROM = 1;             /* send high & low addresses of eeprom */
    SMB_SENDWORDADDR = 1;       /* Send Control byte or Word Address after Slave Address */
  }
  else
  {
    SMB_SENDWORDADDR = 1;       /* Send Control byte or Word Address after Slave Address */
  }
  SMB_RANDOMREAD = 0;     /* Do not send a START signal after the word address */
  SMB_ACKPOLL = 1;        /* Enable Acknowledge Polling (The ISR will automatically restart */
                           /* the transfer if the slave does not acknoledge its address. */

  /* Specify the Outgoing Data */
  WORD_ADDR = DestAddr;       /* Set the target address, 8 or 16 bit address */

  pSMB_DATA_OUT = pDataStart; /* The outgoing data pointer points to the <len> data bytes */

  SMB_DATA_LEN = Len;         /* Specify to ISR that the next transfer will contain length of bytes */
                              /* time being allowed maximum 256 bytes */
  STA = 1;                    /* Initiate SMBus Transfer */
}

/**
  ****************************************************************************
  * @brief
  * @param  uchar* dest_addr - pointer to the array that will be filled
  *                with the data from the RTC location from 0x02 to 0x0A.
  * @param  uchar src_addr - beginning address to read data from the RTC
  * @param  uchar len - length of the array to be read from the RTC
  * @retval
  * @note
  *****************************************************************************
  */
void SMbusRead(uchar SMBusDeviceID, uchar* DestAddr, uint16 SrcAddr, uchar Len)
{
   while (SMB_BUSY);           /* Wait for SMBus to be free. */
   SMB_BUSY = 1;               /* Claim SMBus (set to busy) */

   /* Set SMBus ISR parameters */
   TARGET = SMBusDeviceID;     /* Set target slave address */

   SMB_RW = WRITE;             /* after the repeated start is sent. The ISR handles this */
                               /* switch over if the <SMB_RANDOMREAD> bit is set. */

   if(TARGET == DS1338_ID)     /* DS1338 Real Time Clock ? */
   {
     SMB_SENDWORDADDR = 1;     /* Send Byte Address after Slave Address */
   }
   else if(TARGET == EEP24LC1025_ID)   /* 24LC1025 EEPROM? */
   {
     SMB_EEPROM = 1;             /* Send high & low addresses of eeprom */
     SMB_SENDWORDADDR = 1;       /* Send Control byte or Word Address after Slave Address */
   }
   else
   {
     SMB_SENDWORDADDR = 0;     /* Do not send any address after Slave Address */
   }

   SMB_RANDOMREAD = 1;         /* Send a START after the word address */
   SMB_ACKPOLL = 1;            /* Enable Acknowledge Polling */

   /* Set the target address in the DS1338's internal address space */
   WORD_ADDR = SrcAddr;

   /* Set the the incoming data pointer */
   pSMB_DATA_IN = (uchar*)DestAddr;

   /* Specify to ISR that the next transfer will contain <len> data bytes */
   SMB_DATA_LEN = Len;

   /* Initiate SMBus Transfer */
   STA = 1;
   while(SMB_BUSY);            /* Wait until data is read */
}

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
