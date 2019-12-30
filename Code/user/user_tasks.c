#include <stm32f0xx.h>
#include "user_tasks.h"

//uncomment when SOM config completed
/*sys inc*/
#include "stm32f0xx_dma.h"
#include "stm32f0xx_ext.h"

uint32_t ContentSwitching = 1;

STATE device_state = PROGRAMMING;

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

/*error handler*/
void _task_error_handler(void *pvParameters)
{	
	for(;;){	
		/*error waiting*/
		xSemaphoreTake(xErrorHandler,portMAX_DELAY);
	}		
}

/*led driver 1 ms tick*/
void _task_led(void *pvParameters)
{	
	static int tick = 0, tick_reload = 1000000;
	/*config led on PB1 and PB0*/
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= (GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0);
	
	led_0_off;
	led_1_off;
	for(;;)
	{
		switch(device_state)
		{
			case IDLE:
				led_1_off;
				if(tick%100 == 0)
				{
				led_0_invertor();	
				}
			break;				
			case WORKING:
				led_0_on;
				led_1_off;
				break;
			case ALARM:
				led_0_off;
				led_1_on;	
				break;
			case PROGRAMMING:
				if(tick%1000 == 0)
				{
					led_1_invertor();
					led_0_invertor();
				}
				break;
		}
		/*tick update*/
		if(tick <= tick_reload)
		{
			tick++;
		}
		else
		{
			tick = 0;
		}	
		vTaskDelay(1);
	}
}

/*main cycle 1 ms tick*/
void _task_main(void *pvParameters)
{	
	static uint8_t tick = 0,tick_reload = 100;
	static const uint8_t adc_timing = 5,freq_timing = 25,i2c_update = 50;
	ADC_on;
	EnableEXTI_Interupts();
	
	if(LCD_ENABLE)
	{
		Init_LCD_1602();
		LCD_SetLoadingWindow();
		LCD_ClearDisplay();
		LCD_DrawWorkspace();
	}
	for(;;){
		//power_factor_conversion();
		
		
		if(tick%adc_timing == 0)
		{
			adc_conversion();			/*adc conversion*/  
		}
		if(tick%freq_timing == 0)
		{
			frequency_conversion();		/*frequency and phase failure conversion*/
		}
		if(LCD_ENABLE)
		{
			if(tick%i2c_update == 0)
			{	
				text_ascii_conversion();
				i2c_transfer();	
			}
		}
		/*tick update*/
		if(tick < tick_reload)
		{
			tick++;
		}
		else
		{
			tick = 0;
		}
		vTaskDelay(1);
	}
}

/**/
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

/**/
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

void frequency_conversion(){
	
	DisableGeneralTimers();	
	/*get period and frequency value*/
	if(TIM15_CCR1_Array[1] > TIM15_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseA = MotorConfigurationPointer->FrequencyWatchdog;
		CapturedPeriodPointer->PhaseA_Period = (TIM15_CCR1_Array[1]-TIM15_CCR1_Array[0]);
		TIM15->CNT = 0;
		TIM15->CCR1 = 0;
		TIM15_CCR1_Array[0]=0;
		TIM15_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseA_Frequency = (1*TIMER_10US)/(CapturedPeriodPointer->PhaseA_Period) + 1;
	}
	else{
		if(WatchDogPointer->FrequencyPhaseA > 0){
			WatchDogPointer->FrequencyPhaseA--;
		}
	}	
	if(TIM16_CCR1_Array[1] > TIM16_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseB = MotorConfigurationPointer->FrequencyWatchdog;
		CapturedPeriodPointer->PhaseB_Period = (TIM16_CCR1_Array[1]-TIM16_CCR1_Array[0]);
		TIM16->CNT = 0;
		TIM16->CCR1 = 0;
		TIM16_CCR1_Array[0]=0;
		TIM16_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseB_Frequency = (1*TIMER_10US)/(CapturedPeriodPointer->PhaseB_Period);  
	}else{
		if(WatchDogPointer->FrequencyPhaseB > 0){
			WatchDogPointer->FrequencyPhaseB--;
		}
	}
	if(TIM17_CCR1_Array[1] > TIM17_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseC = MotorConfigurationPointer->FrequencyWatchdog;
		CapturedPeriodPointer->PhaseC_Period = (TIM17_CCR1_Array[1]-TIM17_CCR1_Array[0]);
			TIM17->CNT = 0;
			TIM17->CCR1 = 0;
			TIM17_CCR1_Array[0]=0;
			TIM17_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseC_Frequency = (1*TIMER_10US)/(CapturedPeriodPointer->PhaseC_Period);  
	}else{
		if(WatchDogPointer->FrequencyPhaseC > 0){
			WatchDogPointer->FrequencyPhaseC--;
		}
	}
	
	EnableGeneralTimers();	
}

