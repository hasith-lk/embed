/**
  ******************************************************************************
  * @file		delay.h
  * @author	Yohanes Erwin Setiawan
  * @date		10 January 2016
  ******************************************************************************
  */

#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
#include "system_stm32f10x.h"

void DelayInit(void);
void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);

// Arduino Model
//#define delayMicroseconds(us) delay_us(us)
//#define delay(ms) delay_ms(ms)

// Systick Model
#define delayMicroseconds(us) DelayUs(us)
#define delay(ms) DelayMs(ms)

#ifndef STM32_DELAY_US_MULT
#define STM32_DELAY_US_MULT             (SystemCoreClock / 6000000L)
#endif
/**
 * @brief Delay the given number of microseconds.
 *
 * @param us Number of microseconds to delay.
 */
static inline void delay_us(uint32_t us) {
    us *= STM32_DELAY_US_MULT;

    /* fudge for function call overhead  */
    us--;
    asm volatile("   mov r0, %[us]          \n\t"
                 "1: subs r0, #1            \n\t"
                 "   bhi 1b                 \n\t"
                 :
                 : [us] "r" (us)
                 : "r0");
}

static inline void delay_ms(uint32_t ms){
   while(ms--)
      delay_us(1000);
}

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
