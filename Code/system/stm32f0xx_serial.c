#include "stm32f0xx_serial.h"

void usart_init(){
	/*PA9 - TX PA10 - RX */	
	RCC->AHBENR    |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER   |= (GPIO_MODER_MODER10_1|GPIO_MODER_MODER9_1);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR9|GPIO_OSPEEDR_OSPEEDR10);    
	GPIOA->AFR[1] |= ((1<<4)|(1<<8));	
	/*usart config*/
	RCC->CFGR3   |=  RCC_CFGR3_USART1SW_0; 
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//USART1->BRR  = 0x1388;
	USART1->CR1 |= (USART_CR1_UE| USART_CR1_RXNEIE| USART_CR1_TE| USART_CR1_RE);
//	USART1->CR3 |=	USART_CR3_DMAT;
}

void usart_speed(uint32_t baudrate)
{	
	uint16_t USARTDIV = 0;
	const uint32_t pll_freq = 48000000;
	USARTDIV = (uint16_t)(pll_freq/baudrate);
	USART1->BRR = USARTDIV;
}
