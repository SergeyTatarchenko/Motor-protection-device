#include <stm32f0xx.h>
#include <stm32f0xx.h>
#include "user_tasks.h"

/*sys inc*/
#include "stm32f0xx_dma.h"
#include "stm32f0xx_ext.h"

/*-------------------------*/
#include "OBJ_MODEL.h"
#include "obj_model_config.h"
/*-------------------------*/
uint32_t ContentSwitching = 1;


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
		
		Init_LCD_1602();
		LCD_DrawWorkspace();
		OBJ_Init();
		ADC_on;
	for(;;){
		
		adc_conversion();
		frequency_conversion();
		CheckPowerNetwork();
		
		/*not tested*/
	//	power_factor_conversion();
		/*working part*/
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
	usart_init();
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
	PowerFactorPointer = & PowerFactor;
	PowerFactorLCDPointer = & PowerFactorLCD;
	
	WatchDogPointer = & WatchDog;
	MotorConfigurationPointer = & MotorConfiguration;
	
	/*initial parameter loading*/
	MotorConfiguration.MaxPhaseVoltage = 0;
	MotorConfiguration.MinPhaseVoltage = 0;
	MotorConfiguration.MinPhasefrequency = (uint16_t)DEFAULT_FREQUENCY_MIN;
	MotorConfiguration.MaxPhasefrequency = (uint16_t)DEFAULT_FREQUENCY_MAX;
	
}

void frequency_conversion(){
	
			DisableGeneralTimers();	
	/*get period and frequency value*/
	if(TIM15_CCR1_Array[1] > TIM15_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseA = FREQUENCY_WATCHDOG_VALUE;
		CapturedPeriodPointer->PhaseA_Period = (TIM15_CCR1_Array[1]-TIM15_CCR1_Array[0]);
		TIM15->CNT = 0;
		TIM15->CCR1 = 0;
		TIM15_CCR1_Array[0]=0;
		TIM15_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseA_Frequency = (1*TIMER_US)/(CapturedPeriodPointer->PhaseA_Period) + 1;
	}
	else{
		if(WatchDogPointer->FrequencyPhaseA > 0){
			WatchDogPointer->FrequencyPhaseA--;
		}
	}	
	if(TIM16_CCR1_Array[1] > TIM16_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseB = FREQUENCY_WATCHDOG_VALUE;
		CapturedPeriodPointer->PhaseB_Period = (TIM16_CCR1_Array[1]-TIM16_CCR1_Array[0]);
		TIM16->CNT = 0;
		TIM16->CCR1 = 0;
		TIM16_CCR1_Array[0]=0;
		TIM16_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseB_Frequency = (1*TIMER_US)/(CapturedPeriodPointer->PhaseB_Period);  
	}else{
		if(WatchDogPointer->FrequencyPhaseB > 0){
			WatchDogPointer->FrequencyPhaseB--;
		}
	}
	if(TIM17_CCR1_Array[1] > TIM17_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseC = FREQUENCY_WATCHDOG_VALUE;
		CapturedPeriodPointer->PhaseC_Period = (TIM17_CCR1_Array[1]-TIM17_CCR1_Array[0]);
			TIM17->CNT = 0;
			TIM17->CCR1 = 0;
			TIM17_CCR1_Array[0]=0;
			TIM17_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseC_Frequency = (1*TIMER_US)/(CapturedPeriodPointer->PhaseC_Period);  
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
	/*получение значения Km с каждой фазы*/
	
	/*phase A*/
	EXTI->IMR |= PHASEMETER_A_IRQ;
	/*wait for conversion*/
	vTaskDelay(25);
	EXTI->IMR &= ~PHASEMETER_A_IRQ;
	/*add check Km function*/
	PowerFactorPointer->PhaseA_Cos = CalcPowerFactor(PowerFactorPointer->PhaseA_Factor,CapturedPeriodPointer->PhaseA_Period);
	/*---------------------*/	
	/*phase B*/
	EXTI->IMR |= PHASEMETER_B_IRQ;
	/*wait for conversion*/
	vTaskDelay(25);	
	EXTI->IMR &= ~PHASEMETER_B_IRQ;
	PowerFactorPointer->PhaseB_Cos = CalcPowerFactor(PowerFactorPointer->PhaseB_Factor,CapturedPeriodPointer->PhaseB_Period);
	/*---------------------*/
	/*phase C*/
	EXTI->IMR |= PHASEMETER_C_IRQ;
	/*wait for conversion*/
	vTaskDelay(25);
	EXTI->IMR &= ~PHASEMETER_C_IRQ;
	PowerFactorPointer->PhaseC_Cos = CalcPowerFactor(PowerFactorPointer->PhaseC_Factor,CapturedPeriodPointer->PhaseC_Period);
	/*---------------------*/
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
//	
//	/*convert power factor value*/
//	itoa(PowerFactorPointer->PhaseA_Cos,PowerFactorLCDPointer->PhaseA_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
//	itoa(PowerFactorPointer->PhaseB_Cos,PowerFactorLCDPointer->PhaseB_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
//	itoa(PowerFactorPointer->PhaseC_Cos,PowerFactorLCDPointer->PhaseC_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
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
