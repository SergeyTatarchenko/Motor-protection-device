#ifndef ADC_H
#define ADC_H

#include "stm32f0xx.h"

#define ADC_on	(ADC1->CR |= ADC_CR_ADSTART)
#define ADC_off	(ADC1->CR |= ADC_CR_ADSTP)

#define ADC_Ref     2960    /* mV*/
#define ADC_Depth   4096-1    /*12 bit adc*/

extern void     ADC_Init(void);
extern int 		ADC_CalcValue(uint32_t data);
#endif
