/*
 * stm32_pin_port_map.h
 *
 *  Created on: May 13, 2023
 *      Author: Home Admin
 */

#ifndef INC_STM32_PIN_PORT_MAP_H_
#define INC_STM32_PIN_PORT_MAP_H_

#define PIN_PA0 	((uint8_t)0x00)
#define PIN_PA1 	((uint8_t)0x01)
#define PIN_PA2 	((uint8_t)0x02)
#define PIN_PA3 	((uint8_t)0x03)
#define PIN_PA4 	((uint8_t)0x04)
#define PIN_PA5 	((uint8_t)0x05)
#define PIN_PA6 	((uint8_t)0x06)
#define PIN_PA7 	((uint8_t)0x07)
#define PIN_PA8 	((uint8_t)0x08)
#define PIN_PA9 	((uint8_t)0x09)
#define PIN_PA10 	((uint8_t)0x0A)
#define PIN_PA11 	((uint8_t)0x0B)
#define PIN_PA12 	((uint8_t)0x0C)
#define PIN_PA13 	((uint8_t)0x0D)
#define PIN_PA14 	((uint8_t)0x0E)
#define PIN_PA15 	((uint8_t)0x0F)

#define PIN_PB0 	((uint8_t)0x10)
#define PIN_PB1 	((uint8_t)0x11)
#define PIN_PB2 	((uint8_t)0x12)
#define PIN_PB3 	((uint8_t)0x13)
#define PIN_PB4 	((uint8_t)0x14)
#define PIN_PB5 	((uint8_t)0x15)
#define PIN_PB6 	((uint8_t)0x16)
#define PIN_PB7 	((uint8_t)0x17)
#define PIN_PB8 	((uint8_t)0x18)
#define PIN_PB9 	((uint8_t)0x19)
#define PIN_PB10 	((uint8_t)0x1A)
#define PIN_PB11 	((uint8_t)0x1B)
#define PIN_PB12 	((uint8_t)0x1C)
#define PIN_PB13 	((uint8_t)0x1D)
#define PIN_PB14 	((uint8_t)0x1E)
#define PIN_PB15 	((uint8_t)0x1F)

#define PIN_PC0 	((uint8_t)0x20)
#define PIN_PC1 	((uint8_t)0x21)
#define PIN_PC2 	((uint8_t)0x22)
#define PIN_PC3 	((uint8_t)0x23)
#define PIN_PC4 	((uint8_t)0x24)
#define PIN_PC5 	((uint8_t)0x25)
#define PIN_PC6 	((uint8_t)0x26)
#define PIN_PC7 	((uint8_t)0x27)
#define PIN_PC8 	((uint8_t)0x28)
#define PIN_PC9 	((uint8_t)0x29)
#define PIN_PC10 	((uint8_t)0x2A)
#define PIN_PC11 	((uint8_t)0x2B)
#define PIN_PC12 	((uint8_t)0x2C)
#define PIN_PC13 	((uint8_t)0x2D)
#define PIN_PC14 	((uint8_t)0x2E)
#define PIN_PC15 	((uint8_t)0x2F)

#define PIN_PD0 	((uint8_t)0x30)
#define PIN_PD1 	((uint8_t)0x31)
#define PIN_PD2 	((uint8_t)0x32)
#define PIN_PD3 	((uint8_t)0x33)
#define PIN_PD4 	((uint8_t)0x34)
#define PIN_PD5 	((uint8_t)0x35)
#define PIN_PD6 	((uint8_t)0x36)
#define PIN_PD7 	((uint8_t)0x37)
#define PIN_PD8 	((uint8_t)0x38)
#define PIN_PD9 	((uint8_t)0x39)
#define PIN_PD10 	((uint8_t)0x3A)
#define PIN_PD11 	((uint8_t)0x3B)
#define PIN_PD12 	((uint8_t)0x3C)
#define PIN_PD13 	((uint8_t)0x3D)
#define PIN_PD14 	((uint8_t)0x3E)
#define PIN_PD15 	((uint8_t)0x3F)

// repeat pin ids for 4 times t represent each port
const uint16_t Pin_To_HAL_Pin_Map[] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4,
		GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,
		GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,
		GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9,
		GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_0,
		GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7,
		GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14,
		GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
		GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12,
		GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 };

const GPIO_TypeDef *Pin_To_HAL_Port_Map[] = { GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA,
		GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB,
		GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOC, GPIOC,
		GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC,
		GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD,
		GPIOD, GPIOD, GPIOD };

#define PINTOPORT(x)  		((GPIO_TypeDef *)GPIOA_BASE)Pin_To_HAL_Port_Map[x]
#define PINTOPINMASK(x)  	(uint16_t)Pin_To_HAL_Pin_Map[x]

#endif /* INC_STM32_PIN_PORT_MAP_H_ */
