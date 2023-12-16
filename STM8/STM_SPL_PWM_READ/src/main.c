#include "stm8s_conf.h"
#include "stm8s103_Serial.h"
#include "stm8s_it.h"

void Clock_setup(void);
void GPIO_setup(void);
void TIM2_setup(void);
void TIM1_setup(void);

uint8_t msg[35] = {'\0'};
volatile unsigned int  overflow_count = 0;
volatile unsigned long pulse_ticks = 0;
volatile unsigned long start_time;
volatile unsigned long end_time;

void main(void)
{
    Clock_setup();
    GPIO_setup();
    TIM2_setup();
    TIM1_setup();

    Serial_begin(9600);
    Serial_print_string("Enter command");
    Serial_newline();

    unsigned char i = 0;
    char ch = 0;
    unsigned long time_period = 0;
 /*
    while (TRUE)
    {
        
        if (TIM2_GetCounter() > 6000)
        {
            GPIO_WriteLow(GPIOB, GPIO_PIN_5);
            time_period = pulse_ticks;
            //Serial_print_string("Time Period");
            end_time = TIM1_GetCounter();
            Serial_print_int(end_time);
            //Serial_newline();
            // UART1_SendData8('A');
            // while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET); //wait for sending
        }
        else
        {
            GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
        }
        
    }
    */
    while (1)
    {
        wfi();
        //end_time = TIM1_GetCounter();
        time_period = pulse_ticks;
        Serial_print_int(time_period);
        Serial_newline();

        for (size_t i = 0; i < 1000; i++)
        {
            for (size_t j = 0; j < 100; j++)
            {
                __asm__("nop");
            }            
        }        
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
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV2);

    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,
                          DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);

    
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
}

void GPIO_setup(void)
{
    GPIO_DeInit(GPIOB);
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);

    //GPIO_DeInit(GPIOC);
    //GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    //GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
}

void TIM2_setup(void)
{
    // PWM generation via PC5 (alternate Func)
    TIM2_DeInit();

    /* TIM2 configuration:
    - TIM2CLK is set to 2 MHz, the TIM2 Prescaler is equal to XX so the TIM2 counter
    clock used is 2 MHz / 32 = 62500 Hz

    - In this example we need to generate a time base equal to 500 ms
    so TIM2_PERIOD = ((0.5 * 62500) - 1) = 31243 */

    TIM2_TimeBaseInit(TIM2_PRESCALER_4, 1250); // (2Mhz/32) 62500 / 625
    //TIM2_TimeBaseInit(TIM2_PRESCALER_16, 100); // (2Mhz/16) 125000 / 1000
    
    TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 1000, TIM2_OCPOLARITY_LOW);
    TIM2_SetCompare1(312); // duty cycle 31250 /15625

    //TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_Cmd(ENABLE);
}

void TIM1_setup(void)
{
    // capture via PC6 (alternate Func)
    TIM1_DeInit();
    TIM1_TimeBaseInit(200, TIM1_COUNTERMODE_UP, 55535, 1);
    // TIM1_ClearFlag(TIM1_FLAG_CC2);
    
    TIM1_ICInit(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, 1, 1);
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
    TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
    TIM1_Cmd(ENABLE);

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