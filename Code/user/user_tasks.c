#include <stm32f0xx.h>
#include "user_tasks.h"

/*sys inc*/
#include "stm32f0xx_dma.h"
#include "stm32f0xx_ext.h"

/*init peripherals and start other tasks*/
void vSysInit(void *pvParameters){
	
	/*init led on Discovery board */
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	GPIOC->BSRR  |= GPIO_BSRR_BS_9;
	
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
	/*enable timers*/
	EnableGeneralTimers();
	
	/*start I/O model*/
	CapturedVoltagePointer =& CapturedVoltage;
	VoltageTextLCDPointer =& VoltageTextLCD;
	
	CapturedPeriodPointer =&  CapturedPeriod;
	PeriodLCDPointer = &PeriodLCD;
	
	PowerFactorPointer = &PowerFactor;
	
	/*init LCD1602*/
	Init_LCD_1602();
	
	/*adc start*/
	ADC_on;

	/****test****/
	EXTI_Init();
	NVIC_Init();
	/************/
		
	xTaskCreate(vADC_Conversion,"ADC convertion", configMINIMAL_STACK_SIZE, NULL, 2 , NULL );
	xTaskCreate(vTIM_PeriodConversion,"TIM conversion", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	
	xTaskCreate(vInitialStateCheck,"initial loading", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
	
	/*delete task*/
	vTaskDelete(NULL);
}

/*check state routine */
void vInitialStateCheck(void *pvParameters){
	
	/*initial delay for complete the calculation of the main parameters*/
	vTaskDelay(1000);
	
	/*draw boot window*/
	LCD_DrawBootWindow();
	
	vTaskDelay(1000);
	vTaskDelay(1000);
	
	/*add parameter control fuction */
	
	/*clear display*/
	LCD_ClearDisplay();
	vTaskDelay(1000);
	
	LCD_DrawWorkspace();
	xTaskCreate(vI2CTransfer,"I2C Transmit", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
	/*enable phase shift counter*/
	EnableMetering();
	
	
	
	/*delete task*/
	vTaskDelete(NULL);
}
/*transmit current data to lcd (demo function)*/	
void vI2CTransfer(void *pvParameters){
	uint8_t counter;
	for(;;){
			
			/*voltage data transmit to LCD*/
			LCD_SetDRAM_Adress(0x02);
			for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
				/*phase A */
				LCD_SendChar(VoltageTextLCDPointer->PhaseA_VoltageArray[counter]+0x30);
			}
			LCD_SetDRAM_Adress(0x07);
			for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
				/*phase B */
				LCD_SendChar(VoltageTextLCDPointer->PhaseB_VoltageArray[counter]+0x30);
				}
			LCD_SetDRAM_Adress(0x0C);
			for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
				/*phase C*/
				LCD_SendChar(VoltageTextLCDPointer->PhaseC_VoltageArray[counter]+0x30);
			}
			
			/*frequency data tramsmit to LCD*/	
			LCD_SetDRAM_Adress(0x42);
			for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
				/*phase A */
				LCD_SendChar(PeriodLCDPointer->PhaseA_PeriodArray[counter]+0x30);
			}
			LCD_SetDRAM_Adress(0x47);
			for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
				/*phase B */
				LCD_SendChar(PeriodLCDPointer->PhaseB_PeriodArray[counter]+0x30);
			}
			LCD_SetDRAM_Adress(0x4C);
			for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
				/*phase C */
				LCD_SendChar(PeriodLCDPointer->PhaseC_PeriodArray[counter]+0x30);
			}
			
			vTaskDelay(100);
		}	
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
		
		vTaskDelay(500);
	}
}

/*calc signal period,create array for lcd transmit*/
void vTIM_PeriodConversion(void *pvParameters){
	
	for(;;){
		/*get period value*/
		if(TIM15_CCR1_Array[1] > TIM15_CCR1_Array[0]){
			CapturedPeriodPointer->PhaseA_Period = TIM15_CCR1_Array[1]-TIM15_CCR1_Array[0];
		}
		if(TIM16_CCR1_Array[1] > TIM16_CCR1_Array[0]){
			CapturedPeriodPointer->PhaseB_Period = TIM16_CCR1_Array[1]-TIM16_CCR1_Array[0];
		}
		if(TIM17_CCR1_Array[1] > TIM17_CCR1_Array[0]){
			CapturedPeriodPointer->PhaseC_Period = TIM17_CCR1_Array[1]-TIM17_CCR1_Array[0];
		}
		
		/*convert value to array*/
		itoa(CapturedPeriodPointer->PhaseA_Period,PeriodLCDPointer->PhaseA_PeriodArray,DEFAULT_PERIOD_BUF_SIZE);
		itoa(CapturedPeriodPointer->PhaseB_Period,PeriodLCDPointer->PhaseB_PeriodArray,DEFAULT_PERIOD_BUF_SIZE);
		itoa(CapturedPeriodPointer->PhaseC_Period,PeriodLCDPointer->PhaseC_PeriodArray,DEFAULT_PERIOD_BUF_SIZE);
		
		vTaskDelay(200);
	}
	
}
