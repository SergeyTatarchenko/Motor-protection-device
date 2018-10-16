#include "stm32f0xx_adc.h"

void ADC_Init(){
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |=	RCC_APB2ENR_ADCEN;
	
	/*PA1 is ADC1_IN1*/
	GPIOA->MODER |= GPIO_MODER_MODER1;
	
	/*adc calibration*/
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR&ADC_CR_ADCAL) == ADC_CR_ADCAL);
	/*adc clock is PCLK/4*/
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;
	/*adc enable*/
	ADC1->CR  |= ADC_CR_ADEN;
	/*continious mode */
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	/*channel 1*/
	ADC1->CHSELR = ADC_CHSELR_CHSEL1;
	
	/*adc start conversion*/
	ADC1->CR |= ADC_CR_ADSTART;
}

uint32_t ADC_CalcValue(void){
		uint32_t temp;
		temp = (uint32_t)((ADC1->DR * (uint32_t)ADC_Ref)/(uint32_t)ADC_Depth);
		return temp;
}
