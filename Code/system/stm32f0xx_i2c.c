#include "stm32f0xx_i2c.h"
/*************************************************
Init I2C module, Master mode, speed 400 KHz
*************************************************/
void I2CInit()
{
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; 
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	/* calculated in I2C timing configuration tool*/
	I2C1->TIMINGR = 0x00901A53;
	I2C1->CR1 |= I2C_CR1_PE;	
}
 
uint_least8_t	I2CSendData (uint8_t adress,uint8_t *data, int lenght){
	uint_least8_t state = 0;
	int counter = 0;
	/*write operation*/
	I2C1->CR2 &= ~(I2C_CR2_NBYTES|I2C_CR2_SADD); 
	I2C1->CR2 |= (lenght<<16)|(adress<<1);
	I2C1->CR2 |= I2C_CR2_START; 	/* Go */
	while((I2C1->ISR & I2C_ISR_TXIS) != I2C_ISR_TXIS);
	if(lenght < 2) {
		I2CSendByte(*data);
	}
	else{
		for(counter = 0; counter< lenght;counter++){
		I2CSendByte(data[counter]);
		}
	}
	while((I2C1->ISR & I2C_ISR_TC) != I2C_ISR_TC);
	I2C1->CR2 |= I2C_CR2_STOP; 	/* Stop */
	while ((I2C1->ISR & I2C_ISR_BUSY));
	I2C1->ICR |= I2C_ICR_STOPCF;
	if(I2C_ERROR){
		return state;
	}
	else{
		return !state;
	}
}

uint_least8_t	I2CGetData (uint8_t chip_adress,uint8_t adress_byte, uint8_t *data,int lenght){
	uint_least8_t state = 0;
	return state;
	
}
void I2CSendByte(uint8_t byte){
	I2C1->TXDR = (uint8_t)byte;
	while((I2C1->ISR & I2C_ISR_TXE) != I2C_ISR_TXE);
}
