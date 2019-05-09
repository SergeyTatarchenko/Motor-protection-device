#include "stm32f0xx_ext.h"

void EXTI_Init(){
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	
	
	/*PC1..PC6 as input with pull down, high speed*/
	
	GPIOC->MODER &= ~(GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|
					  GPIO_MODER_MODER4|GPIO_MODER_MODER5|GPIO_MODER_MODER6);
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR1_1|GPIO_PUPDR_PUPDR2_1|GPIO_PUPDR_PUPDR3_1|
					  GPIO_PUPDR_PUPDR4_1|GPIO_PUPDR_PUPDR5_1|GPIO_PUPDR_PUPDR6_1);
	GPIOC->OSPEEDR|= (GPIO_OSPEEDER_OSPEEDR1|GPIO_OSPEEDER_OSPEEDR2|GPIO_OSPEEDER_OSPEEDR3|
					  GPIO_OSPEEDER_OSPEEDR4|GPIO_OSPEEDER_OSPEEDR5|GPIO_OSPEEDER_OSPEEDR6);
	
	/*PA0 is input with pull down, high speed*/
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;
	GPIOA->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR0;
		
	/*EXTI config, connect EXTI1-6 to GPIOC*/
	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PC|SYSCFG_EXTICR1_EXTI2_PC|SYSCFG_EXTICR1_EXTI3_PC);
	SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PC|SYSCFG_EXTICR2_EXTI5_PC|SYSCFG_EXTICR2_EXTI6_PC);
	
	/*rising edge interrupt on PC1..PC6*/
	EXTI->RTSR |=(EXTI_RTSR_TR1|EXTI_RTSR_TR2|EXTI_RTSR_TR3|EXTI_RTSR_TR4|EXTI_RTSR_TR5|EXTI_RTSR_TR6);

	/*EXTI config, connect EXTI0 to GPIOA, user button*/
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	/*rising edge interrupt on PA0*/
	EXTI->RTSR |=EXTI_RTSR_TR0;
	
	/*user button interrupt mask (for demo)*/
	EXTI->IMR |=(EXTI_IMR_MR0);

}

void NVIC_Init(){	
	
	NVIC_SetPriority (EXTI0_1_IRQn,5);
	NVIC_SetPriority (EXTI2_3_IRQn,5);
	NVIC_SetPriority (EXTI4_15_IRQn,5);
		
	/*enable global interrupt*/
	__enable_irq ();
}
void EnableEXTI_Interupts(void){
	
	NVIC_EnableIRQ(EXTI0_1_IRQn);	
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void DisableEXTI_Interupts(){

	NVIC_DisableIRQ(EXTI0_1_IRQn);	
	NVIC_DisableIRQ(EXTI2_3_IRQn);
	NVIC_DisableIRQ(EXTI4_15_IRQn);
}

/*EXTI line 0 and 1 interrupt handler*/
void EXTI0_1_IRQHandler(){
	
	static portBASE_TYPE xTaskWoken = pdFALSE;
	
	/*interrupt on PA0, user button */
	if(EXTI->PR & EXTI_PR_PR0){
		
		/*switch display image */
		if(ContentSwitching < 3){
			ContentSwitching++;
		}else{
			ContentSwitching = 1;
		}
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR0;
	}
	/*start measuring the phase shift for phase A and stop for measuring phase B */
	/*interrupt on PC1 */
	if(EXTI->PR & EXTI_PR_PR1){			
		/*start timer for fhase A shift*/
		PHASEMETER_A_START;
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR1;
	}
		
	
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}

/*EXTI line 2 and 3 interrupt handler*/
void  EXTI2_3_IRQHandler(){
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*start measuring the phase shift for phase B and stop for measuring phase C */

	/*interrupt on PC2 */
	if(EXTI->PR & EXTI_PR_PR2){
				
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR2;
	}

	/*interrupt on PC3 */
	if(EXTI->PR & EXTI_PR_PR3){
		
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR3;
	}
			
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}

/*EXTI line 4 and 5 interrupt handler*/
void EXTI4_15_IRQHandler(){
	
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*start measuring the phase shift for phase C and stop for measuring phase A */

	/*interrupt on PC4 */
	if(EXTI->PR & EXTI_PR_PR4){

		/*stop timer for fhase A shift*/
		PHASEMETER_A_STOP;
		
		if(PHASEMETER_A_VALUE > 0){
			/*get value from timer in us*/
			PowerFactorPointer->PhaseA_Factor = PHASEMETER_A_VALUE;
			/*clear buffer*/
			PHASEMETER_A_VALUE = 0;	
		}
		
		GPIOC->BSRR |= GPIO_BSRR_BS_9;
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR4;
	}

	/*interrupt on PC5 */
	if(EXTI->PR & EXTI_PR_PR5){
		
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR5;
	}

	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}

