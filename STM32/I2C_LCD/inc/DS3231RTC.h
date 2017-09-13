/*
 * DS3231RTC.h
 *
 *  Created on: Aug 9, 2016
 *      Author: Admin
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "i2c.h"

#ifndef DS3231RTC_H_
#define DS3231RTC_H_

#define RTC_STATUS 0x0F

typedef struct
{
   uint8_t seconds;
   uint8_t minutes;
   uint8_t hours;
   uint8_t format;
   uint8_t timeOfDay;
   uint8_t dayOfweek;
   uint8_t date;
   uint8_t month;
   uint8_t year;
}TimeStruct;

void RtcInit(void);

uint8_t SetTime(TimeStruct *);
TimeStruct GetTime();
char* GetTimeString(TimeStruct);

#endif /* DS3231RTC_H_ */
