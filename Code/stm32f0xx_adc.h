#ifndef ADC_H
#define ADC_H

#include "stm32f0xx.h"

#define ADC_on	(ADC1->CR |= ADC_CR_ADSTART)
#define ADC_off	(ADC1->CR |= ADC_CR_ADSTP)

extern void ADC_Init(void);

#endif
