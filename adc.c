#include "adc.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"

#include <driverlib.h>
#include <ti/sysbios/knl/Event.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

void Init_AdcRef(void)
{
    Ref_A_setReferenceVoltage(REF_A_BASE, REF_A_VREF2_0V);
    Ref_A_enableReferenceVoltage(REF_A_BASE);
    Ref_A_enableTempSensor(REF_A_BASE);

    while(Ref_A_isVariableReferenceVoltageOutputReady(REF_A_BASE) != REF_A_READY);
}

void Init_ADC(void)
{
    // Call Reference init
    Init_AdcRef();

    // Init struct
    ADC12_B_initParam params = { 0 };
    params.clockSourceDivider =                 ADC12_B_CLOCKDIVIDER_1;
    params.clockSourcePredivider =              ADC12_B_CLOCKPREDIVIDER__1;
    params.clockSourceSelect =                  ADC12_B_CLOCKSOURCE_ADC12OSC;
    params.internalChannelMap =                 ADC12_B_TEMPSENSEMAP;
    params.sampleHoldSignalSourceSelect =       ADC12_B_CLOCKSOURCE_MCLK;

    ADC12_B_init(ADC12_B_BASE, &params);

    // HAL ADC Init
    ADC12_B_enable(ADC12_B_BASE);
    ADC12_B_setupSamplingTimer(
            ADC12_B_BASE,
            ADC12_B_CYCLEHOLD_256_CYCLES,
            ADC12_B_CYCLEHOLD_4_CYCLES,
            ADC12_B_MULTIPLESAMPLESDISABLE
    );


    // Memory init struct
    ADC12_B_configureMemoryParam ADC_StructParam = { 0 };
    ADC_StructParam.differentialModeSelect =    ADC12_B_DIFFERENTIAL_MODE_DISABLE;
    ADC_StructParam.endOfSequence =             ADC12_B_ENDOFSEQUENCE;
    ADC_StructParam.inputSourceSelect =         ADC12_B_INPUT_TCMAP;
    ADC_StructParam.memoryBufferControlIndex =  ADC12_B_MEMORY_0;
    ADC_StructParam.refVoltageSourceSelect =    ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS;
    ADC_StructParam.windowComparatorSelect =    ADC12_B_WINDOW_COMPARATOR_DISABLE;

    ADC12_B_configureMemory(ADC12_B_BASE, &ADC_StructParam);

    // Interrupt init
    ADC12_B_clearInterrupt(
            ADC12_B_BASE,
            0,
            ADC12_B_IFG0
    );

    ADC12_B_enableInterrupt(
            ADC12_B_BASE,
            ADC12_B_IE0,
            0,
            0
    );
}

void IRQ_Adc(unsigned index) // INT 45
{
    uint16_t status = ADC12_B_getInterruptStatus(
            ADC12_B_BASE,
            0,
            ADC12_B_IFG0);

    switch (status)
    {
    case ADC12_B_IFG0:
        Event_post(h_event_Adc, EVENT_ADC_CONV);
        ADC12_B_clearInterrupt(
            ADC12_B_BASE,
            0,
            ADC12_B_IFG0
        );
        break;
    }


}
