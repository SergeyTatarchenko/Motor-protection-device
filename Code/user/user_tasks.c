#include <stm32f0xx.h>
#include "user_tasks.h"

/*sys inc*/
#include "stm32f0xx_dma.h"
#include "stm32f0xx_ext.h"

#define GREEN_LED_ON	(GPIOC->BSRR |=GPIO_BSRR_BS_9)
#define GREEN_LED_OFF	(GPIOC->BSRR |=GPIO_BSRR_BR_9)

#define BLUE_LED_ON		(GPIOC->BSRR |=GPIO_BSRR_BS_8)
#define BLUE_LED_OFF	(GPIOC->BSRR |=GPIO_BSRR_BR_8)

uint32_t ContentSwitching = 1;

/*semaphore for error handler*/
xSemaphoreHandle xErrorHandler;

/*mutex for I2C , perform correct transmit */
xSemaphoreHandle xMutex_BUS_BUSY;

void check_state_TASK(void *pvParameters){

	
	uint_least8_t PowerNetworkStatus;
	/*all peripherals init*/
	SysInit();
	
	ADC_on;
	EnableGeneralTimers();
	
	/* for demo */
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	
	/*add check network state function */
	PowerNetworkStatus = 1;
	
	if(PowerNetworkStatus == 1){
		
		xErrorHandler = xSemaphoreCreateBinary();
		xMutex_BUS_BUSY = xSemaphoreCreateMutex();

		if((xErrorHandler != NULL) && (xMutex_BUS_BUSY != NULL) ){
			
			LCD_DrawWorkspace();
			
			xTaskCreate(&error_handler_TASK,"error handler",configMINIMAL_STACK_SIZE, NULL, 4 , NULL );
			xTaskCreate(&main_TASK,"main cycle",configMINIMAL_STACK_SIZE, NULL, 3 , NULL );
		
		}else{
			/*internal cicruit error*/
		}	
	}else{
		/*add error number return function */
	}
		
	vTaskDelete(NULL);

}

void error_handler_TASK(void *pvParameters){
	
	for(;;){
		
		/*error waiting*/
		xSemaphoreTake(xErrorHandler,portMAX_DELAY);
		/*add error number return function */
		BLUE_LED_ON;
		while(1);
	}	
		
}
void main_TASK(void *pvParameters){
	
	//NVIC_EnableIRQ(EXTI0_1_IRQn);
	//NVIC_EnableIRQ(EXTI2_3_IRQn);
	//NVIC_EnableIRQ(EXTI4_15_IRQn);
	
	for(;;){
		
		adc_conversion();
		frequency_conversion();
		
		/*test error counter*/
		if(FreqErrorCnt < FREQUENCY_SENSETIVITY){
			GREEN_LED_ON;
		}
		else{
			GREEN_LED_OFF;
		}
		//	power_factor_conversion();
		text_ascii_conversion();
		
		/*i2c transmit to LCD*/
		xSemaphoreTake(xMutex_BUS_BUSY,portMAX_DELAY);
		i2c_transfer();
		xSemaphoreGive(xMutex_BUS_BUSY);
		
		vTaskDelay(100);
	}
}


void SysInit(){
	
	/*init led on Discovery board */
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	
	I2CInit();
	ADC_Init();
	GeneralTimerConfig();
	DMA_InitTIM15();
	DMA_InitTIM16();
	DMA_InitTIM17();
	DMA_InitADC();
	EXTI_Init();
	NVIC_Init();
	
	/*start I/O model*/
	CapturedVoltagePointer =& CapturedVoltage;
	VoltageTextLCDPointer =& VoltageTextLCD;
	CapturedPeriodPointer =&  CapturedPeriod;
	PeriodLCDPointer = & PeriodLCD;
	PowerFactorPointer = &PowerFactor;
	PowerFactorLCDPointer = & PowerFactorLCD;
	
	Init_LCD_1602();
}

