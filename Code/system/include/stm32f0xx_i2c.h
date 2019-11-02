#ifndef I2C_H
#define I2C_H
#include "stm32f0xx.h"
/*-----------local define-----------------------*/
#define I2C_SPEED	400000UL
#define I2C_TimeOut	1000UL
#define I2C_ERROR	\
	((I2C1 ->ISR & I2C_ISR_BERR)\
	||(I2C1->ISR & I2C_ISR_NACKF)\
	||(I2C1->ISR & I2C_ISR_ARLO )\
	||(I2C1->ISR & I2C_ISR_OVR)\
	||(I2C1->ISR & I2C_ISR_TIMEOUT)\
	||(I2C1->ISR & I2C_ISR_PECERR))	
/*-----------global function prototypes---------*/
extern void    			I2CInit(void);
extern uint_least8_t	I2CSendData(uint8_t adress,uint8_t *data,int lenght);
extern uint_least8_t	I2CGetData (uint8_t chip_adress,uint8_t adress_byte, uint8_t *data,int lenght);
void I2CSendByte(uint8_t byte);
/*-----------local function prototypes----------*/
#endif
