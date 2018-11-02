#include "stm32f0xx_adc.h"

void ADC_Init(){
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |=	RCC_APB2ENR_ADCEN;
	
	/*PA1,PA2,PA3 configured as ADC IN*/
	GPIOA->MODER |= GPIO_MODER_MODER1|
					GPIO_MODER_MODER2|
					GPIO_MODER_MODER3;
	
	/*adc calibration*/
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR&ADC_CR_ADCAL) == ADC_CR_ADCAL);
	/*adc clock is PCLK/4*/
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;
	/*adc enable*/
	ADC1->CR  |= ADC_CR_ADEN;
	/*continious mode */
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	/*enable DMA for ADC with circular mode*/
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG; 
	/*channel 1,2,3*/
	ADC1->CHSELR = ADC_CHSELR_CHSEL1|ADC_CHSELR_CHSEL2|ADC_CHSELR_CHSEL3;
}

/*calc adc value, return result in mV/10 */
int ADC_CalcValue(uint32_t data){
		int temp;
		temp = (int)((data * (int)ADC_Ref)/(int)ADC_Depth);
		if ((temp < 0)||(temp>ADC_Ref)){
			temp = 0;
		}
		return temp/10;
}
