/*
 * spi.c
 *
 *  Created on: 30 sept. 2025
 *      Author: l.heywang
 */
#include "spi.h"

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

void init_spi(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P1,
            MOSI + MISO,
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    EUSCI_B_SPI_initMasterParam SPI_cfg = {
            .clockPhase =           EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
            .clockPolarity =        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,
            .clockSourceFrequency = 10000,
            .desiredSpiClock =      10000,
            .msbFirst =             EUSCI_B_SPI_LSB_FIRST,
            .selectClockSource =    EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
            .spiMode =              EUSCI_B_SPI_3PIN
    };

    EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &SPI_cfg);
    EUSCI_B_SPI_enable(EUSCI_B0_BASE);

    EUSCI_B_SPI_clearInterrupt(
            EUSCI_B0_BASE,
            EUSCI_B_SPI_RECEIVE_INTERRUPT + EUSCI_B_SPI_TRANSMIT_INTERRUPT
    );

    EUSCI_B_SPI_enableInterrupt(
            EUSCI_B0_BASE,
            EUSCI_B_SPI_RECEIVE_INTERRUPT + EUSCI_B_SPI_TRANSMIT_INTERRUPT
    );

    return;
}

void IRQ_spi(unsigned index)
{
    uint8_t status = EUSCI_B_SPI_getInterruptStatus(
            EUSCI_B0_BASE,
            EUSCI_B_SPI_RECEIVE_INTERRUPT + EUSCI_B_SPI_TRANSMIT_INTERRUPT
    );

    switch (status)
    {
    case EUSCI_B_SPI_RECEIVE_INTERRUPT:


        EUSCI_B_SPI_clearInterrupt(
                EUSCI_B0_BASE,
                EUSCI_B_SPI_RECEIVE_INTERRUPT
        );
        break;

    case EUSCI_B_SPI_TRANSMIT_INTERRUPT:


        EUSCI_B_SPI_clearInterrupt(
                EUSCI_B0_BASE,
                EUSCI_B_SPI_TRANSMIT_INTERRUPT
        );
        break;
    }
}

void SPI_putchar(uint8_t Data)
{
    while (EUSCI_B_SPI_isBusy(EUSCI_B0_BASE) == EUSCI_B_SPI_BUSY);

    EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, Data);

    return;
}

void SPI_putstring(uint8_t *Data)
{
    while (EUSCI_B_SPI_isBusy(EUSCI_B0_BASE) == EUSCI_B_SPI_BUSY);

    while(*Data != '\r')
    {
        while (EUSCI_B_SPI_isBusy(EUSCI_B0_BASE) == EUSCI_B_SPI_BUSY);
        EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, *Data++);
    }

    return;
}

uint8_t SPI_receive(void)
{
    while (EUSCI_B_SPI_receiveData(EUSCI_B0_BASE) != '\r')
    {
        /*
         * Warning : undefined case with  *val++ ...
         */
        *Buffer++ = EUSCI_B_SPI_receiveData(EUSCI_B0_BASE);
    }
}


