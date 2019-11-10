#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TRUE	1
#define FALSE	0

#define PHASE_A  1
#define PHASE_B  2
#define PHASE_C  4

/*timers prescale*/
#define TIMER_MS    1000
#define TIMER_10US  100000UL

/*schematic features*/

/*voltage drop on p-n diode in mV*/
#define DIODE_DROP	510

/*input voltage divider*/
#define VOLTAGE_PREDIV	2

#define LCD_ENABLE TRUE

/*version 1.0*/
#define GREEN_LED_ON	(GPIOC->BSRR |=GPIO_BSRR_BS_9)
#define GREEN_LED_OFF	(GPIOC->BSRR |=GPIO_BSRR_BR_9)

#define BLUE_LED_ON		(GPIOC->BSRR |=GPIO_BSRR_BS_8)
#define BLUE_LED_OFF	(GPIOC->BSRR |=GPIO_BSRR_BR_8)

/*version 1.1*/
//#define led_0_on		(GPIOB->BSRR |=GPIO_BSRR_BS_0)
//#define led_0_off		(GPIOB->BSRR |=GPIO_BSRR_BR_0)

//#define led_1_on		(GPIOB->BSRR |=GPIO_BSRR_BS_1)
//#define led_1_off		(GPIOB->BSRR |=GPIO_BSRR_BR_1)

#define led_0_on		(GPIOC->BSRR |=GPIO_BSRR_BS_9)
#define led_0_off		(GPIOC->BSRR |=GPIO_BSRR_BR_9)

#define led_1_on		(GPIOC->BSRR |=GPIO_BSRR_BS_8)
#define led_1_off		(GPIOC->BSRR |=GPIO_BSRR_BR_8)


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
PB6, PB7 				- config for I2C transmit; <avaliable on A.0>
PB8, PB9, PB14			- config for frequency confersion;
PB0, PB1                - config for led control;  
PA4,PA5,PA6,PB10,PB11,PB12 - config for phase rotation,power factor and angle shear conversion,
PA9,PA10                - config for USART receive/transmit (SOM usage optional)
*/
#endif
