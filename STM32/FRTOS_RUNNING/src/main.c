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
#include "stdint.h"


#define LIGHT_PORT GPIOB
#define LIGHT_1 GPIO_Pin_12
#define LIGHT_2 GPIO_Pin_13
#define LIGHT_3 GPIO_Pin_14
#define LIGHT_4 GPIO_Pin_15

#define COLOR_PORT GPIOA
#define GREEN_PIN GPIO_Pin_1
#define BLUE_PIN GPIO_Pin_2
#define RED_PIN GPIO_Pin_3

#define TIME_DELAY 1000

void ledInit(void);


 // RTOS task
void vTaskLedRed(void *p);
void vTaskLedYellow(void *p);
void vTaskLedGreen(void *p);
void vTaskLedGreen1(void *p);
void vTaskLedRunner(void *p);

void LightLED(uint16_t lightNo, uint16_t colorPin);

int main(void)
{
#ifdef DEBUG
   initialise_monitor_handles();
#endif

   // Configure GPIO for LED
   ledInit();

   // Create LED blink task
   /*xTaskCreate(vTaskLedRed, (const char*) "Red LED Blink", 128, NULL, 1, NULL);
   xTaskCreate(vTaskLedYellow, (const char*) "Yellow LED Blink", 128, NULL, 1, NULL);
   xTaskCreate(vTaskLedGreen, (const char*) "Green LED Blink", 128, NULL, 1, NULL);
   xTaskCreate(vTaskLedGreen1, (const char*) "Green LED Blink 1", 128, NULL, 1, NULL);

   // Start RTOS scheduler
   vTaskStartScheduler();*/

   xTaskCreate(vTaskLedRunner, (const char*) "LED Runner", 128, NULL, 1, NULL);
   vTaskStartScheduler();

   return 0;
}

void vTaskLedRunner(void *p)
{
   for (;;)
   {
      LightLED(LIGHT_1, GREEN_PIN);
      vTaskDelay(TIME_DELAY/portTICK_RATE_MS);

      LightLED(LIGHT_1, BLUE_PIN);
      vTaskDelay(TIME_DELAY/portTICK_RATE_MS);

      LightLED(LIGHT_1, RED_PIN);
      vTaskDelay(TIME_DELAY/portTICK_RATE_MS);

      LightLED(LIGHT_1, BLUE_PIN);
      vTaskDelay(TIME_DELAY/portTICK_RATE_MS);

   }
}

void LightLED(uint16_t lightNo, uint16_t color)
{
   LIGHT_PORT->ODR = ~lightNo;
   COLOR_PORT->ODR = ~color;
}

void ledInit()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure PC13, PC14, PC15 as push-pull output
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Pin = LIGHT_1 | LIGHT_2 | LIGHT_3 | LIGHT_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LIGHT_PORT, &GPIO_InitStruct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //GPIO_InitStruct.GPIO_Pin = RED_PIN | GREEN_PIN | BLUE_PIN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(COLOR_PORT, &GPIO_InitStruct);
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
