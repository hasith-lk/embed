#include "stm8s_conf.h"
//#include "stdio.h"
//#include "stm8s_gpio.h"

void clock_setup(void);
void GPIO_setup(void);
void TIM2_setup(void);
void UART1_setup(void);

uint8_t msg[35] = { '\0' };

void main(void)
{
    clock_setup();
    GPIO_setup();
    //TIM2_setup();
    UART1_setup();

/*
    Serial_begin(9600);
    Serial_print_string("Enter command");
    Serial_newline();*/

    unsigned char i = 0;
    char ch = 0;

    while (TRUE)
    {
        /*
        if (Serial_available())
        {
            ch = Serial_read_char();
            /*
            Serial_print_string("You have pressed: ");            
            Serial_print_char(ch);
            Serial_newline();
            

            //sprintf(msg, "You have pressed: %c\n\r", ch);
            //Serial_print_string(msg);

            if (ch == '0')
                GPIO_WriteHigh(GPIOB, GPIO_PIN_5); // LED OFF

            if (ch == '1')
                GPIO_WriteLow(GPIOB, GPIO_PIN_5); // LED ON
        }
        */
    
        /*
        if (TIM2_GetCounter() > 976)
        {
            GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
            UART1_SendData8('A');
            while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET); //wait for sending
        }
        else
        {
            GPIO_WriteLow(GPIOB, GPIO_PIN_5);
        }
        */

        /* UART - start */    
        if (UART1_GetFlagStatus(UART1_FLAG_RXNE) == TRUE)
        {
            ch = UART1_ReceiveData8();
            // LCD_goto(7, 1);
            // LCD_putchar(ch);
            UART1_ClearFlag(UART1_FLAG_RXNE);
            //sprintf(msg, "You have pressed: %c\n\r", ch);
            //UART1_SendData8(ch);
            if (ch == '1')
                GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
            else
                GPIO_WriteLow(GPIOB, GPIO_PIN_5);
        }

        if (UART1_GetFlagStatus(UART1_FLAG_TXE) == FALSE)
        {
            // LCD_goto(7, 0);
            // LCD_putchar(i + 0x30);
            i++;
        }
        /* UART - end */
    }
}

void clock_setup(void)
{
    CLK_DeInit();

    CLK_HSECmd(DISABLE);
    CLK_LSICmd(DISABLE);
    CLK_HSICmd(ENABLE);
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE)
        ;

    CLK_ClockSwitchCmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);

    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,
                          DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);

    CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
}

void GPIO_setup(void)
{
    GPIO_DeInit(GPIOB);
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);

    GPIO_DeInit(GPIOD);
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);
}

void TIM2_setup(void)
{
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_2048, 1952);
    TIM2_Cmd(ENABLE);
}

void UART1_setup(void)
{
    UART1_DeInit();

    UART1_Init(9600,
               UART1_WORDLENGTH_8D,
               UART1_STOPBITS_1,
               UART1_PARITY_NO,
               UART1_SYNCMODE_CLOCK_DISABLE,
               UART1_MODE_TXRX_ENABLE);

    UART1_Cmd(ENABLE);
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