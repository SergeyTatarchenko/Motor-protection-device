#include "stm32f0xx_serial.h"

void usart_init(){
	
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	/*PA9 - TX PA10 - RX */
	GPIOA->MODER  |= (GPIO_MODER_MODER10_1|GPIO_MODER_MODER9_1);
	GPIOA->OTYPER |= (GPIO_OTYPER_OT_7|GPIO_OTYPER_OT_6);
	GPIOB->AFR[1] |= (1<<4)|(1<<8);
	GPIOA->BSRR |=GPIO_BSRR_BR_9|GPIO_BSRR_BR_10;
	
	/*usart config*/
		/* config div USART for clock 9600 kbit/s */
	/* USARTDIV = 9600 */
	/* 
	* baud = f_ck / (16*USARTDIV)
	*
	* f_ck = 72MHz
	* baud = 468,75 
	* 468 -> 1D4
	* 0.75 * 16 = 12 -> C
	* BRR = 0x00001D4C
	*/
	//USART1->BRR = 0x1D4C;		// f_ck = 72MHz, baud 9600
//	USART1->BRR = 0x186A;		// f_ck = 60MHz, baud 9600
//	USART_BRR_DIV_Mantissa ;				//  DIV_Mantissa[11:0]
//	USART_BRR_DIV_Fraction ;				//  DIV_Fraction[3:0]
	//USART_BRR |= USART_BRR_DIV_Mantissa;
	
	USART1->CR1 |= USART_CR1_UE				// bit_13	Usart Enable
//				| USART_CR1_M				// bit_12	Word Length 0: 1 Start bit, 8 Data bits, n Stop bit; 	1: 1 Start bit, 9 Data bits, n Stop bit
//				| USART_CR1_WAKE			// bit_11	Wakeup method - 0: Idle Line;	1: Address Mark
//				| USART_CR1_PCE				// bit_10	Parity control enable
//				| USART_CR1_PS				// bit_9	Parity selection - 0: Even parity	1: Odd parity 
//				| USART_CR1_PEIE			// bit_8	PE interrupt enable - 1: A USART interrupt is generated whenever PE=1 in the USART_SR register
//				| USART_CR1_TXEIE			// bit_7	TXE interrupt enable - 1: A USART interrupt is generated whenever TXE=1 in the USART_SR register
//				| USART_CR1_TCIE			// bit_6	Transmission complete interrupt enable - 1: A USART interrupt is generated whenever TC=1 in the USART_SR register
				| USART_CR1_RXNEIE			// bit_5	RXNE interrupt enable - 1: A USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register
//				| USART_CR1_IDLEIE			// bit_4	IDLE interrupt enable - 1: A USART interrupt is generated whenever IDLE=1 in the USART_SR register
				| USART_CR1_TE				// bit_3	Transmitter enable
				| USART_CR1_RE				// bit_2	Receiver enable	 
//				| USART_CR1_RWU				// bit_1	Receiver wakeup - 0: Receiver in active mode	1: Receiver in mute mode
//				| USART_CR1_SBK				// bit_0	Send break - 0: No break character is transmitted	1: Break character will be transmitted
	;
	
////	USART1->CR2 |= USART_CR2_LINEN			// bit_14	LIN mode enable 0: LIN mode disabled	1: LIN mode enabled
//////				| USART_CR2_STOP_0			// bit_13	Stop bit 00: 1 Stop bit		01: 0.5 Stop bit	10: 2 Stop bits		11: 1.5 Stop bits
//////				| USART_CR2_STOP_1
//				| USART_CR2_CLKEN			// bit_11	Clock enable
//				| USART_CR2_CPOL			// bit_10	Clock polarity	0: Steady low value on CK pin outside transmission window	1: Steady high value on CK pin outside transmission window
//				| USART_CR2_CPHA			// bit_9	Clock phase		0: The first clock transition is the first data capture edge	1: The second clock transition is the first data capture edge.
//				| USART_CR2_LBCL			// bit_8	Last bit clock pulse	0: The clock pulse of the last data bit is not output to the CK pin		1: The clock pulse of the last data bit is output to the CK pin
//				| USART_CR2_LBDIE			// LIN break detection interrupt enable	1: An interrupt is generated whenever LBD=1 in the USART_SR register
//				| USART_CR2_LBDL			// lin break detection length	0: 10 bit break detection	1: 11 bit break detection
				
//				| (USART_CR2_ADD & 0x0001)	// Address of the USART node
	;


	USART1->CR3 |= /*USART_CR3_CTSIE			// bit_10	CTS interrupt enable	1: An interrupt is generated whenever CTS=1 in the USART_SR register
////				|*/ //USART_CR3_CTSE			// bit_9	CTS	enable 	0: CTS hardware flow control disabled
//				| USART_CR3_RTSE			// bit_8	RTS enable	0: RTS hardware flow control disabled
				 USART_CR3_DMAT			// bit_7	DMA enable transmitter
//				| USART_CR3_DMAR			// bit_6	DMA enable receiver
//				| USART_CR3_SCEN			// bit_5	Smartcard mode enable
//				| USART_CR3_NACK			// bit_4	Smartcard NACK enable	0: NACK transmission in case of parity error is disabled	1: NACK transmission during parity error is enabled
//				| USART_CR3_HDSEL			// bit_3	Half-duplex selection
//				| USART_CR3_IRLP			// bit_2	IrDA low-power	0: Normal mode
//				| USART_CR3_IREN			// bit_1	IrDA mode enable
//				| USART_CR3_EIE				// bit_0	Error interrupt enable	1: An interrupt is generated whenever DMAR=1 in the USART_CR3 register and FE=1 or ORE=1 or NE=1 in the USART_SR register.
	;
	
//	USART1->GTPR |= (USART_GTPR_GT & 0x0000) // bit_15 - bit_8 Guard time value
//				| (USART_GTPR_PSC & 0x0000)	// bit_7 - bit_0	Prescaler value
	;
}
