#include "stm32f0xx_serial.h"

void USART_init()
{
	/*usart config*/
	RCC->CFGR3   |=  RCC_CFGR3_USART1SW_0; 
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//USART1->BRR  = 0x1388;
	USART1->CR1 |= (USART_CR1_RXNEIE| USART_CR1_TE| USART_CR1_RE);
	USART1->CR1 |=USART_CR1_UE;
	//	USART1->CR3 |=	USART_CR3_DMAT;
}

void usart_speed(uint32_t baudrate)
{	
	uint16_t USARTDIV = 0;
	const uint32_t pll_freq = 48000000;
	USARTDIV = (uint16_t)(pll_freq/baudrate);
	USART1->BRR = USARTDIV;
}
