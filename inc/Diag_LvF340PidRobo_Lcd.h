/**
  ******************************************************************************
  * @file    Diag_LvF340_PidRobo_Lcd.h
  * @author  MSU FTE MAIC Application Team
  * @version V1.0.0
  * @date    20-March-2016
  * @brief   Header for Diag_LvF340_PidRobo_Lcd.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LVF340_PID_ROBO_LCD_H
#define __LVF340_PID_ROBO_LCD_H

/** @addtogroup BSP
  * @{
  */
/** @addtogroup LCD
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
                                  /* D7  D6  D5  D4 -D3  D2  D1  D0    */
/* Slave address for write           SA6 SA5 SA2 SA3 SA2 SA2 SA0 R/nW  */
#define  PCF2119_ID        0x76   /*  0   1   1   1   0   1   1   0    */
                                  /* SA0 is connected to Vdd, R/nW=0   */

/* Exported variables for LCD  buffers for TOP and BOTTOM lines  -------------*/
extern uchar  data lcd_display_image_top[17];
extern uchar  data lcd_display_image_bottom[17];

/* Exported functions ------------------------------------------------------- */
void  lcd_init(void);
uchar lcd_test(void);

void erase_lcd_buffer(void);
void display_sensor_readings_in_bars(const unsigned int *ir_sensors_values, uchar ucX, uchar ucY, bit EraseBuffer);

void  print_string (const schar *pcStr, uchar ucX, uchar ucY, bit EraseBuffer);
void  print_integer (int val, uchar ucX, uchar ucY, bit EraseBuffer);


#endif /* __LVF340_PID_ROBO_LCD_H */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