void frequency_conversion(){
	
		/*get period and frequency value*/
		if((TIM15_CCR1_Array[1] > TIM15_CCR1_Array[0])&&(TIM15_CCR1_Array[1]<0x9C40)){
			TimerWatchDog = 10;
			
			CapturedPeriodPointer->PhaseA_Period = (TIM15_CCR1_Array[1]-TIM15_CCR1_Array[0]);
			TIM15->CCR1 = 0;
			TIM15_CCR1_Array[0]=0;
			TIM15_CCR1_Array[1]=0;
			/*get value in Hz*/
			CapturedPeriodPointer->PhaseA_Frequency = (1*TIMER_US)/(CapturedPeriodPointer->PhaseA_Period) + 1;
		}
		
	if(TIM16_CCR1_Array[1] > TIM16_CCR1_Array[0]){
		CapturedPeriodPointer->PhaseB_Period = (TIM16_CCR1_Array[1]-TIM16_CCR1_Array[0]);
		TIM16->CCR1 = 0;
		TIM16_CCR1_Array[0]=0;
		TIM16_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseB_Frequency = (1*TIMER_US)/(CapturedPeriodPointer->PhaseB_Period);  
	}
	if(TIM17_CCR1_Array[1] > TIM17_CCR1_Array[0]){
		CapturedPeriodPointer->PhaseC_Period = (TIM17_CCR1_Array[1]-TIM17_CCR1_Array[0]);
			TIM17->CCR1 = 0;
			TIM17_CCR1_Array[0]=0;
			TIM17_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseC_Frequency = (1*TIMER_US)/(CapturedPeriodPointer->PhaseC_Period);  
	}	
	
	if((CapturedPeriodPointer->PhaseA_Frequency != DEFAULT_FREQUENCY)){	
		FreqErrorCnt --;
	}else{
		FreqErrorCnt = 5;
	}
}

void adc_conversion(){
	
	/*get ADC value in mV*/
	CapturedVoltagePointer->PhaseA_Voltage = ADC_CalcValue(CapturedVoltageArray[0]);
	CapturedVoltagePointer->PhaseB_Voltage = ADC_CalcValue(CapturedVoltageArray[1]);
	CapturedVoltagePointer->PhaseC_Voltage = ADC_CalcValue(CapturedVoltageArray[2]);
	
}

void power_factor_conversion(){
	
	/*calc power factor value*/
	PowerFactorPointer->PhaseA_Cos = CalcPowerFactor(PowerFactorPointer->PhaseA_Factor,20);
	PowerFactorPointer->PhaseB_Cos = CalcPowerFactor(PowerFactorPointer->PhaseB_Factor,CapturedPeriodPointer->PhaseB_Period);
	PowerFactorPointer->PhaseC_Cos = CalcPowerFactor(PowerFactorPointer->PhaseC_Factor,CapturedPeriodPointer->PhaseC_Period);
}

/*create text arrays for lcd*/
void text_ascii_conversion(){
	
	/*convert adc value*/
	itoa(CapturedVoltagePointer->PhaseA_Voltage,VoltageTextLCDPointer->PhaseA_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	itoa(CapturedVoltagePointer->PhaseB_Voltage,VoltageTextLCDPointer->PhaseB_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	itoa(CapturedVoltagePointer->PhaseC_Voltage,VoltageTextLCDPointer->PhaseC_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	
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
					LCD_SetDRAM_Adress(0x01);
					LCD_SendChar('A');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+3);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseA_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+10);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseA_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+6);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseA_FactorArray[counter]+0x30);
				}
				
			break;

			case PHASE_B:
				LCD_SetDRAM_Adress(0x01);
				LCD_SendChar('B');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+3);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseB_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+10);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseB_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+6);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseB_FactorArray[counter]+0x30);
				}
				
			break;

			case PHASE_C:
				LCD_SetDRAM_Adress(0x01);
				LCD_SendChar('C');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+3);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseC_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+10);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseC_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+6);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseC_FactorArray[counter]+0x30);
				}
	
			break;

			default:

			break;
		}

}
