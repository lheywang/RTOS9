#ifndef ADC_D_
#define ADC_D_

#include <ti/sysbios/knl/Event.h>

#define EVENT_ADC_CONV Event_Id_01

void Init_ADC(void);
void IRQ_Adc(unsigned index);

#endif
