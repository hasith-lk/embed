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
#include "DS3231RTC.h"
#include "serialPrint.h"
#include "LiquidCrystal_I2C.h"
#include "i2c.h"

#ifdef DEBUG
extern "C" void initialise_monitor_handles();
#endif

#define RTC_SECONDS 0x00
#define RTC_MINUTES 0x01
#define RTC_HOURS   0x02
#define RTC_DAY     0x03
#define RTC_DATE    0x04
#define RTC_MONTH   0x05
#define RTC_YEAR    0x06

/*
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
*/

uint8_t __attribute__ ((noinline)) bcd2dec(uint8_t n);
uint8_t dec2bcd(uint8_t n);
void SetTime(uint8_t year, uint8_t month, uint8_t date, uint8_t day, uint8_t hours, uint8_t minutes, uint8_t seconds);
void GetTime1(char *timeString);

uint8_t testMethod(uint8_t year, uint8_t month, uint8_t date, uint8_t day, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t count)
{
   SetTime(year, month, date, day, hours, minutes, seconds);
   delay(1000);
   char timeString[16];
   while (count--) {
      GetTime1(timeString);
      delay(900);
   }
   return count;
}


int main ()
{
#ifdef DEBUG
	initialise_monitor_handles();
#else
	Stdinout_Init();
#endif

	//puts("Enter something");
	DelayInit();
	I2c_Init();

	LiquidCrystal_I2C lcd1602(0x27, 3, POSITIVE);
	lcd1602.begin(16, 2);

   lcd1602.clear();

   /*char timeString[20];
   char timeString1[20];
	while(1)
	{
	   lcd1602.clear();
	   GetTime(timeString);
	   strcpy(timeString1, timeString);
	   lcd1602.print(timeString1);
	   delay_ms(700);
	}*/
   while(1)
   {
      TimeStruct t = GetTime();
      lcd1602.print(GetTimeString(t));
   }
}

void GetTime1(char *timeString)
{
   uint8_t const RTC_ADDR = 0x68;
   uint8_t DS3231_TIME_CAL_ADDR = 0x00;
   uint8_t receivedData[] = {0,0,0,0,0,0,0};

   //uint8_t receivedData =0;

   if (I2C_Write(I2C1, &DS3231_TIME_CAL_ADDR, 1, RTC_ADDR << 1) == Success)
   {
      I2C_Read(I2C1, receivedData, 7, RTC_ADDR << 1);

      // Seconds
      uint8_t seconds = bcd2dec(receivedData[RTC_SECONDS]);

      // Minutes
      uint8_t minutes = bcd2dec(receivedData[RTC_MINUTES]);

      // 12/24 hour = 6th MSB
      uint8_t timeFormat = receivedData[RTC_HOURS] & (0x40);

      // Hours
      uint8_t hours  = 0;
      uint8_t periodOfDay = 0;
      if (timeFormat == 0 )
      {
         // 24 hour format = 7 LS bits
         hours = bcd2dec(receivedData[RTC_HOURS] & (0x3F));
      }
      else
      {
         // 12 hour format = 6 LS bits
         hours = bcd2dec(receivedData[RTC_HOURS] & (0x1F));
         // AM/PM = 7th LSB
         periodOfDay = receivedData[RTC_HOURS] & (0x20);
      }
      // Day of week
      uint8_t day = bcd2dec(receivedData[RTC_DAY]);

      // date
      uint8_t date = bcd2dec(receivedData[RTC_DATE]);

      // Month
      uint8_t month = bcd2dec(receivedData[RTC_MONTH] & (0x1F));

      // Year
      uint8_t year = bcd2dec(receivedData[RTC_YEAR]);


      //itoa(seconds, printBuf, 10);
      // 16 16 8 7:17 27
      // 16 16 9 0:15 23
      sprintf(timeString, "%d %d %d %d:%d %02d", year, month, date, hours, minutes, seconds);
      puts(timeString);
   }
}

void SetTime(uint8_t year, uint8_t month, uint8_t date, uint8_t day, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
   uint8_t receivedData[] = {0,0,0,0,0,0,0,0};

   // Seconds
   receivedData[RTC_SECONDS + 1] = dec2bcd(seconds);

   // Minutes
   receivedData[RTC_MINUTES + 1] = dec2bcd(minutes);

   // 24 hour =
   receivedData[RTC_HOURS + 1] = dec2bcd(hours);

   // Day
   receivedData[RTC_DAY + 1] = dec2bcd(day);

   // Date
   receivedData[RTC_DATE + 1] = dec2bcd(date);

   // Month
   receivedData[RTC_MONTH + 1] = dec2bcd(month);

   // Year
   receivedData[RTC_YEAR + 1] = dec2bcd(year);

   uint8_t const RTC_ADDR = 0x68;

//   uint8_t DS3231_TIME_CAL_ADDR = 0x00;
//   I2C_Write(I2C1, &DS3231_TIME_CAL_ADDR, 1, RTC_ADDR << 1);

   if (I2C_Write(I2C1, receivedData, 8, RTC_ADDR << 1) == Success)
   {
      uint8_t statusReg;
      I2C_Read(I2C1, &statusReg, 1, RTC_ADDR << 1 );
      char printBuf[20];
      sprintf(printBuf, "%d %d %d %d %d:%d %d", year, month, date, day, hours, minutes, seconds);
      puts(printBuf);
   }

}

uint8_t __attribute__ ((noinline)) bcd2dec(uint8_t n)
{
    return n - 6 * (n >> 4);
}

uint8_t dec2bcd(uint8_t n)
{
    return n + 6 * (n / 10);
}
