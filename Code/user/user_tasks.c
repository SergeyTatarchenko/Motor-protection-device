#include <stm32f0xx.h>
#include "user_tasks.h"
#include "stm32f0xx_dma.h"

uint8_t TCH[4] ={CHAR_TCH};

uint8_t _PR_[4] ={CHAR_PRB};
uint8_t _m_[4] ={LETTER_m};
uint8_t _V_[4] ={LETTER_V};


/*init peripherals and start other tasks*/
void vSysInit(void *pvParameters){
	
	/*init led on Discovery board */
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	GPIOC->BSRR |= GPIO_BSRR_BS_9;
	
	/*init I2C1*/
	I2CInit();
	/*inut ADC1*/
	ADC_Init();
	/*DMA init*/
	DMA_InitADC();
	/*adc start*/
	ADC_on;
	
	/*timer init*/
	GeneralTimerConfig();
	TIM15_CCR1_Pointer = &TIM15_CCR1;
	/*init DMA for TIM15*/
	DMA_InitTIM15();
	
	
	
	
	
	/*start I/O model*/
	CapturedPeriodPointer =&  CapturedPeriod;
	CapturedVoltagePointer =& CapturedVoltage;
	VoltageTextLCDPointer =& VoltageTextLCD;
	/*init LCD1602*/
	Init_LCD_1602();
	/*write initial text */
	LCD_WriteText();
	
	xTaskCreate(vADC_Conversion, "ADC convertion", configMINIMAL_STACK_SIZE, NULL, 4 , NULL );
	xTaskCreate(vI2CTransfer, 	"I2C Transmit", configMINIMAL_STACK_SIZE, NULL, 5, NULL );
	//xTaskCreate(vTimerTest, 	"test", configMINIMAL_STACK_SIZE, NULL, 6, NULL );
	
	
	/*delete task*/
	vTaskDelete(NULL);
}

/*transmit current data to lcd*/	
void vI2CTransfer(void *pvParameters){
	uint8_t counter;
	int temp;
	for(;;){
			
			/*voltage transmit*/
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
			
			//temp = TIM15->CCR1;
			temp = TestArray[1]-TestArray[0];
		
			itoa(temp,CapturedPeriodTimer,6);
			LCD_SetDRAM_Adress(0x40);
			for(counter = 0 ; counter < 6; counter++){
				/*phase A */
				LCD_SendChar(CapturedPeriodTimer[counter]+0x30);
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
		
		vTaskDelay(100);
	}
}

