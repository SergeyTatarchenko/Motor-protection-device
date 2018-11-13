#include "stm32f0xx_ext.h"

void EXTI_Init(){
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	
	/*PC0..PC5 as input with pull down, high speed*/
	
	GPIOC->MODER &= ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|
					  GPIO_MODER_MODER3|GPIO_MODER_MODER4|GPIO_MODER_MODER5);
	
	GPIOC->PUPDR |=(GPIO_PUPDR_PUPDR0_1|GPIO_PUPDR_PUPDR1_1|GPIO_PUPDR_PUPDR2_1|
					GPIO_PUPDR_PUPDR3_1|GPIO_PUPDR_PUPDR4_1|GPIO_PUPDR_PUPDR5_1);
	
	GPIOC->OSPEEDR |=(GPIO_OSPEEDER_OSPEEDR0|GPIO_OSPEEDER_OSPEEDR1|GPIO_OSPEEDER_OSPEEDR2|
					  GPIO_OSPEEDER_OSPEEDR3|GPIO_OSPEEDER_OSPEEDR4|GPIO_OSPEEDER_OSPEEDR5);
	
	/*EXTI config, connect to GPIOC*/
	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PC|SYSCFG_EXTICR1_EXTI1_PC|SYSCFG_EXTICR1_EXTI2_PC|SYSCFG_EXTICR1_EXTI3_PC);
	SYSCFG->EXTICR[1] |=(SYSCFG_EXTICR2_EXTI4_PC|SYSCFG_EXTICR2_EXTI5_PC);
	
	/*rising edge interrupt on PC0..PC5*/
	EXTI->RTSR |=(EXTI_RTSR_TR0|EXTI_RTSR_TR1|EXTI_RTSR_TR2|EXTI_RTSR_TR3|EXTI_RTSR_TR4|EXTI_RTSR_TR5);
}

void NVIC_Init(){
	
	/*enable external interrupt on NVIC*/
	NVIC_EnableIRQ (EXTI0_1_IRQn);
	NVIC_EnableIRQ (EXTI2_3_IRQn);
	NVIC_EnableIRQ (EXTI4_15_IRQn);
	
	//NVIC_SetPriority (EXTI9_5_IRQn, 2);
	
	/*enable global interrupt*/
	__enable_irq ();
}

/*EXTI line 0 and 1 interrupt handler*/
void EXTI0_1_IRQHandler(){
	
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*start measuring the phase shift for phase A and stop for measuring phase B */
	
	EXTI->PR |= EXTI_PR_PR0;
	EXTI->PR |= EXTI_PR_PR1;
	
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}

/*EXTI line 2 and 3 interrupt handler*/
void  EXTI2_3_IRQHandler(){
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*start measuring the phase shift for phase B and stop for measuring phase C */

	EXTI->PR |= EXTI_PR_PR2;
	EXTI->PR |= EXTI_PR_PR3;
	
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}

/*EXTI line 4 and 5 interrupt handler*/
void EXTI4_15_IRQHandler(){
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*start measuring the phase shift for phase C and stop for measuring phase A */

	EXTI->PR |= EXTI_PR_PR4;
	EXTI->PR |= EXTI_PR_PR5;
	
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}

