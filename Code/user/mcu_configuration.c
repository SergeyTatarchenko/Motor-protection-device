/*-----------------------------------------------------------------------
mcu config v 1.1
------------------------------------------------------------------------*/
#include "mcu_configuration.h"


/*----------------------------------------------------------------------
general microcontroller pin configuration
------------------------------------------------------------------------*/
void mcu_gpio_init(void)
{
	/*mcu pin configuration*/
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;
	/*----------------------------ADC unit------------------------------*/
	/*
	PA1, PA2, PA3, PA7 config in analog mode
	*/	
	GPIOA->MODER   |= (GPIO_MODER_MODER1|GPIO_MODER_MODER2|
	                   GPIO_MODER_MODER3|GPIO_MODER_MODER7);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR1|GPIO_OSPEEDR_OSPEEDR2|
	                   GPIO_OSPEEDR_OSPEEDR3|GPIO_OSPEEDR_OSPEEDR7);
	/*----------------------------I2C unit------------------------------*/
	/*
	PB6, PB7 config as AF1 (I2C )
	*/
	GPIOB->MODER   |=  (GPIO_MODER_MODER7_1|GPIO_MODER_MODER6_1);
	GPIOB->OTYPER  |=  (GPIO_OTYPER_OT_7|GPIO_OTYPER_OT_6);
	GPIOB->AFR[0]  |=  ((1<<24)|(1<<28));
	GPIOB->BSRR    |=  (GPIO_BSRR_BS_6|GPIO_BSRR_BS_7);
	/*---------------------------CCP unit ------------------------------*/
	/*
	PB8 config as AF2 (TIM16), PB9 config as AF2 (TIM17), PB14 config as
	AF1 (TIM15)
	*/
	GPIOB->MODER   |= (GPIO_MODER_MODER14_1|GPIO_MODER_MODER9_1|
	                    GPIO_MODER_MODER8_1);
	GPIOB->AFR[1]  |= ((2)|(2<<4)|(1<<24));
	/*--------------------------EXTI unit ------------------------------*/
	/*
	PA4, PA5, PA6, PB10, PB11, PB12 config as input with pull down 
	*/
	GPIOA->MODER   &= ~(GPIO_MODER_MODER4|GPIO_MODER_MODER5|
	                    GPIO_MODER_MODER6);
	GPIOB->MODER   &= ~(GPIO_MODER_MODER10|GPIO_MODER_MODER11|
	                    GPIO_MODER_MODER12);
	GPIOA->PUPDR   |= (GPIO_PUPDR_PUPDR4_1|GPIO_PUPDR_PUPDR5_1|
	                   GPIO_PUPDR_PUPDR6_1);
	GPIOB->PUPDR   |= (GPIO_PUPDR_PUPDR10_1|GPIO_PUPDR_PUPDR11_1|
	                   GPIO_PUPDR_PUPDR12_1);				  
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4|GPIO_OSPEEDER_OSPEEDR5|
	                   GPIO_OSPEEDER_OSPEEDR6);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10|GPIO_OSPEEDER_OSPEEDR11|
	                   GPIO_OSPEEDER_OSPEEDR12);
	/*---------------------------LED unit ------------------------------*/
	/*
	PB0, PB1 config as output push pull
	*/
	GPIOB->MODER   |= (GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0);
	/*-------------------------USART unit ------------------------------*/
	/*
	PA9 config as AF1 (USART TX), PA10 config as AF1 (USART RX)
	*/
	GPIOA->MODER   |= (GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR9|GPIO_OSPEEDR_OSPEEDR10);
	GPIOA->AFR[1]  |= ((1<<4)|(1<<8));
	/*--------------------TRIAC control pin-----------------------------*/
	/*
	PA0 config as output oped drain
	*/
	GPIOA->MODER   |= GPIO_MODER_MODER0_0;
	GPIOA->OTYPER  |= GPIO_OTYPER_OT_0;
	/*---------------------TEMP control pin-----------------------------*/
	/*
	PA8 config as output push pull
	*/
	GPIOA->MODER   |= GPIO_MODER_MODER8_0;
}
/*----------------------------------------------------------------------
microcontroller peripherals configuration
------------------------------------------------------------------------*/
void mcu_peripheral_init()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;      /*SYSCFG clock enable*/
	I2CInit();
	ADC_Init();
	USART_init();
	EXTI_Init();
	NVIC_Init();
	DMA_Init();
	TIMConfig();
}

void SysInit()
{
	mcu_gpio_init();
	mcu_peripheral_init();
	
	CapturedVoltagePointer =& CapturedVoltage;
	VoltageTextLCDPointer =& VoltageTextLCD;
	CapturedPeriodPointer =&  CapturedPeriod;
	PeriodLCDPointer = & PeriodLCD;
	PowerFactorPointer = & PowerFactor;
	PowerFactorLCDPointer = & PowerFactorLCD;
	WatchDogPointer = & WatchDog;
	MotorConfigurationPointer = & MotorConfiguration;
}

void led_0_invertor(void)
{
	static uint8_t invertor = 0;
	if(invertor)
	{
		led_0_off;
	}
	else
	{
		led_0_on;
	}
	invertor = ~invertor;
}

void led_1_invertor(void)
{
	static uint8_t invertor = 0;
	if(invertor)
	{
		led_1_off;
	}
	else
	{
		led_1_on;
	}
	invertor = ~invertor;
}

void watchdog_config()
{
	/* WD access enable */
	IWDG->KR = 0x5555;
	/*WD prescaler = 16, clock = 40 kHz/16 = 2,5 kHz
		1) 100 ms = 10 Hz, 2500/10 = 250
		2) 500 ms = 2 Hz   2500/2  = 1250
	*/
	IWDG->PR |= IWDG_PR_PR_1; /*010 - divider /16*/
//	IWDG->RLR = 250;		/*reset CPU after 100 ms*/
	IWDG->RLR = 1250;		/*reset CPU after 500 ms*/	
	
	IWDG_RELOAD;
	/*enable debug*/
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
	/*WD enable*/
	IWDG->KR = 0xCCCC;
}
