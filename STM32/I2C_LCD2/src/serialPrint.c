/*
 * serialPrint.c
 *
 *  Created on: Jul 17, 2016
 *      Author: Admin
 */

#include "serialPrint.h"
#include "stm32f10x.h"

#define SERIAL_BAUD_RATE  115200

void __init_serial()
{
	/* Bit configuration structure for GPIOA PIN9 and PIN10 */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enalbe clock for USART1, AFIO and GPIOA */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO |
						   RCC_APB2Periph_GPIOA, ENABLE);

	/* GPIOA PIN9 alternative function Tx */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIOA PIN10 alternative function Rx */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART configuration structure for USART1 */
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = SERIAL_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);
	/* Enable USART1 */
	USART_Cmd(USART1, ENABLE);
	/* Enable RXNE interrupt */
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/* Enable USART1 global interrupt */
	//NVIC_EnableIRQ(USART1_IRQn);

}

void Stdinout_Init(void)
{
	__init_serial();
}

PUTCHAR_PROTOTYPE
{
	// Place your implementation of fputc here

	// Wait while (Transmit data register empty) this will stop overriding buffer
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{}

	// write a character to the USART
	USART_SendData(USART1, (uint8_t) ch);

	// Loop until the transmission is complete
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

//GETCHAR_PROTOTYPE
int GetSerialChar()
{
	while ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET);
		return (int)USART_ReceiveData(USART1);
}

/*int SendSerial(uint8_t ch)
{
	 Configure instrumentation trace macroblock
	ITM->LAR = 0xC5ACCE55;
	ITM->TCR = (1 << ITM_TCR_ATBID_Pos) // Trace bus ID for TPIU
			 | (1 << ITM_TCR_DWTENA_Pos) // Enable events from DWT
			 | (1 << ITM_TCR_SYNCENA_Pos) // Enable sync packets
			 | (1 << ITM_TCR_ITMENA_Pos); // Main enable for ITM
	ITM->TER = 0xFFFFFFFF; // Enable all stimulus ports

	//__io_putchar(ch);
	//printf("A1");
	putchar((int)ch);
	char test = (char)ch;
	//_write(0, &test, 1);
	ITM_SendChar((uint32_t) ch);
	return ch;
}*/
