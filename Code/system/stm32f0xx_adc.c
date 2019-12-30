
#include "stm32f0xx_adc.h"

void ADC_Init()
{	
	RCC->APB2ENR |=	RCC_APB2ENR_ADCEN;
	/*adc clock is PCLK/4*/
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;
	/*continious mode */
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	/*enable DMA for ADC with circular mode*/
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG; 
	/*channel 1,2,3*/
	ADC1->CHSELR = ADC_CHSELR_CHSEL1|ADC_CHSELR_CHSEL2|ADC_CHSELR_CHSEL3;
	/*adc calibration*/
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR&ADC_CR_ADCAL) == ADC_CR_ADCAL);
	/*adc enable*/
	ADC1->CR  |= ADC_CR_ADEN;
}

/*calc adc value, return result in mV */
int ADC_CalcValue(uint32_t data)
{
		int temp;
		temp = (int)((data * (int)ADC_Ref)/(int)ADC_Depth);
		if ((temp < 0)||(temp>ADC_Ref)){
			temp = 0;
		}
		return temp;
}
