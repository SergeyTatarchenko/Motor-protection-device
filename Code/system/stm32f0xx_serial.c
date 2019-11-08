#include "stm32f0xx_serial.h"

void usart_init(){
	/*PA9 - TX PA10 - RX */	
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER   |= (GPIO_MODER_MODER10_1|GPIO_MODER_MODER9_1);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR9|GPIO_OSPEEDR_OSPEEDR9);    
	GPIOB->AFR[1] |= (1<<4)|(1<<8);	
	/*usart config*/
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->CR1 |= (USART_CR1_UE| USART_CR1_RXNEIE| USART_CR1_TE| USART_CR1_RE);
	USART1->CR3 |=	USART_CR3_DMAT;
}

void usart_speed(uint32_t usartdiv)
{
	/* config div USART for clock 9600 kbit/s 
	*  USARTDIV = 9600 
	* baud = f_ck / (16*USARTDIV)
	*
	* f_ck = 48MHz
	* baud = 468,75 
	* 468 -> 1D4
	* 0.75 * 16 = 12 -> C
	* BRR = 0x00001D4C
	*/	
	float baud = 0.0;
	uint32_t div_fraction = 0;
	uint32_t div_mantissa = 0;
	uint32_t brr = 0;
	
//	baud = (float)PLL_FREQ / (16.0 * (float)usartdiv);	
	
	div_fraction = (int)(baud * 100); 		
	div_fraction = div_fraction % 100;	
	div_fraction = (float)(div_fraction/100.0)*16.0;
	
	div_mantissa = baud * 16;				
	
	brr = div_mantissa | div_fraction;
	
	USART1->BRR = brr;
}