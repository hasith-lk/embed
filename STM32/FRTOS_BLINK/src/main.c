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
#include "FreeRTOS.h"
#include "task.h"

#include "LCD_I2C.h"
			
void ledInit(void);
void lcdInit(void);

LCD_I2C_Struct *LCD_I2C162;
LCD_I2C_Struct *LCD_I2C204;

// RTOS task
void vTaskLedRed(void *p);
void vTaskLedYellow(void *p);
void vTaskLedGreen(void *p);
void vTaskLedGreen1(void *p);

void vTaskLcd162(void *p);
void vTaskLcd204(void *p);

int main(void)
{
#ifdef DEBUG
   initialise_monitor_handles();
#endif

   // Configure GPIO for LED
   ledInit();
   lcdInit();

   // Create LED blink task
   xTaskCreate(vTaskLedRed, (const char*) "Red LED Blink", 128, NULL, 1, NULL);
   xTaskCreate(vTaskLedYellow, (const char*) "Yellow LED Blink", 128, NULL, 1, NULL);
   xTaskCreate(vTaskLedGreen, (const char*) "Green LED Blink", 128, NULL, 1, NULL);

   xTaskCreate(vTaskLedGreen1, (const char*) "Green LED Blink 1", 128, NULL, 1, NULL);

   xTaskCreate(vTaskLcd162, (const char*) "LCD 16X2", 128, NULL, 1, NULL);
   xTaskCreate(vTaskLcd204, (const char*) "LCD 20X4", 120, NULL, 1, NULL);

   // Start RTOS scheduler
   vTaskStartScheduler();

   return 0;
}

void ledInit()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure PC13, PC14, PC15 as push-pull output
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void vTaskLedRed(void *p)
{
    for (;;)
    {
        GPIOB->ODR ^= GPIO_Pin_12;
        if ((GPIOB->ODR & GPIO_Pin_12) != (uint32_t)Bit_RESET)
        {
           GPIOA->BRR = GPIO_Pin_5;
        }
        else
        {
           GPIOA->BSRR = GPIO_Pin_5;
        }
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}

void vTaskLedYellow(void *p)
{
    for (;;)
    {
        GPIOB->ODR ^= GPIO_Pin_13;
        vTaskDelay(500/portTICK_RATE_MS);
    }
}

void vTaskLedGreen(void *p)
{
    for (;;)
    {
        GPIOB->ODR ^= GPIO_Pin_14;
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}

void vTaskLedGreen1(void *p)
{
    for (;;)
    {
        GPIOB->ODR ^= GPIO_Pin_15;
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
   /* This function will get called if a task overflows its stack.   If the
   parameters are corrupt then inspect pxCurrentTCB to find which was the
   offending task. */

   ( void ) pxTask;
   ( void ) pcTaskName;

   for( ;; );
}


void lcdInit(void)
{
   I2c_Init();

   LCD_I2C204 = new_LCD_I2C_Struct(0x23, 3, POSITIVE);

   LCD_I2C162 = new_LCD_I2C_Struct(0x27, 3, POSITIVE);

   LCD_I2C_begin(LCD_I2C162, 16, 2);

   LCD_I2C_begin(LCD_I2C204, 20, 4);

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

   LCD_createChar(LCD_I2C162, 1, customChar);
   LCD_createChar(LCD_I2C204, 1, customChar);
}

void vTaskLcd162(void *p)
{
   uint8_t delayTime = 500;
   while(1)
   {
      int i;

      taskDelay(delayTime);
      for (i=1; i<4; i++){
      LCD_setCursor(LCD_I2C162, 1, i);

      LCD_write(LCD_I2C162, 0b0001);
      LCD_setCursor(LCD_I2C162, 2, i);
      LCD_write(LCD_I2C162, 0b0001);
      LCD_setCursor(LCD_I2C162, 3, i);
      LCD_write(LCD_I2C162, 0b0001);
      LCD_setCursor(LCD_I2C162, 4, i);
      LCD_write(LCD_I2C162, 0b0001);
      LCD_setCursor(LCD_I2C162, 5, i);
      LCD_write(LCD_I2C162, 0b0001);
      LCD_setCursor(LCD_I2C162, 6, i);
      LCD_write(LCD_I2C162, 0b0001);
      LCD_setCursor(LCD_I2C162, 7, i);
      LCD_write(LCD_I2C162, 0b0001);
      LCD_setCursor(LCD_I2C162, 8, i);
      LCD_write(LCD_I2C162, 0b0001);
      }

      taskDelay(delayTime);
      for (i=1; i<4; i++){
      LCD_setCursor(LCD_I2C162, 1, i);
      LCD_I2C_printStr(LCD_I2C162, "||||||||");
      }

      //LCD_backlight(LCD_I2C);

      taskDelay(delayTime);
      for (i=1; i<4; i++){
      LCD_setCursor(LCD_I2C162, 1, i);
      LCD_I2C_printStr(LCD_I2C162, "////////");
      }

      taskDelay(delayTime);
      for (i=1; i<4; i++){
      LCD_setCursor(LCD_I2C162, 1, i);
      LCD_I2C_printStr(LCD_I2C162, "--------");
      }

      //taskDelay(delayTime);
      LCD_setCursor(LCD_I2C162, 6, 0);
      LCD_I2C_printStr(LCD_I2C162, "16 X 2");
   }
}

void vTaskLcd204(void *p)
{
   uint8_t delayTime = 500;
   while(1)
   {
      int i;

      taskDelay(delayTime);
      for (i=1; i<4; i++){
      LCD_setCursor(LCD_I2C204, 1, i);

      LCD_write(LCD_I2C204, 0b0001);
      LCD_setCursor(LCD_I2C204, 2, i);
      LCD_write(LCD_I2C204, 0b0001);
      LCD_setCursor(LCD_I2C204, 3, i);
      LCD_write(LCD_I2C204, 0b0001);
      LCD_setCursor(LCD_I2C204, 4, i);
      LCD_write(LCD_I2C204, 0b0001);
      LCD_setCursor(LCD_I2C204, 5, i);
      LCD_write(LCD_I2C204, 0b0001);
      LCD_setCursor(LCD_I2C204, 6, i);
      LCD_write(LCD_I2C204, 0b0001);
      LCD_setCursor(LCD_I2C204, 7, i);
      LCD_write(LCD_I2C204, 0b0001);
      LCD_setCursor(LCD_I2C204, 8, i);
      LCD_write(LCD_I2C204, 0b0001);
      }

      taskDelay(delayTime);
      for (i=1; i<4; i++){
      LCD_setCursor(LCD_I2C204, 1, i);
      LCD_I2C_printStr(LCD_I2C204, "||||||||");
      }

      //LCD_backlight(LCD_I2C);

      taskDelay(delayTime);
      for (i=1; i<4; i++){
      LCD_setCursor(LCD_I2C204, 1, i);
      LCD_I2C_printStr(LCD_I2C204, "////////");
      }

      taskDelay(delayTime);
      for (i=1; i<4; i++){
      LCD_setCursor(LCD_I2C204, 1, i);
      LCD_I2C_printStr(LCD_I2C204, "--------");
      }

      //taskDelay(delayTime);
      LCD_setCursor(LCD_I2C204, 6, 0);
      LCD_I2C_printStr(LCD_I2C204, "20 X 4");
   }
}
