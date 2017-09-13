/*
 * main.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: Admin
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stm32f10x.h"
//#include "delay.h"
//#include "serialPrint.h"
#include "LiquidCrystal_I2C.h"

#ifdef DEBUG
extern "C" void initialise_monitor_handles();
#endif

// Size optimization
extern "C" void *malloc(size_t t){
   return (void*)0;}

extern "C" void free(void *){}

void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}
// end size optimizations

int main ()
{
#ifdef DEBUG
	initialise_monitor_handles();
#else
	//Stdinout_Init();
#endif

	//puts("Enter something");
	DelayInit();

	I2c_Init();
	LiquidCrystal_I2C lcd1602(0x27, 3, POSITIVE);
	lcd1602.begin(16, 2);

   lcd1602.clear();
   lcd1602.print("Hello");

   lcd1602.setCursor(1, 1);
   lcd1602.print("Hello Amalika !");

   lcd1602.clear();
   uint8_t delayTime = 500;

   uint8_t customChar[8] = {
      0b00000,
      0b10000,
      0b01000,
      0b00100,
      0b00010,
      0b00001,
      0b00000,
      0b00000
   };

   lcd1602.createChar(1, customChar);

   //while(1)
   {
      delay(delayTime);
      lcd1602.setCursor(1, 1);
      lcd1602.write(0b0001);
      lcd1602.setCursor(2, 1);
      lcd1602.write(0b0001);
      lcd1602.setCursor(3, 1);
      lcd1602.write(0b0001);
      lcd1602.setCursor(4, 1);
      lcd1602.write(0b0001);
      lcd1602.setCursor(5, 1);
      lcd1602.write(0b0001);
      lcd1602.setCursor(6, 1);
      lcd1602.write(0b0001);
      lcd1602.setCursor(7, 1);
      lcd1602.write(0b0001);
      lcd1602.setCursor(8, 1);
      lcd1602.write(0b0001);

      delay(delayTime);
      lcd1602.setCursor(1, 1);
      lcd1602.print("||||||||");

      delay(delayTime);
      lcd1602.setCursor(1, 1);
      lcd1602.print("////////");

      delay(delayTime);
      lcd1602.setCursor(1, 1);
      lcd1602.print("--------");

   }
   lcd1602.setCursor(1, 1);
   lcd1602.print("Hello Amalika !");
}

