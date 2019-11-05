#include "stm32f0xx_dma.h"


void DMA_InitADC(){

/*enable DMA*/
RCC->AHBENR |= RCC_AHBENR_DMAEN;
/*pointer to peripheral memory*/
DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
DMA1_Channel1->CMAR = (uint32_t)CapturedVoltageArray;
DMA1_Channel1->CNDTR = 3;
DMA1_Channel1->CCR |= (DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC);
DMA1_Channel1->CCR |= DMA_CCR_EN;

}
void DMA_InitTIM15(){

/*enable DMA*/
RCC->AHBENR |= RCC_AHBENR_DMAEN;
/*pointer to peripheral memory*/

	DMA1_Channel5->CPAR = (uint32_t) (&(TIM15->CCR1));
	DMA1_Channel5->CMAR = (uint32_t)TIM15_CCR1_Array;
	
	DMA1_Channel5->CNDTR = 2;
	DMA1_Channel5->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel5->CCR |= DMA_CCR_EN;

}

void DMA_InitTIM16(){


	/*enable DMA*/
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	/*pointer to peripheral memory*/
	
	DMA1_Channel3->CPAR = (uint32_t) (&(TIM16->CCR1));
	DMA1_Channel3->CMAR = (uint32_t)TIM16_CCR1_Array;
	
	DMA1_Channel3->CNDTR = 2;
	DMA1_Channel3->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel3->CCR |= DMA_CCR_EN;

}

void DMA_InitTIM17(){
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	/*remap TIM17_CH1 on DMA Channel 2*/
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_TIM17_DMA_RMP;
	/*enable DMA*/
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	/*pointer to peripheral memory*/

	DMA1_Channel2->CPAR = (uint32_t) (&(TIM17->CCR1));
	DMA1_Channel2->CMAR = (uint32_t)TIM17_CCR1_Array;
	
	DMA1_Channel2->CNDTR = 2;
	DMA1_Channel2->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel2->CCR |= DMA_CCR_EN;

}
