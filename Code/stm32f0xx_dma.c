#include "stm32f0xx_dma.h"


void DMA_InitADC(){

/*enable DMA*/
RCC->AHBENR |= RCC_AHBENR_DMAEN;
/*pointer to peripheral memory*/
DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
DMA1_Channel1->CMAR = (uint32_t)CapturedVoltageArray;
DMA1_Channel1->CNDTR = 3;
DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
DMA1_Channel1->CCR |= DMA_CCR_EN;

}
