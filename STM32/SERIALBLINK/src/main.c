/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "delay.h"
#include "i2c.h"
#include "serialPrint.h"


void BlinkTest() {
	printf("Start \n\r");
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	DelayInit();
	printf("delay init \n\r");
	while (1) {
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		DelayMs(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		DelayMs(2000);
	}
	printf("delay Complete \n\r");
}

int main ()
{
#ifdef DEBUG
	initialise_monitor_handles();
#else
	Stdinout_Init();
#endif

	BlinkTest();

	puts("Enter something");

	return 0;
}
