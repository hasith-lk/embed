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
#include "utility.h"
			
static __IO uint32_t TimingDelay;
void Delay(__IO uint32_t nTime);
void loopDelay(uint32_t nTime);

int main(void)
{
#ifdef DEBUG
	initialise_monitor_handles();
#endif
	//Stdout_Init();
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/* Configure PD0 and PD2 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	printf("Start \n\r");
	//SendSerial((uint8_t)'A');
	printf("Start Again \n\r");
/*
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//Delay(2000);
		loopDelay(0x100000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//Delay(1000);
		loopDelay(0x100000);
#ifdef DPRINT
		printf("Done \n\r");
		//SendSerial((uint8_t)'L');
		printf("D \n\r");
		ITM_SendChar((uint32_t) 'I');
#endif
	}
*/

	if (IsDebuggerAttched())
		printf("D \n\r");
	else
	{
		while(1)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			loopDelay(0x100000);
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			loopDelay(0x100000);
		}
	}

}

void loopDelay(uint32_t nTime)
{
	uint32_t c;
	for (c = nTime; c > 0; c--)
	{
		asm volatile("nop");
	}
}

void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}
