#include "rtc.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <driverlib.h>
#include <ti/sysbios/knl/Event.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

#include "hal_LCD.h"

void rtc_init(void)
{
    /* lundi 29 09 2025 @ 12:00:00 */
    act_calendar.Year =         2025;
    act_calendar.Month =        9;
    act_calendar.DayOfWeek =    1;
    act_calendar.DayOfMonth =   29;

    act_calendar.Hours =        12;
    act_calendar.Minutes =      00;
    act_calendar.Seconds =      00;

    // Init
    RTC_C_initCalendar(
            RTC_C_BASE,
            &act_calendar,
            RTC_C_FORMAT_BINARY
    );

    RTC_C_clearInterrupt(RTC_C_BASE, RTC_C_CLOCK_READ_READY_INTERRUPT);
    RTC_C_enableInterrupt(RTC_C_BASE, RTC_C_CLOCK_READ_READY_INTERRUPT);

    RTC_C_startClock(RTC_C_BASE);

    return;
}

void rtc_irq(unsigned index)
{
    uint8_t status = RTC_C_getInterruptStatus(RTC_C_BASE, RTC_C_CLOCK_READ_READY_INTERRUPT);

    switch(status)
    {
    case RTC_C_CLOCK_READ_READY_INTERRUPT:

        RTC_C_clearInterrupt(RTC_C_BASE, RTC_C_CLOCK_READ_READY_INTERRUPT);
        Event_post(h_event_rtc, EVENT_RTC);

        break;
    }

    return;
}

void rtc_display(void)
{
    Calendar time = RTC_C_getCalendarTime(RTC_C_BASE);

    ShowChar((time.Seconds % 10) + '0', pos6);
    ShowChar((time.Seconds / 10) + '0', pos5);
    ShowChar((time.Minutes % 10) + '0', pos4);
    ShowChar((time.Minutes / 10) + '0', pos3);
    ShowChar((time.Hours % 10) + '0', pos2);
    ShowChar((time.Hours / 10) + '0', pos1);

    LCDM7   |= 0x04;
    LCDBM7  |= 0x04;

    LCDM20  |= 0x04;
    LCDBM20 |= 0x04;

    return;
}
