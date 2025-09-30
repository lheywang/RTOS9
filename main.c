/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty_min.c ========
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>

#include <driverlib.h>

/* Board Header file */
#include "main.h"
#include "uart.h"
#include "ring_buffer.h"

// proto
void init_gpio(void);
void irq0(unsigned index);
void TimerBTN(unsigned index);

void Event_BTNDelay(UArg arg0, UArg arg1);
void Event_BTNDelay2(UArg arg0, UArg arg1);
void uart_consummer(UArg arg0, UArg arg1);

uint32_t millis;

/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */

void init_gpio(void)
{
    // porc 1 :=====================================D
    GPIO_setAsOutputPin(GPIO_PORT_P1, LEDV);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, LEDV);
    // GPIO_setAsInputPin(GPIO_PORT_P1, BTN1 + BTN2);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, BTN1 + BTN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, BTN1 + BTN2);

    // porc 9 :=====================================D
    GPIO_setAsOutputPin(GPIO_PORT_P9, LEDR);
    GPIO_setOutputLowOnPin(GPIO_PORT_P9, LEDR);

    // interrpute :=====================================D
    GPIO_selectInterruptEdge(GPIO_PORT_P1, BTN1 + BTN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P1, BTN1 + BTN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, BTN1 + BTN2);

    return;
}

void irq0(unsigned index)
{
    uint16_t status = GPIO_getInterruptStatus(GPIO_PORT_P1, BTN1 + BTN2);

    switch (status)
    {
    case BTN1 :
        Event_post(h_event0, Event_BTN1);
        GPIO_clearInterrupt(GPIO_PORT_P1, BTN1);
        break;

    case BTN2 :
        Event_post(h_event0, Event_BTN2);
        GPIO_clearInterrupt(GPIO_PORT_P1, BTN2);
        break;

    default :
        break;
    }

    return;
}

void TimerBTN(unsigned index)
{
    millis += 1;
    return;
}

void Event_BTNDelay(UArg arg0, UArg arg1)
{
    uint16_t posted;
    uint32_t t;

    while (1)
    {
        posted = Event_pend(
                h_event0,
                Event_Id_NONE,
                Event_BTN1,
                TIMEOUT
        );

        switch (posted)
        {
        case Event_BTN1:
            t = millis;
            Timer_start(h_timer0);

            while (1)
            {
                if(GPIO_getInputPinValue(GPIO_PORT_P1, BTN1)) // short
                {
                    GPIO_setOutputHighOnPin(GPIO_PORT_P1, LEDV);
                    break;
                }

                if ((millis-t) > TIMEOUT_LONG) // long
                {
                    GPIO_setOutputLowOnPin(GPIO_PORT_P1, LEDV);
                    break;
                }
            }
            break;

        }

        Timer_stop(h_timer0);
        Task_sleep(50);
    }
}

// Could be combined with BTN1 + BTN2
void Event_BTNDelay2(UArg arg0, UArg arg1)
{
    uint16_t posted;
    uint32_t t;

    while (1)
    {
        posted = Event_pend(
                h_event0,
                Event_Id_NONE,
                Event_BTN2,
                TIMEOUT
        );

        switch (posted)
        {
        case Event_BTN2:
                t = millis;
                Timer_start(h_timer0);

                while (1)
                {
                    if(GPIO_getInputPinValue(GPIO_PORT_P1, BTN2)) // short
                    {
                        GPIO_setOutputHighOnPin(GPIO_PORT_P9, LEDR);
                        break;
                    }

                    if ((millis-t) > TIMEOUT_LONG) // long
                    {
                        GPIO_setOutputLowOnPin(GPIO_PORT_P9, LEDR);
                        break;
                    }
                }
                break;

        }

        Timer_stop(h_timer0);
        Task_sleep(50);
    }
}

// Buffer is init in the uart.c function
void uart_consummer(UArg arg0, UArg arg1)
{
    while (1)
    {
        if (buffer_get_size() > 16)
        {
            while (buffer_get_size() > 1)
            {
                uint8_t tmp = buffer_pop();
                if (tmp > 0x60)
                    tmp = tmp - 0x61 + 0x41; // set to lowercase
                else if (tmp > 0x40)
                    tmp = tmp - 0x41 + 0x61; // set to upercase

                EUSCI_A_UART_transmitData(EUSCI_A1_BASE, tmp);
                Task_sleep(5);
            }
        }

        Task_sleep(50);
    }
}



/*
 *  ======== main ========
 */
int main(void)
{
    // Watchdog (stop while init).
    WDT_A_hold(WDT_A_BASE);

    // FRAM optimisation
    PM5CTL0 &= ~LOCKLPM5;

    // init
    init_gpio();
    init_Uart();

    /* Start BIOS */
    BIOS_start();
    return (0);
}
