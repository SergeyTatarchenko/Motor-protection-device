#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PHASE_A  1
#define PHASE_B  2
#define PHASE_C  4

#define DEFAULT_FREQUENCY_MIN		49
#define DEFAULT_FREQUENCY_MAX		51

#define FREQUENCY_SENSETIVITY		5
#define FREQUENCY_WATCHDOG_VALUE	10




/*
version 1.0 mcu - stm32f030r8t6;

PA1,PA2,PA3 			- config for ADC conversion;
PB6, PB7 				- config for I2C transmit; <demo>
PB8, PB9, PB14			- config for frequency confersion;
PC0						- config for user button;  <demo>
PC1,PC2,PC3,PC4,PC5,PC6 - config for phase rotation,power factor and angle shear conversion,
PA9,PA10                - config for USART receive/transmit (SOM usage optional)

version 1.1 mcu - stm32f030c8t6;

PA1,PA2,PA3 			- config for ADC conversion;
PB8, PB9, PB14			- config for frequency confersion;
PA4,PA5,PA6,PB10,PB11,PB12 - config for phase rotation,power factor and angle shear conversion,
PA9,PA10                - config for USART receive/transmit (SOM usage optional)
*/
#endif
