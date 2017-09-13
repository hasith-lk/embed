/*
 * serialPrint.h
 *
 *  Created on: Jul 17, 2016
 *      Author: Admin
 */

#ifndef SERIALPRINT_H_
#define SERIALPRINT_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define GETCHAR_PROTOTYPE int __io_getchar()
//#else
//  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
//#endif /* __GNUC__ */

void Stdinout_Init(void);
int GetSerialChar(void);

#ifdef __cplusplus
}
#endif

#endif /* SERIALPRINT_H_ */
