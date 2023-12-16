#include "stm8s_conf.h"
#include "stm8s103_Serial.h"
#include "stm8s_it.h"
#include "i2c.h"
// #include "LCD_I2C.h"
#include "LCD.h"

void Clock_setup(void);
void GPIO_setup(void);
void TIM2_setup(void);
void TIM1_setup(void);

uint8_t msg[35] = {'\0'};
volatile unsigned int overflow_count = 0;
volatile unsigned long pulse_ticks = 0;
volatile unsigned long start_time;
volatile unsigned long end_time;

unsigned char bl_state;
unsigned char data_value;

void delay_ms(uint8_t micro)
{
   int i = 0;
   int j = 0;
   for (i = 0; i <= micro; i++)
   {
      for (j = 0; j < 720; j++) // Nop = Fosc/4     (CLK_GetClockFreq()/4 ?)
        __asm__("nop");        // Perform no operation //assembly code <span style="white-space:pre"> </span>
   }
}

void show_value(unsigned char value)
{
   char ch = 0x00;
   ch = ((value / 100) + 0x30);
   LCD_goto(6, 1);
   LCD_putchar(ch);
   ch = (((value / 10) % 10) + 0x30);
   LCD_goto(7, 1);
   LCD_putchar(ch);
   ch = ((value % 10) + 0x30);
   LCD_goto(8, 1);
   LCD_putchar(ch);
}

void main(void)
{
    const char txt1[] = {"MICROARENA"};
    const char txt2[] = {"SShahryiar"};
    const char txt3[] = {"STM8S003K3"};
    const char txt4[] = {"Discovery!"};
    unsigned char s = 0x00;

    Clock_setup();
    GPIO_setup();
    LCD_init();
    LCD_clear_home();
    LCD_goto(3, 0);
    LCD_putstr(txt1);
    LCD_goto(3, 1);
    LCD_putstr(txt2);
    delay_ms(2600);
    LCD_clear_home();
    for (s = 0; s < 10; s++)
    {
        LCD_goto((3 + s), 0);
        LCD_putchar(txt3[s]);
        delay_ms(60);
    }
    for (s = 0; s < 10; s++)
    {
        LCD_goto((3 + s), 1);
        LCD_putchar(txt4[s]);
        delay_ms(60);
    }
    delay_ms(2600);
    s = 0;
    LCD_clear_home();
    LCD_goto(3, 0);
    LCD_putstr(txt1);
    while (1)
    {
        show_value(s);
        s++;
        delay_ms(200);
    };
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
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
}

void GPIO_setup(void)
{
    // GPIO_DeInit(GPIOB);
    // GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);

    //GPIO_DeInit(GPIOB);
    //GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_FAST);
    //GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);
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
    // TIM2_TimeBaseInit(TIM2_PRESCALER_16, 100); // (2Mhz/16) 125000 / 1000

    TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 1000, TIM2_OCPOLARITY_LOW);
    TIM2_SetCompare1(312); // duty cycle 31250 /15625

    // TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
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