void adc_conversion(){

	float temp;
	/*get ADC value in mV*/
	CapturedVoltagePointer->PhaseA_Voltage_peak = ADC_CalcValue(CapturedVoltageArray[0]);
	if(CapturedVoltagePointer->PhaseA_Voltage_peak > 200){
		/*include voltage drop on diode*/
		CapturedVoltagePointer->PhaseA_Voltage_peak += (uint32_t)DIODE_DROP;
		/*input voltage divider*/
		CapturedVoltagePointer->PhaseA_Voltage_peak *= VOLTAGE_PREDIV;
		
		/*get rms voltage for display (demo) */
		temp = (float)CapturedVoltagePointer->PhaseA_Voltage_peak;
		temp = temp * 0.707;
		CapturedVoltagePointer->PhaseA_Voltage_rms = ((uint32_t)temp/10);
		/**/
	}else{
		CapturedVoltagePointer->PhaseA_Voltage_peak = 0;
		CapturedVoltagePointer->PhaseA_Voltage_rms = 0;

	}

	CapturedVoltagePointer->PhaseB_Voltage_peak = ADC_CalcValue(CapturedVoltageArray[1]);
	if(CapturedVoltagePointer->PhaseB_Voltage_peak > 200){
		/*include voltage drop on diode*/
		CapturedVoltagePointer->PhaseB_Voltage_peak += (uint32_t)DIODE_DROP;
		/*input voltage divider*/
		CapturedVoltagePointer->PhaseB_Voltage_peak *= VOLTAGE_PREDIV;
		
		/*get rms voltage for display (demo) */
		temp = (float)CapturedVoltagePointer->PhaseB_Voltage_peak;
		temp = temp * 0.707;
		CapturedVoltagePointer->PhaseB_Voltage_rms = ((uint32_t)temp/10);
		/**/
	}else{
		CapturedVoltagePointer->PhaseB_Voltage_peak = 0;
		CapturedVoltagePointer->PhaseB_Voltage_rms = 0;

	}

	CapturedVoltagePointer->PhaseC_Voltage_peak = ADC_CalcValue(CapturedVoltageArray[2]);
	if(CapturedVoltagePointer->PhaseC_Voltage_peak > 200){
		/*include voltage drop on diode*/
		CapturedVoltagePointer->PhaseC_Voltage_peak += (uint32_t)DIODE_DROP;
		/*input voltage divider*/
		CapturedVoltagePointer->PhaseC_Voltage_peak *= VOLTAGE_PREDIV;
		
		/*get rms voltage for display (demo) */
		temp = (float)CapturedVoltagePointer->PhaseC_Voltage_peak;
		temp = temp * 0.707;
		CapturedVoltagePointer->PhaseC_Voltage_rms = ((uint32_t)temp/10);
		/**/
	}else{
		CapturedVoltagePointer->PhaseC_Voltage_peak = 0;
		CapturedVoltagePointer->PhaseC_Voltage_rms = 0;

	}
}
void power_factor_conversion(){
	
	EXTI->IMR &= PHASEMETER_DEFAULT;	
	/*phase A*/
	EXTI->IMR |= PHASEMETER_A_IRQ;
	/*wait for conversion*/
	vTaskDelay(25);
	EXTI->IMR &= ~PHASEMETER_A_IRQ;
	/*add check Km function*/
	//PowerFactorPointer->PhaseA_Cos = CalcPowerFactor(PowerFactorPointer->PhaseA_Factor,CapturedPeriodPointer->PhaseA_Period);
	PowerFactorPointer->PhaseA_Cos = CalcPowerFactor(PowerFactorPointer->PhaseA_Factor,1818);
	//PowerFactorPointer->PhaseA_Cos=45;
	if(PowerFactorPointer->PhaseA_Factor>150)
	{
		BLUE_LED_ON;
	}
//	/*---------------------*/	
//	/*phase B*/
//	EXTI->IMR |= PHASEMETER_B_IRQ;
//	/*wait for conversion*/
//	vTaskDelay(25);	
//	EXTI->IMR &= ~PHASEMETER_B_IRQ;
//	PowerFactorPointer->PhaseB_Cos = CalcPowerFactor(PowerFactorPointer->PhaseB_Factor,CapturedPeriodPointer->PhaseB_Period);
//	/*---------------------*/
//	/*phase C*/
//	EXTI->IMR |= PHASEMETER_C_IRQ;
//	/*wait for conversion*/
//	vTaskDelay(25);
//	EXTI->IMR &= ~PHASEMETER_C_IRQ;
//	PowerFactorPointer->PhaseC_Cos = CalcPowerFactor(PowerFactorPointer->PhaseC_Factor,CapturedPeriodPointer->PhaseC_Period);
//	/*---------------------*/
	EXTI->IMR &= PHASEMETER_DEFAULT;
}

