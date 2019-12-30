#include "stm32f0xx_dma.h"



void  DMA_Init()
{
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	/*------------------------ADC config--------------------------------*/
	DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
	DMA1_Channel1->CMAR = (uint32_t)CapturedVoltageArray;
	DMA1_Channel1->CNDTR = 3;
	DMA1_Channel1->CCR |= (DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC);
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	/*--------------------TIM15,16,17 config----------------------------*/
	DMA1_Channel5->CPAR = (uint32_t) (&(TIM15->CCR1));	         /*TIM15*/
	DMA1_Channel5->CMAR = (uint32_t)TIM15_CCR1_Array;
	DMA1_Channel5->CNDTR = 2;
	DMA1_Channel5->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel5->CCR |= DMA_CCR_EN;
	DMA1_Channel3->CPAR = (uint32_t) (&(TIM16->CCR1));			/*TIM16*/	
	DMA1_Channel3->CMAR = (uint32_t)TIM16_CCR1_Array;
	DMA1_Channel3->CNDTR = 2;
	DMA1_Channel3->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel3->CCR |= DMA_CCR_EN;
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_TIM17_DMA_RMP;				/*TIM17*/
	DMA1_Channel2->CPAR = (uint32_t) (&(TIM17->CCR1));
	DMA1_Channel2->CMAR = (uint32_t)TIM17_CCR1_Array;
	DMA1_Channel2->CNDTR = 2;
	DMA1_Channel2->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel2->CCR |= DMA_CCR_EN;	
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP;			   /*USART1*/
	DMA1_Channel4->CCR &= 0;
	DMA1_Channel4->CCR |= DMA_CCR_MINC|DMA_CCR_DIR;
	DMA1_Channel4->CPAR |= (uint32_t)&(USART1->TDR);
	/*Transfer complete interrupt enable */
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;
	/*medium priority level */
	DMA1_Channel4->CCR |= DMA_CCR_PL_1;
}

void DMA_InitADC()
{
	/*enable DMA*/
	RCC->AHBENR |= RCC_AHBENR_DMAEN;	
	/*pointer to peripheral memory*/
	DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
	DMA1_Channel1->CMAR = (uint32_t)CapturedVoltageArray;
	DMA1_Channel1->CNDTR = 3;
	DMA1_Channel1->CCR |= (DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC);
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	
}

void DMA_init_for_frequency_measurment(void)
{
	/*enable DMA*/
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	/*TIM15*/
	DMA1_Channel5->CPAR = (uint32_t) (&(TIM15->CCR1));
	DMA1_Channel5->CMAR = (uint32_t)TIM15_CCR1_Array;
	DMA1_Channel5->CNDTR = 2;
	DMA1_Channel5->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel5->CCR |= DMA_CCR_EN;
	/*TIM16*/
	DMA1_Channel3->CPAR = (uint32_t) (&(TIM16->CCR1));
	DMA1_Channel3->CMAR = (uint32_t)TIM16_CCR1_Array;
	DMA1_Channel3->CNDTR = 2;
	DMA1_Channel3->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel3->CCR |= DMA_CCR_EN;
	/*TIM17*/
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	/*remap TIM17_CH1 on DMA Channel 2*/
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_TIM17_DMA_RMP;
	DMA1_Channel2->CPAR = (uint32_t) (&(TIM17->CCR1));
	DMA1_Channel2->CMAR = (uint32_t)TIM17_CCR1_Array;
	DMA1_Channel2->CNDTR = 2;
	DMA1_Channel2->CCR |= DMA_CCR_MINC |DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
	DMA1_Channel2->CCR |= DMA_CCR_EN;	
}

void DMA_init_for_USART(void)
{
	/*DMA1 clock*/
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	/*init DMA for USART1 transmit*/
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	/*remap USART1_TX on DMA Channel 4*/
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP;
	/*memory increment mode enabled,
	memory\peripheral size size 8 bit
	single mode,memory to peripheral direction*/
	DMA1_Channel4->CCR &= 0;
	DMA1_Channel4->CCR |= DMA_CCR_MINC|DMA_CCR_DIR;
	DMA1_Channel4->CPAR |= (uint32_t)&(USART1->TDR);
	/*Transfer complete interrupt enable */
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;
	/*medium priority level */
	DMA1_Channel4->CCR |= DMA_CCR_PL_1;
}

void DMA_Ch4_Reload(uint8_t *message, int new_buf_size)
{
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	DMA1_Channel4->CMAR &= ~(uint32_t)0xffffffff;
	DMA1_Channel4->CMAR |= (uint32_t)message;
	DMA1_Channel4->CNDTR = new_buf_size;	
	DMA1_Channel4->CCR |= DMA_CCR_EN;
}

void DMA1_Channel4_IRQHandler()
{
	
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*interrupt on transfer complete */
	if(DMA1->ISR &= DMA_ISR_TCIF4){
		DMA1->IFCR |= DMA_IFCR_CTCIF4;
	}
	/*return mutex for other tasks to usart transmit*/
//	xSemaphoreGiveFromISR(xMutex_USART_BUSY,&xTaskWoken);
	
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
	
}
