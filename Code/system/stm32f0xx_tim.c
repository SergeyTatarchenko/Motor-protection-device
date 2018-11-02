#include "stm32f0xx_tim.h"

void GeneralTimerConfig(){
	
	/*enable clock for TIM15/16/17*/
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN|RCC_APB2ENR_TIM16EN|RCC_APB2ENR_TIM17EN;
	/*PORTB clock*/
	RCC->AHBENR  |=  RCC_AHBENR_GPIOBEN;
	
	
	
	/*TIM15 CH1 on PB15*/
	GPIOB->AFR[1] |= (1<<24);
	/*AF on PB14*/
	GPIOB->MODER 	|= GPIO_MODER_MODER14_1;
	/*Get 1KHz timer*/
	TIM15->PSC = 48000-1;
	
	/*select TI1 for TIM15_CH1*/
	TIM15->CCMR1 |= TIM_CCMR1_CC1S_0;

	/*dont use psc and */
  TIM15->CCMR1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC1PSC);
	
	/*DMA enable*/
	TIM15->DIER |= TIM_DIER_CC1DE ;
	
	/*rising edge*/
  TIM15->CCER &= ~TIM_CCER_CC1P;
	/*CCP enable*/
  TIM15->CCER |= TIM_CCER_CC1E;         

  TIM15->CR1 |= TIM_CR1_CEN; 
}
