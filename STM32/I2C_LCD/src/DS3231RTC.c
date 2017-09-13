/*
 * DS3231RTC.c
 *
 *  Created on: Aug 9, 2016
 *      Author: Admin
 */

#include "DS3231RTC.h"

#define RTC_SECONDS 0x00
#define RTC_MINUTES 0x01
#define RTC_HOURS   0x02
#define RTC_DAY     0x03
#define RTC_DATE    0x04
#define RTC_MONTH   0x05
#define RTC_YEAR    0x06

#define RTC_I2C_ADDR    0x68

uint8_t __attribute__ ((noinline)) bcd2dec(uint8_t n)
{
    return n - 6 * (n >> 4);
}

uint8_t dec2bcd(uint8_t n)
{
    return n + 6 * (n / 10);
}

void RtcInit(void)
{
   I2c_Init();
}

// Should set time in 24 hour format
uint8_t SetTime(TimeStruct *timeStruct)
{
   // Init array for 7 registers + 1 init register address
   uint8_t receivedData[] = {0,0,0,0,0,0,0,0};

   // Seconds
   receivedData[RTC_SECONDS + 1] = dec2bcd((*timeStruct).seconds);

   // Minutes
   receivedData[RTC_MINUTES + 1] = dec2bcd((*timeStruct).minutes);

   // 24 hour =
   receivedData[RTC_HOURS + 1] = dec2bcd((*timeStruct).hours);

   // Day
   receivedData[RTC_DAY + 1] = dec2bcd((*timeStruct).dayOfweek);

   // Date
   receivedData[RTC_DATE + 1] = dec2bcd((*timeStruct).date);

   // Month
   receivedData[RTC_MONTH + 1] = dec2bcd((*timeStruct).month);

   // Year
   receivedData[RTC_YEAR + 1] = dec2bcd((*timeStruct).year);


   if (I2C_Write(I2C1, receivedData, 8, RTC_I2C_ADDR << 1) == Success)
   {
      uint8_t statusReg;
      I2C_Read(I2C1, &statusReg, 1, RTC_I2C_ADDR << 1 );
      //char printBuf[20];
      //sprintf(printBuf, "%d %d %d %d %d:%d %d", year, month, date, day, hours, minutes, seconds);
      //puts(printBuf);
   }
   return 0;
   }

TimeStruct GetTime(void)
{
   TimeStruct temp;

   uint8_t DS3231_START_REG_ADDR = 0x00;
   uint8_t receivedData[] = {0,0,0,0,0,0,0};

   if (I2C_Write(I2C1, &DS3231_START_REG_ADDR, 1, RTC_I2C_ADDR << 1) == Success)
   {
      I2C_Read(I2C1, receivedData, 7, RTC_I2C_ADDR << 1);

      // Seconds
      temp.seconds = bcd2dec(receivedData[RTC_SECONDS]);

      // Minutes
      temp.minutes = bcd2dec(receivedData[RTC_MINUTES]);

      // 12/24 hour = 6th MSB
      temp.format = receivedData[RTC_HOURS] & (0x40);

      // Hours
      if (temp.format == 0 )
      {
         // 24 hour format = 7 LS bits
         temp.hours = bcd2dec(receivedData[RTC_HOURS] & (0x3F));
      }
      else
      {
         // 12 hour format = 6 LS bits
         temp.hours = bcd2dec(receivedData[RTC_HOURS] & (0x1F));
         // AM/PM = 7th LSB
         temp.timeOfDay = receivedData[RTC_HOURS] & (0x20);
      }
      // Day of week
      temp.dayOfweek = bcd2dec(receivedData[RTC_DAY]);

      // date
      temp.date = bcd2dec(receivedData[RTC_DATE]);

      // Month
      temp.month = bcd2dec(receivedData[RTC_MONTH] & (0x1F));

      // Year
      temp.year = bcd2dec(receivedData[RTC_YEAR]);

      //sprintf(timeString, "%d %d %d %d:%d %02d", year, month, date, hours, minutes, seconds);
      //puts(timeString);
   }

   return temp;
   }

char* GetTimeString(TimeStruct t)
{
   char timeString[20];
   sprintf(timeString, "T%d %d %d %d:%02d %02d", t.year, t.month, t.date, t.hours, t.minutes, t.seconds);
   return timeString;
}
