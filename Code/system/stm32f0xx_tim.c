#include "stm32f0xx_tim.h"

void GeneralTimerConfig(){
		
	/*enable clock for TIM15/16/17*/
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN|RCC_APB2ENR_TIM16EN|RCC_APB2ENR_TIM17EN;
	/*enable clock for TIM3/6/14*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN|RCC_APB1ENR_TIM6EN|RCC_APB1ENR_TIM14EN;
	
	/*PORTB clock*/
	RCC->AHBENR  |=  RCC_AHBENR_GPIOBEN;
	
	/*TIM15 CH1 on PB14*/
	GPIOB->AFR[1] |= (1<<24);
	/*TIM17 CH1 on PB9*/
	GPIOB->AFR[1] |= (2<<4);
	/*TIM16 CH1 on PB8*/
	GPIOB->AFR[1] |= (2);
	
	/*AF on PB14*/
	GPIOB->MODER 	|= GPIO_MODER_MODER14_1;
	/*AF on PB9*/
	GPIOB->MODER 	|= GPIO_MODER_MODER9_1;
	/*AF on PB8*/
	GPIOB->MODER 	|= GPIO_MODER_MODER8_1;
	
	/*TIM15/16/17 used for background frequency measurement*/
	
	/******** TIM15 config*********/
	/*Get 1KHz timer*/
	TIM15->PSC = TIMER_PSC_1;
	/*select TI1 for TIM15_CH1*/
	TIM15->CCMR1 |= TIM_CCMR1_CC1S_0;
	/*dont use psc and filter */
  	TIM15->CCMR1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC1PSC);
	/*DMA enable*/
	TIM15->DIER |= TIM_DIER_CC1DE ;
	/*rising edge*/
  	TIM15->CCER &= ~TIM_CCER_CC1P;
	/*CCP enable*/
  	TIM15->CCER |= TIM_CCER_CC1E;         

	/******** TIM16 config*********/
  	/*Get 1KHz timer*/
	TIM16->PSC = TIMER_PSC_1;
	/*select TI1 for TIM15_CH1*/
	TIM16->CCMR1 |= TIM_CCMR1_CC1S_0;
	/*dont use psc and filter */
  	TIM16->CCMR1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC1PSC);
	/*DMA enable*/
	TIM16->DIER |= TIM_DIER_CC1DE ;
	/*rising edge*/
  	TIM16->CCER &= ~TIM_CCER_CC1P;
	/*CCP enable*/
  	TIM16->CCER |= TIM_CCER_CC1E;   

  	/******** TIM17 config*********/
  	
  	/*Get 1KHz timer*/
	TIM17->PSC = TIMER_PSC_1;
	/*select TI1 for TIM15_CH1*/
	TIM17->CCMR1 |= TIM_CCMR1_CC1S_0;
	/*dont use psc and filter */
  	TIM17->CCMR1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC1PSC);
	/*DMA enable*/
	TIM17->DIER |= TIM_DIER_CC1DE ;
	/*rising edge*/
  	TIM17->CCER &= ~TIM_CCER_CC1P;
	/*CCP enable*/
  	TIM17->CCER |= TIM_CCER_CC1E; 
	
	
	/*TIM3/6/14 used to implement a phase meter*/
	TIM3->PSC = TIMER_PSC_2;
	TIM6->PSC = TIMER_PSC_2;
	TIM14->PSC= TIMER_PSC_2;
	
}

void EnableGeneralTimers(){

	/*enable TIM15*/
  	TIM15->CR1 |= TIM_CR1_CEN; 
  	/*enable TIM16*/
  	TIM16->CR1 |= TIM_CR1_CEN; 
  	/*enable TIM17*/
  	TIM17->CR1 |= TIM_CR1_CEN; 
}

void DisableGeneralTimers(){
	
	TIM15->CR1 &= ~TIM_CR1_CEN; 
  	TIM16->CR1 &= ~TIM_CR1_CEN; 
  	TIM17->CR1 &= ~TIM_CR1_CEN; 
}
