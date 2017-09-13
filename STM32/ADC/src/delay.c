/**
  ******************************************************************************
  * @file		delay.c
  * @author	Yohanes Erwin Setiawan
  * @date		10 January 2016
  ******************************************************************************
  */

#include "delay.h"

typedef enum {Error = 0, Success = !Error, Deinit = 10 } Status;

// For store tick counts in us
static __IO uint32_t usTicks;
static __IO uint16_t timerInit = Deinit;

// SysTick_Handler function will be called every 1 us
void SysTick_Handler()
{
	if (usTicks != 0)
	{
		usTicks--;
	}
}

void DelayInit()
{
   if (timerInit == Deinit)
   {
      // Update SystemCoreClock value
      SystemCoreClockUpdate();
      // Configure the SysTick timer to overflow every 1 us
      SysTick_Config(SystemCoreClock / 1000000);
      timerInit = Success;
   }
}

void DelayUs(uint32_t us)
{
	// Reload us value
	usTicks = us;
	// Wait until usTick reach zero
	while (usTicks);
}

void DelayMs(uint32_t ms)
{
	// Wait until ms reach zero
	while (ms--)
	{
		// Delay 1ms
		DelayUs(1000);
	}
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
