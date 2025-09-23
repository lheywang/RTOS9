#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"

#include <driverlib.h>

void Init_AdcRef(void)
{
    Ref_A_setReferenceVoltage(REF_A_BASE, REF_A_VREF2_0V);
    Ref_A_enableReferenceVoltage(REF_A_BASE);
    Ref_A_enableTempSensor(REF_A_BASE);

    while(Ref_A_isVariableReferenceVoltageOutputReady(REF_A_BASE) != REF_A_READY);
}

void Init_ADC(void)
{
    Init_AdcRef();

    ADC12_B_initParam params = {0};

    params.clockSourceDivider =             ADC12_B_CLOCKDIVIDER_1;
    params.clockSourcePredivider =          ADC12_B_CLOCKPREDIVIDER__1;
    params.clockSourceSelect =              ADC12_B_CLOCKSOURCE_ADC12OSC;
    params.internalChannelMap =             ADC12_B_TEMPSENSEMAP;
    params.sampleHoldSignalSourceSelect =   ADC12_B_CLOCKSOURCE_MCLK;

    ADC12_B_init(ADC12_B_BASE, &params);
    ADC12_B_enable(ADC12_B_BASE);

    ADC12_B_setupSamplingTimer(
            ADC12_B_BASE,
            ADC12_B_CYCLEHOLD_256_CYCLES,
            ADC12_B_CYCLEHOLD_4_CYCLES,
            ADC12_B_MULTIPLESAMPLESDISABLE
            );
}

void IRQ_Adc(unsigned index) // INT 45
{

}
