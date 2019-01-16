#include <stm32f0xx.h>
#include "user_tasks.h"

/*sys inc*/
#include "stm32f0xx_dma.h"
#include "stm32f0xx_ext.h"

uint32_t ContentSwitching = 1;

/*init peripherals and start other tasks*/
void vSysInit(void *pvParameters){
	
	/*init led on Discovery board */
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	
	/*init I2C1*/
	I2CInit();
	/*inut ADC1*/
	ADC_Init();
	/*DMA init*/
	DMA_InitADC();
	
	/*timer init*/
	GeneralTimerConfig();
	/*init DMA for TIM15*/
	DMA_InitTIM15();
	/*init DMA for TIM16*/
	DMA_InitTIM16();
	/*init DMA for TIM17*/
	DMA_InitTIM17();
		
	/*start I/O model*/
	CapturedVoltagePointer =& CapturedVoltage;
	VoltageTextLCDPointer =& VoltageTextLCD;
	CapturedPeriodPointer =&  CapturedPeriod;
	PeriodLCDPointer = & PeriodLCD;
	PowerFactorPointer = &PowerFactor;
	PowerFactorLCDPointer = & PowerFactorLCD;
	
	Init_LCD_1602();
	EXTI_Init();
	NVIC_Init();
	
	/*start program*/
	xTaskCreate(vADC_Conversion,"ADC convertion", configMINIMAL_STACK_SIZE, NULL, 2 , NULL );
	xTaskCreate(vTIM_PeriodConversion,"TIM conversion", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	xTaskCreate(vInitialStateCheck,"initial loading", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
	
	/*delete task*/
	vTaskDelete(NULL);
}

/*check state routine */
void vInitialStateCheck(void *pvParameters){
	
	uint_least8_t PowerNetworkStatus = 0;
	
	/*enable timers*/
	EnableGeneralTimers();
	/*adc start*/
	ADC_on;
	/*таймер стартует с частотой 1 кГц, АЦП настроено на 12 бит, быстрое преобразование */
	LCD_DrawBootWindow();
	/*initial delay for complete the calculation of the main parameters*/
	vTaskDelay(5000);
	
		
	/*add parameter control fuction */
	/*Проверка параметров сети, таких как частота, напряжение, последовательность фаз, разбаланс фаз*/
	PowerNetworkStatus =1;
	
	if(PowerNetworkStatus){
		/*clear display*/
		LCD_ClearDisplay();
		
		/*Enable actuator*/
		
		vTaskDelay(2000);
		LCD_DrawWorkspace();
	
		xTaskCreate(vI2CTransfer,"I2C Transmit", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
		xTaskCreate(vPowerFactorConversion,"PW conversion", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	
		EnableMetering();
		
	}else{
		/*add error control function*/
	}
	/*delete task*/
	vTaskDelete(NULL);
}
/*Create voltage array for lcd transmit, calc value from ADC1*/
void vADC_Conversion(void *pvParameters){
	/*variable for ADC value*/
	for(;;){
		/*get ADC value in mV*/
		CapturedVoltagePointer->PhaseA_Voltage = ADC_CalcValue(CapturedVoltageArray[0]);
		CapturedVoltagePointer->PhaseB_Voltage = ADC_CalcValue(CapturedVoltageArray[1]);
		CapturedVoltagePointer->PhaseC_Voltage = ADC_CalcValue(CapturedVoltageArray[2]);
		
		/*convert value to array*/
		itoa(CapturedVoltagePointer->PhaseA_Voltage,VoltageTextLCDPointer->PhaseA_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
		itoa(CapturedVoltagePointer->PhaseB_Voltage,VoltageTextLCDPointer->PhaseB_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
		itoa(CapturedVoltagePointer->PhaseC_Voltage,VoltageTextLCDPointer->PhaseC_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
		
		vTaskDelay(200);
	}
}
/*calc signal period,create array for lcd transmit*/
void vTIM_PeriodConversion(void *pvParameters){
	
	for(;;){

		/*get period and frequency value*/
		if(TIM15_CCR1_Array[1] > TIM15_CCR1_Array[0]){
			CapturedPeriodPointer->PhaseA_Period = (TIM15_CCR1_Array[1]-TIM15_CCR1_Array[0]);
			/*get value in Hz*/
			CapturedPeriodPointer->PhaseA_Frequency = (1*TIMER_MS)/(CapturedPeriodPointer->PhaseA_Period);
		}
		if(TIM16_CCR1_Array[1] > TIM16_CCR1_Array[0]){
			CapturedPeriodPointer->PhaseB_Period = (TIM16_CCR1_Array[1]-TIM16_CCR1_Array[0]);
			/*get value in Hz*/
			CapturedPeriodPointer->PhaseB_Frequency = (1*TIMER_MS)/(CapturedPeriodPointer->PhaseB_Period);  
		}
		if(TIM17_CCR1_Array[1] > TIM17_CCR1_Array[0]){
			CapturedPeriodPointer->PhaseC_Period = (TIM17_CCR1_Array[1]-TIM17_CCR1_Array[0]);
			/*get value in Hz*/
			CapturedPeriodPointer->PhaseC_Frequency = (1*TIMER_MS)/(CapturedPeriodPointer->PhaseC_Period);  
		}	
		/*convert frequency value to array*/
		itoa(CapturedPeriodPointer->PhaseA_Frequency,PeriodLCDPointer->PhaseA_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);
		itoa(CapturedPeriodPointer->PhaseB_Frequency,PeriodLCDPointer->PhaseB_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);
		itoa(CapturedPeriodPointer->PhaseC_Frequency,PeriodLCDPointer->PhaseC_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);

		vTaskDelay(400);
	}
}

/*transmit current data to lcd (demo function)*/	
void vI2CTransfer(void *pvParameters){
	uint8_t counter;
	for(;;){
		
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
	vTaskDelay(1000);
	}
}
void vPowerFactorConversion(void *pvParameters){

	for(;;){
		/*calc power factor value*/
		PowerFactorPointer->PhaseA_Cos = CalcPowerFactor(PowerFactorPointer->PhaseA_Factor,CapturedPeriodPointer->PhaseA_Period);
		PowerFactorPointer->PhaseB_Cos = CalcPowerFactor(PowerFactorPointer->PhaseB_Factor,CapturedPeriodPointer->PhaseB_Period);
		PowerFactorPointer->PhaseC_Cos = CalcPowerFactor(PowerFactorPointer->PhaseC_Factor,CapturedPeriodPointer->PhaseC_Period);
		
		/*convert frequency value to array*/
		itoa(PowerFactorPointer->PhaseA_Cos,PowerFactorLCDPointer->PhaseA_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
		itoa(PowerFactorPointer->PhaseB_Cos,PowerFactorLCDPointer->PhaseB_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
		itoa(PowerFactorPointer->PhaseC_Cos,PowerFactorLCDPointer->PhaseC_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
		
		vTaskDelay(400);
	}
}
