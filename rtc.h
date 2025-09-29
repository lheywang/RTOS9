/*
 * rtc.h
 *
 *  Created on: 29 sept. 2025
 *      Author: l.heywang
 */

#ifndef RTC_H_
#define RTC_H_
#include <driverlib.h>

#define EVENT_RTC Event_Id_02

Calendar act_calendar;

void rtc_init(void);
void rtc_display(void);


#endif /* RTC_H_ */
