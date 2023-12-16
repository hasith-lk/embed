/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <ad9850.hpp>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define POWER_DOWN_BIT	0b00000100
#define PULSE_HIGH(pin)		HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_RESET);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void MM_AD9850_Init() {
	/*
	 Initialize AD9850
	 PA2 - W_CLK
	 PA3 - FQ_UD
	 PA4 - DATA
	 PA5 - RESET
	 */
	// https://github.com/Billwilliams1952/AD9850-Library-Arduino/blob/master/AD9850.cpp
	HAL_GPIO_WritePin(GPIOA, PIN_FQ_UPD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, PIN_W_CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, PIN_MOD_RESET_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, PIN_DATA_Pin, GPIO_PIN_RESET);

	//reset
	//HAL_GPIO_WritePin(GPIOA, PIN_FQ_UPD_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOA, PIN_MOD_RESET_Pin, GPIO_PIN_SET);			// Default to parallel mode
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, PIN_MOD_RESET_Pin, GPIO_PIN_RESET);

	// and enable serial mode
	HAL_GPIO_WritePin(GPIOA, PIN_FQ_UPD_Pin, GPIO_PIN_RESET);
	PULSE_HIGH(PIN_W_CLK_Pin);		// Force into Serial mode
	PULSE_HIGH(PIN_FQ_UPD_Pin);
}
void MM_AD9850_SetFreq(unsigned int frequency) {
	// frequency calc from datasheet page 8 =  * /2^32
	// AD9850 has 125Mhz clock
	uint32_t frequencyWord = (frequency * 4294967296) / 125000000;

	printf("Freq %x ",frequencyWord);

	for (uint8_t i = 0; i < 32; i++) {
		//digitalWrite(data7, (uint8_t) (freq & 0x1));
		if ((frequencyWord & 0x1) == 1) {
			HAL_GPIO_WritePin(GPIOA, PIN_DATA_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOA, PIN_DATA_Pin, GPIO_PIN_RESET);
		}

		PULSE_HIGH(PIN_W_CLK_Pin);
		frequencyWord >>= 1;
	}

	uint8_t phaseVal = 0;
	uint8_t powerDown = 0;

	if (powerDown)
		phaseVal |= POWER_DOWN_BIT;
	else
		phaseVal &= ~POWER_DOWN_BIT;

	printf("phase %x ",phaseVal);
	for (uint8_t i = 0; i < 8; i++) {
		//digitalWrite(data7, phaseVal & 0x01);
		if ((frequency & 0x1) == 1)
			HAL_GPIO_WritePin(GPIOA, PIN_DATA_Pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOA, PIN_DATA_Pin, GPIO_PIN_RESET);

		PULSE_HIGH(PIN_W_CLK_Pin);
		phaseVal >>= 1;
	}

	PULSE_HIGH(PIN_FQ_UPD_Pin);		// Should see frequency/phase now.
}

void MM_AD9850_Init2() {
	/*
	 Initialize AD9850
	 PA0 - W_CLK - 2
	 PA1 - FQ_UD - 3
	 PA2 - DATA - 4
	 PA3 - RESET - 5
	 All output 2Mhz
	 */
	HAL_GPIO_WritePin(PIN_EN_SERIAL_GPIO_Port, PIN_EN_SERIAL_Pin, GPIO_PIN_RESET);
	// enable GPIOA
	//RCC->APB2ENR = RCC_APB2ENR_IOPAEN;

	// set pin as output, 2Mhz
	//GPIOA->CRL = GPIO_CRL_MODE0_1 | GPIO_CRL_MODE1_1 | GPIO_CRL_MODE2_1
	//		| GPIO_CRL_MODE3_1;

	// enable serial mode (datasheet page 12 figure 10)
	// UP
	//GPIOA->BSRR = 1<<3 | 1<<0 | 1<<1;
	GPIOA->BSRR = 1 << 5 | 1 << 2 | 1 << 3;
	HAL_Delay(100);
	//DOWN
	//GPIOA->BRR = 1<<3 | 1<<0 | 1<<1;
	GPIOA->BRR = 1 << 5 | 1 << 2 | 1 << 3;
}

void MM_AD9850_SetFreq2(unsigned int frequency) {
	int n = 0;
	// frequency calc from datasheet page 8 =  * /2^32
	// AD9850 has 125Mhz clock
	frequency = (frequency * 4294967296) / 125000000;

	for (n = 0; n < 32; n++) {
		// set data pin (2) to 1 or 0
		if ((frequency & 0x1) == 1) {
			// set data pin (2) to 1
			//GPIOA->BSRR = 1<<2;
			GPIOA->BSRR = 1 << 4;
		} else {
			// set data pin (2) to 0
			//GPIOA->BRR = 1<<2;
			GPIOA->BRR = 1 << 4;
		}

		// W_CLK pulse, so AD9850 can shift data in register
		//GPIOA->BSRR = 1<<0;
		//GPIOA->BRR = 1<<0;

		GPIOA->BSRR = 1 << 2;
		GPIOA->BRR = 1 << 2;

		// shift one bit...
		frequency >>= 1;
	}

	// all 0 for ad9850
	//GPIOA->BRR = 1<<2;
	GPIOA->BRR = 1 << 4;
	for (n = 0; n < 8; n++) {
		// W_CLK pulse, so AD9850 can shift data in register
		//GPIOA->BSRR = 1<<0;
		//GPIOA->BRR = 1<<0;

		GPIOA->BSRR = 1 << 2;
		HAL_Delay(40);
		GPIOA->BRR = 1 << 2;
	}

	// should be done
	//GPIOA->BSRR = 1<<1;
	//GPIOA->BRR = 1<<1;

	GPIOA->BSRR = 1 << 3;
	HAL_Delay(40);
	GPIOA->BRR = 1 << 3;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */
	double freq = 1;
	double trimFreq = 124999500;

	int phase = 0;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_I2C1_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */

	//DDS.begin(W_CLK_GPIO_Port, W_CLK_Pin, FQ_UPD_Pin, DATA_Pin, MOD_RESET_Pin);
	//DDS.calibrate(trimFreq);
	//DDS.setfreq(freq, phase);
	//DDS.down();
	MM_AD9850_Init();
	MM_AD9850_SetFreq(1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		/*HAL_GPIO_TogglePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin);
		 HAL_Delay(800);
		 HAL_GPIO_TogglePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin);
		 HAL_Delay(400);*/
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PIN_EN_SERIAL_Pin|PIN_W_CLK_Pin|PIN_FQ_UPD_Pin|PIN_DATA_Pin
                          |PIN_MOD_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BOARD_LED_Pin */
  GPIO_InitStruct.Pin = BOARD_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BOARD_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PIN_EN_SERIAL_Pin */
  GPIO_InitStruct.Pin = PIN_EN_SERIAL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PIN_EN_SERIAL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PIN_W_CLK_Pin PIN_FQ_UPD_Pin PIN_DATA_Pin PIN_MOD_RESET_Pin */
  GPIO_InitStruct.Pin = PIN_W_CLK_Pin|PIN_FQ_UPD_Pin|PIN_DATA_Pin|PIN_MOD_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int _write(int32_t file, uint8_t *ptr, int32_t len) {

	for (int i = 0; i < len; i++) {
		/*if (*ptr != '\n')
		 LCD_print(LCD_I2C, *ptr);
		 else{
		 int rpad = 21 - len;
		 while (rpad--)
		 LCD_print(LCD_I2C, ' ');
		 row = ++row % 4;
		 LCD_setCursor(LCD_I2C, 0, row);
		 }*/
		ITM_SendChar(*ptr++);
	}

	return len;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