/*create text arrays for lcd*/
void text_ascii_conversion(){
	
	/*convert adc voltage rms value*/
	itoa(CapturedVoltagePointer->PhaseA_Voltage_rms,VoltageTextLCDPointer->PhaseA_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	itoa(CapturedVoltagePointer->PhaseB_Voltage_rms,VoltageTextLCDPointer->PhaseB_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	itoa(CapturedVoltagePointer->PhaseC_Voltage_rms,VoltageTextLCDPointer->PhaseC_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	
	/*convert frequency value*/
	itoa(CapturedPeriodPointer->PhaseA_Frequency,PeriodLCDPointer->PhaseA_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);
	itoa(CapturedPeriodPointer->PhaseB_Frequency,PeriodLCDPointer->PhaseB_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);
	itoa(CapturedPeriodPointer->PhaseC_Frequency,PeriodLCDPointer->PhaseC_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);
	
	/*convert power factor value*/
	itoa(PowerFactorPointer->PhaseA_Cos,PowerFactorLCDPointer->PhaseA_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
	itoa(PowerFactorPointer->PhaseB_Cos,PowerFactorLCDPointer->PhaseB_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
	itoa(PowerFactorPointer->PhaseC_Cos,PowerFactorLCDPointer->PhaseC_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
}


void i2c_transfer(){

	uint8_t counter;
	switch(ContentSwitching){
			
			case PHASE_A:
					LCD_SetDRAM_Adress(DDRAM_adress_row_0);
					LCD_SendChar('A');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+7);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseA_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+4);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseA_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+11);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseA_FactorArray[counter]+0x30);
				}
				
			break;

			case PHASE_B:
				LCD_SetDRAM_Adress(DDRAM_adress_row_0);
				LCD_SendChar('B');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+7);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseB_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+4);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseB_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+11);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseB_FactorArray[counter]+0x30);
				}
				
			break;

			case (PHASE_C -1):
				LCD_SetDRAM_Adress(DDRAM_adress_row_0);
				LCD_SendChar('C');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+7);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseC_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+4);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseC_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+11);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseC_FactorArray[counter]+0x30);
				}
	
			break;

			default:

			break;
		}

}

void send_usart_message(uint8_t *message,uint32_t buf_size)
{
	DMA_Ch4_Reload(message,buf_size);
}


void LCD_DrawWorkspace()
{
	LCD_SetDRAM_Adress(DDRAM_adress_row_0 + 2);
	LCD_Write("Vrms=");
	LCD_SetDRAM_Adress(DDRAM_adress_row_1 + 2);
	LCD_Write("F=");
	LCD_SetDRAM_Adress(DDRAM_adress_row_1 + 8);
	LCD_Write("Km=");
}

void LCD_SetLoadingWindow()
{
	LCD_SetDRAM_Adress(DDRAM_adress_row_0);
	LCD_Write("....LOADING....");
	DELAY(150);
	LCD_SetDRAM_Adress(DDRAM_adress_row_1);
	for(int i = 0 ; i < 16; i++)
	{
		LCD_Write(".");
		DELAY(150);
	}
}
