/*
 * uart.c
 *
 *  Created on: 29 sept. 2025
 *      Author: l.heywang
 */
#include "uart.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

#include <driverlib.h>

#include "main.h"

uint8_t RxData, TxData;



void init_Uart(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
                                               RXD,
                                               GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3,
                                               TXD,
                                               GPIO_PRIMARY_MODULE_FUNCTION);

    // Configure UART peripheral
    EUSCI_A_UART_initParam Uart_cfg = { .clockPrescalar =       4,
                                        .firstModReg =          5,
                                        .msborLsbFirst =        EUSCI_A_UART_LSB_FIRST,
                                        .numberofStopBits =     EUSCI_A_UART_ONE_STOP_BIT,
                                        .overSampling =         EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
                                        .parity =               EUSCI_A_UART_NO_PARITY,
                                        .secondModReg =         85,
                                        .selectClockSource =    EUSCI_A_UART_CLOCKSOURCE_SMCLK,
                                        .uartMode =             EUSCI_A_UART_MODE
    };
    EUSCI_A_UART_init(EUSCI_A1_BASE, &Uart_cfg);

    EUSCI_A_UART_clearInterrupt(
            EUSCI_A1_BASE,
            EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG + EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG
    );
    EUSCI_A_UART_enableInterrupt(
            EUSCI_A1_BASE,
            EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG + EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG
    );

    return;
}

void irq_uart(unsigned index)
{
    uint8_t status = EUSCI_A_UART_getInterruptStatus(
            EUSCI_A1_BASE,
            EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG + EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG
    );

    switch (status)
    {
    case EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG :

        RxData = EUSCI_A_UART_receiveData(EUSCI_A1_BASE);
        EUSCI_A_UART_transmitData(EUSCI_A1_BASE, RxData);

        EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
        break;

    case EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG:



        EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
        break;
    }

    return;
}




