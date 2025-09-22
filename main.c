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

/* TI-RTOS Header files */
// #include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

#include <driverlib.h>

/* Board Header file */
#include "main.h"

// proto
void initgpioparcequeilfautbienetquecestrigolodefairedesnomsarallonge(void);
void tache0parcequeilfautbienetquecestrigolodefairedesnomsarallonge(UArg arg0, UArg arg1);
void tache1parcequeilfautbienetquecestrigolodefairedesnomsarallonge(UArg arg0, UArg arg1);
void jesuisunetachedinterruptnommeedemaniereparticulierementdouteuseetlonguemaisonsenfouspuisquelecompilateurestsuffisamentbonpouroptimisertoutapparament(unsigned index);

/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */

void initgpioparcequeilfautbienetquecestrigolodefairedesnomsarallonge(void)
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

}

void tache0parcequeilfautbienetquecestrigolodefairedesnomsarallonge(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep(1000);
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, LEDV);
	}
}

void tache1parcequeilfautbienetquecestrigolodefairedesnomsarallonge(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep(1100);
        GPIO_toggleOutputOnPin(GPIO_PORT_P9, LEDR);
    }
}

void jesuisunetachedinterruptnommeedemaniereparticulierementdouteuseetlonguemaisonsenfouspuisquelecompilateurestsuffisamentbonpouroptimisertoutapparament(unsigned index)
{
    uint16_t status = GPIO_getInterruptStatus(GPIO_PORT_P1, BTN1 + BTN2);

    switch (status)
    {
    case BTN1 :
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, LEDV);
        GPIO_clearInterrupt(GPIO_PORT_P1, BTN1);
        break;

    case BTN2 :
        GPIO_toggleOutputOnPin(GPIO_PORT_P9, LEDR);
        GPIO_clearInterrupt(GPIO_PORT_P1, BTN2);
        break;

    default :
        break;
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
    initgpioparcequeilfautbienetquecestrigolodefairedesnomsarallonge();

    /* Start BIOS */
    BIOS_start();
    return (0);
}
