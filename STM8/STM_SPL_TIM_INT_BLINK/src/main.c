#include "stm8s_conf.h"
#include "stm8s_it.h"

void Clock_setup(void);
void GPIO_setup(void);
void TIM2_setup(void);

void main(void)
{
    Clock_setup();
    GPIO_setup();
    TIM2_setup();

    while (1)
    {
        wfi();
    }
}

void Clock_setup(void)
{
    CLK_DeInit();

    CLK_HSECmd(DISABLE);
    CLK_LSICmd(DISABLE);
    CLK_HSICmd(ENABLE);
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE)
        ;

    CLK_ClockSwitchCmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV8);

    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,
                          DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);

    /*
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);*/
}

void GPIO_setup(void)
{
    GPIO_DeInit(GPIOB);
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
}

void TIM2_setup(void)
{
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_1024, 500); // 2 MHz / 1024 / 500
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_Cmd(ENABLE);
    enableInterrupts();
}
#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif