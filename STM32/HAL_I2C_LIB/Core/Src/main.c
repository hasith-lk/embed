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
//#include "liquidcrystal_i2c.h"
#include "LCD_I2C.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SLAVE_ADDRESS_LCD 0x4E
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void UsbReceiveData (uint8_t* Buf, uint16_t Len);
void LCD2();
void LCD1();
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
static const uint8_t LCD_ADDR = 0x23 << 1; // Use 8-bit address

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t buf[12];
  int16_t val;
  float temp_c;
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
  //MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  //RegisterCallback(UsbReceiveData);
  //uint8_t msgBuff[] = "USB Initialized\n\r";
  //Send_USB_Data(msgBuff, sizeof(msgBuff));
  LCD1();
  LCD2();

  /*
  HD44780_Init(2);
  HD44780_Clear();
  HD44780_SetCursor(0,0);
  HD44780_PrintStr("STM32 BLUE PILL");
  HD44780_SetCursor(0,1);
  HD44780_PrintStr("I2C LCD DEMO");
  HAL_Delay(2000);
  */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //LCD_I2C_printStr(LCD_I2C, "Sample" );
	  HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */
//
  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */
//
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
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */
//
  /* USER CODE END I2C1_Init 2 */

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

  /*Configure GPIO pin : BOARD_LED_Pin */
  GPIO_InitStruct.Pin = BOARD_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BOARD_LED_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void UsbReceiveData (uint8_t* Buf, uint16_t Len)
{
	if(Buf[0] == '1'){
		 HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_SET);
	  }
	  else if(Buf[0] == '0'){
		  HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_RESET);
	  }

	Send_USB_Data(Buf, Len);
}

void LCD1() {
	HAL_Delay(1000);
	//LCD_I2C_Struct *LCD_I2C = new_LCD_I2C_Struct(0x23, 3, POSITIVE);
	LCD_I2C_Struct *LCD_I2C = new_LCD_I2C_Struct(0x27, 3, POSITIVE);

	LCD_I2C_begin(LCD_I2C, 16, 2);

	LCD_I2C_clear(LCD_I2C);
	LCD_I2C_printStr(LCD_I2C, "Hello World");


}

void LCD2() {
	LCD_I2C_Struct *LCD_I2C = new_LCD_I2C_Struct(0x23, 3, POSITIVE);
	//LCD_I2C_Struct *LCD_I2C2 = new_LCD_I2C_Struct(0x27, 3, POSITIVE);

	LCD_I2C_begin(LCD_I2C, 20, 4);

	LCD_I2C_clear(LCD_I2C);
	LCD_I2C_printStr(LCD_I2C, "Hello World");

	LCD_setCursor(LCD_I2C, 7, 2);

	uint8_t customChar[8] = { 0b00000, 0b10000, 0b01000, 0b00100, 0b00010,
			0b00001, 0b00000, 0b00000 };

	uint8_t delayTime = 500;
	LCD_createChar(LCD_I2C, 1, customChar);

	while (1) {
		int i;

		HAL_Delay(delayTime);
		for (i = 1; i < 4; i++) {
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

		HAL_Delay(delayTime);
		for (i = 1; i < 4; i++) {
			LCD_setCursor(LCD_I2C, 1, i);
			LCD_I2C_printStr(LCD_I2C, "||||||||");
		}

		//LCD_backlight(LCD_I2C);

		HAL_Delay(delayTime);
		for (i = 1; i < 4; i++) {
			LCD_setCursor(LCD_I2C, 1, i);
			LCD_I2C_printStr(LCD_I2C, "////////");
		}

		HAL_Delay(delayTime);
		for (i = 1; i < 4; i++) {
			LCD_setCursor(LCD_I2C, 1, i);
			LCD_I2C_printStr(LCD_I2C, "--------");
		}
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
