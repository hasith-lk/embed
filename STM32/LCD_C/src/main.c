/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "LCD_I2C.h"

int main(void)
{
#ifdef DEBUG
   initialise_monitor_handles();
#endif

   DelayInit();

   I2c_Init();

   LCD2();

   LCD_I2C_Struct *LCD_I2C = new_LCD_I2C_Struct(0x27, 3, POSITIVE);
   //LCD_I2C_Struct *LCD_I2C2 = new_LCD_I2C_Struct(0x27, 3, POSITIVE);

   LCD_I2C_begin(LCD_I2C, 16, 2);

   LCD_I2C_clear(LCD_I2C);
   LCD_I2C_printStr(LCD_I2C, "Hello AMALIKA");

   LCD_setCursor(LCD_I2C, 7, 2);

   uint8_t customChar[8] = {
         0b00000,
         0b10000,
         0b01000,
         0b00100,
         0b00010,
         0b00001,
         0b00000,
         0b00000
      };

      uint8_t delayTime = 500;
      LCD_createChar(LCD_I2C,1, customChar);

      while(1)
      {

         delay(delayTime);
         //LCD_noBacklight(LCD_I2C);
         LCD_setCursor(LCD_I2C, 1, 1);

         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 2, 1);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 3, 1);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 4, 1);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 5, 1);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 6, 1);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 7, 1);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 8, 1);
         LCD_write(LCD_I2C, 0b0001);

         delay(delayTime);
         LCD_setCursor(LCD_I2C, 1, 1);

         LCD_I2C_printStr(LCD_I2C, "||||||||");

         //LCD_backlight(LCD_I2C);

         delay(delayTime);
         LCD_setCursor(LCD_I2C, 1, 1);
         LCD_I2C_printStr(LCD_I2C, "////////");

         delay(delayTime);
         LCD_setCursor(LCD_I2C, 1, 1);
         LCD_I2C_printStr(LCD_I2C, "--------");

      }

   return 1;
}

void LCD2()
{
   LCD_I2C_Struct *LCD_I2C = new_LCD_I2C_Struct(0x23, 3, POSITIVE);
   //LCD_I2C_Struct *LCD_I2C2 = new_LCD_I2C_Struct(0x27, 3, POSITIVE);

   LCD_I2C_begin(LCD_I2C, 20, 4);

   LCD_I2C_clear(LCD_I2C);
   LCD_I2C_printStr(LCD_I2C, "Hello AMALIKA");

   LCD_setCursor(LCD_I2C, 7, 2);

   uint8_t customChar[8] = {
         0b00000,
         0b10000,
         0b01000,
         0b00100,
         0b00010,
         0b00001,
         0b00000,
         0b00000
      };

      uint8_t delayTime = 500;
      LCD_createChar(LCD_I2C,1, customChar);

      while(1)
      {
         int i;

         delay(delayTime);
         for (i=1; i<4; i++){
         LCD_setCursor(LCD_I2C, 1, i);

         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 2, i);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 3, i);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 4, i);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 5, i);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 6, i);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 7, i);
         LCD_write(LCD_I2C, 0b0001);
         LCD_setCursor(LCD_I2C, 8, i);
         LCD_write(LCD_I2C, 0b0001);
         }

         delay(delayTime);
         for (i=1; i<4; i++){
         LCD_setCursor(LCD_I2C, 1, i);
         LCD_I2C_printStr(LCD_I2C, "||||||||");
         }

         //LCD_backlight(LCD_I2C);

         delay(delayTime);
         for (i=1; i<4; i++){
         LCD_setCursor(LCD_I2C, 1, i);
         LCD_I2C_printStr(LCD_I2C, "////////");
         }

         delay(delayTime);
         for (i=1; i<4; i++){
         LCD_setCursor(LCD_I2C, 1, i);
         LCD_I2C_printStr(LCD_I2C, "--------");
         }
      }
}
