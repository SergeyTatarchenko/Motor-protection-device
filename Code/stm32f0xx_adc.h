#ifndef ADC_H
#define ADC_H

#include "stm32f0xx.h"

#define ADC_on	(ADC1->CR |= ADC_CR_ADSTART)
#define ADC_off	(ADC1->CR |= ADC_CR_ADSTP)

#define ADC_Ref     3000    /*3000 mV*/
#define ADC_Depth   4096    /*12 bit adc*/

extern void     ADC_Init(void);
extern uint32_t ADC_CalcValue(void);
#endif
