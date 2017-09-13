/*
 * utility
 *
 *  Created on: Apr 24, 2016
 *      Author: Admin
 */

#ifndef __UTILITY
#define __UTILITY

#include <stdio.h>
#include <stdint.h>


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;

void Stdout_Init(void);
int SendSerial(uint8_t ch);
bool IsDebuggerAttched(void);

#endif /* __UTILITY */
